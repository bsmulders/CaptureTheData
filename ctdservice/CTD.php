<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

require_once('Sensor.php');

class CTD {

	// Input
	public $config;
	public $db;
	public $sensors;
	public $host;
	public $uri;
	public $method;
	public $httpAccepts;

	// Output
	public $rootElement;
	public $outputData;
	public $outputType;
	public $returnHeaders;

	public function __construct($config) {
		$this->config = parse_ini_file($config, TRUE);

		try {
			$this->db = new PDO($this->config['database']['pdo'], $this->config['database']['username'], $this->config['database']['password']);
		}
		catch(PDOException $e) {
			echo $e->getMessage();
			exit(-1);
		}

		foreach($this->config['sensors'] as $sensorName=>$tableName) {
			$this->sensors[] = new Sensor($sensorName, $tableName);
		}

		$this->uri = strtok($_SERVER['REQUEST_URI'], '?');
		$this->host = $_SERVER['HTTP_HOST'];
		$this->method = $_SERVER['REQUEST_METHOD'];
		$this->httpAccepts = explode(",", $_SERVER['HTTP_ACCEPT']);
	}

	public function run() {
		$urlparts = explode('/', $this->uri);

		if (sizeof($this->httpAccepts) > 0) {
			$this->outputType = $this->httpAccepts[0];
			$this->returnHeaders[] = "Content-type: " . $this->httpAccepts[0];
		}

		if (count($urlparts) == 3
		&& $urlparts[1] == "ctdservice"
		&& $urlparts[2] == "") {
			$this->handleNoRequest();
		}
		else if (count($urlparts) == 3
		&& $urlparts[1] == "ctdservice"
		&& $urlparts[2] == "trips") {
			if ($this->method == "POST") {
				$this->handleTripPutRequest();
			}

			$this->handleTripsGetRequest();
		}
		else if (count($urlparts) == 4
		&& $urlparts[1] == "ctdservice"
		&& $urlparts[2] == "trips"
		&& is_numeric($urlparts[3])) {
			$tripid = $urlparts[3];
			$this->handleTripGetRequest($tripid);
		}
		else if (count($urlparts) == 7
		&& $urlparts[1] == "ctdservice"
		&& $urlparts[2] == "trips"
		&& is_numeric($urlparts[3])
		&& $urlparts[4] == "measurement"
		&& is_numeric($urlparts[5])
		&& is_numeric($urlparts[6])) {
			$tripid = $urlparts[3];
			$timestamp = $urlparts[5];
			$timestampsub = $urlparts[6];
			$this->handleMeasurementGetRequest($tripid, $timestamp, $timestampsub);
		}
		else {
			$this->handleWrongRequest();
		}
	}

	public function getOutput() {
		switch($this->outputType) {
			case "text/html":
				return "<h1>" . $this->rootElement . "</h1><pre>" . print_r($this->outputData, true) . "</pre>";
			case "application/json":
				return json_encode(array($this->rootElement => $this->outputData), JSON_NUMERIC_CHECK);
			default:
				return $this->rootElement . "\n" . print_r($this->outputData, true);
		}
	}

	public function getReturnHeaders() {
		return $this->returnHeaders;
	}

	public function finalize() {
		$this->db = null;
	}

	private function handleNoRequest() {
		$this->rootElement = "Error";
		$this->outputData = "Use /trips<br />";
		$this->returnHeaders[] = "HTTP/1.0 400 Bad Request";
	}

	private function handleWrongRequest() {
		$this->rootElement = "Error";
		$this->outputData = "Wrong request";
		$this->returnHeaders[] = "HTTP/1.0 400 Bad Request";
	}

	private function handleTripsGetRequest() {
		$sql = "SELECT * FROM Trip";
		$results = $row = $this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);

		foreach ($results as &$result) {
			$result['URI'] = "http://" . $this->host . "/ctdservice/trips/" . $result['Trip_ID'];
		}

		$this->rootElement = "trips";
		$this->outputData = $results;
	}

	private function handleTripPutRequest() {
		$input = json_decode(file_get_contents('php://input'), true);
		$data = $input['trips'];
		unset($data['URI']);
		unset($data['Sensors']);
		foreach ($data as $field=>$value) {
			$fields[] = "'" . $field . "'";
			$values[] = "'" . $this->db->quote($value) . "'";
		}
		$field_list = join(",", $fields);
		$value_list = join(",", $values);
		$this->db->exec("REPLACE INTO Trip (" . $field_list . ") VALUES (" . $value_list . ")");

		$this->returnHeaders[] = "HTTP/1.0 200 OK";
	}

	private function handleTripGetRequest($tripid) {
		$sql = "SELECT * FROM Trip WHERE Trip_ID = " . $this->db->quote($tripid);
		$result = $this->db->query($sql)->fetch(PDO::FETCH_ASSOC);
		$result['URI'] = "http://" . $this->host . "/ctdservice/trips/" . $tripid;
		$result['FirstMeasurement'] = "http://" . $this->host . "/ctdservice/trips/" . $tripid . "/measurement/" . (int) $result['StartTime'] . "/0";

		foreach($this->sensors as $sensor) {
			$result['Sensor'][]  = $sensor->getSensorName();
		}

		$this->rootElement = "trip";
		$this->outputData = $result;
	}

	private function handleMeasurementGetRequest($tripid, $timestamp, $timestampsub) {
		$result = array();

		foreach($this->sensors as $sensor) {
			$sql = "SELECT * FROM " . $sensor->getTableName() . " WHERE Trip_ID = " . $this->db->quote($tripid) . " AND TimeStamp = " . $this->db->quote($timestamp) . " AND TimeStampSub = " . $this->db->quote($timestampsub);
			$result[$sensor->getSensorName()] = $this->db->query($sql)->fetch(PDO::FETCH_ASSOC);
			unset($result[$sensor->getSensorName()]['Trip_ID']);
			unset($result[$sensor->getSensorName()]['TimeStamp']);
			unset($result[$sensor->getSensorName()]['TimeStampSub']);
		}

		$this->rootElement = "report";
		$this->outputData = $result;
	}
}

?>