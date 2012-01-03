<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

require_once('Sensor.php');
require_once('HTTPRequest.php');
require_once('HTTPResponse.php');

class CTD {

	/**
	 * The configuration file generated from the .ini file
	 * @var object[]
	 */
	private $config;

	/**
	 * PDO database connection handler
	 * @var PDO
	 */
	private $db;

	/**
	 * Array containing the currently supported sensors
	 * @var Sensor[]
	 */
	private $sensors;

	/**
	 * Class containing information about the HTTP request
	 * @var HTTPRequest
	 */
	private $httpRequest;

	/**
	 * Class containing information about the HTTP reponse
	 * @var HTTPResponse
	 */
	private $httpReponse;

	/**
	 * Constructor. Creates a new CTD object using a .ini filepath
	 * @param string $config
	 */
	public function __construct($config) {
		$this->config = parse_ini_file($config, TRUE);

		$this->db = new PDO($this->config['database']['pdo'], $this->config['database']['username'], $this->config['database']['password']);

		foreach($this->config['sensors'] as $sensorName=>$tableName) {
			$this->sensors[] = new Sensor($sensorName, $tableName);
		}

		$this->httpRequest = new HTTPRequest($_SERVER);
		$this->httpReponse = new HTTPResponse();
	}
	
	/**
	 * Get the HTTP request object associated with this instance
	 * @return HTTPRequest
	 */
	public function getHTTPRequest() {
		return $this->httpRequest;
	}
	
	/**
	 * Get the HTTP response object associated with this instance
	 * @return HTTPResponse
	 */
	public function getHTTPResponse() {
		return $this->httpReponse;
	}

	/**
	 * Process the HTTP request
	 */
	public function run() {
		if (sizeof($this->httpRequest->getHttpAccepts()) > 0) {
			$httpAccepts = array_values($this->config['httpaccept']);
			$httpAccept = $this->httpRequest->getHttpAccept($httpAccepts);
				
			$this->httpReponse->setOutputType($httpAccept);
			$this->httpReponse->addReturnHeader(sprintf("Content-type: %s", $httpAccept));
		}

		$urlparts = explode('/', $this->httpRequest->getUri());

		if (count($urlparts) == 3
		&& $urlparts[1] == "ctdservice"
		&& $urlparts[2] == "") {
			$this->handleNoRequest();
		}
		else if (count($urlparts) == 3
		&& $urlparts[1] == "ctdservice"
		&& $urlparts[2] == "trips") {
			if ($this->httpRequest->getMethod() == "POST") {
				$this->handleTripPutRequest();
			}
			else {
				$this->handleTripsGetRequest();
			}
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

	/**
	 * The user didn't request any data
	 */
	private function handleNoRequest() {
		$this->httpReponse->setRootElement("Error");
		$this->httpReponse->setOutputData("Use /trips");
		$this->httpReponse->addReturnHeader("HTTP/1.0 400 Bad Request");
	}

	/**
	 * The user requested data using a bad URI
	 */
	private function handleWrongRequest() {
		$this->httpReponse->addReturnHeader("HTTP/1.0 400 Bad Request");
	}

	/**
	 * Build a list of all trips
	 */
	private function handleTripsGetRequest() {
		$sql = "SELECT * FROM Trip";
		$results = $this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);

		if ($results) {
			foreach ($results as &$result) {
				$result['URI'] = sprintf("http://%s/ctdservice/trips/%d", $this->httpRequest->getHost(), $result['Trip_ID']);
			}

			$this->httpReponse->addReturnHeader("HTTP/1.0 200 OK");
			$this->httpReponse->setRootElement("trips");
			$this->httpReponse->setOutputData($results);
		}
		else {
			$this->httpReponse->addReturnHeader("HTTP/1.0 204 No Content");
		}
	}

	/**
	 * Process a PUT request on a certain trip
	 */
	private function handleTripPutRequest() {
		$input = json_decode(file_get_contents('php://input'), true);
		$data = $input['trips'];
		unset($data['URI']);
		unset($data['Sensors']);

		foreach ($data as $field=>$value) {
			$fields[] = sprintf("'%s'", $field);
			$values[] = sprintf("%s", $this->db->quote($value));
		}

		$sql = sprintf("REPLACE INTO Trip (%s) VALUES (%s)", join(",", $fields), join(",", $values));
		$result = $this->db->exec($sql);

		if ($result) {
			$this->httpReponse->addReturnHeader("HTTP/1.0 200 OK");
		}
		else {
			$this->httpReponse->addReturnHeader("HTTP/1.0 500 Internal Server Error");
		}
	}

	/**
	 * Build list with one specific trip
	 * @param int $tripid
	 */
	private function handleTripGetRequest($tripid) {
		$sql = sprintf("SELECT * FROM Trip WHERE Trip_ID = %s", $this->db->quote($tripid));
		$result = $this->db->query($sql)->fetch(PDO::FETCH_ASSOC);

		if ($result) {
			$result['URI'] = sprintf("http://%s/ctdservice/trips/%d", $this->httpRequest->getHost(), $tripid);
			$result['FirstMeasurement'] = sprintf("http://%s/ctdservice/trips/%d/measurement/%d/%d", $this->httpRequest->getHost(), $tripid, $result['FirstReport'], $result['FirstReportSub']);

			foreach($this->sensors as $sensor) {
				$result['Sensor'][]  = $sensor->getSensorName();
			}
		
			$this->httpReponse->addReturnHeader("HTTP/1.0 200 OK");
			$this->httpReponse->setRootElement("trip");
			$this->httpReponse->setOutputData($result);
		}
		else {
			$this->httpReponse->addReturnHeader("HTTP/1.0 404 Not Found");
		}
	}

	/**
	 * Build a list with all sensor data
	 * @param int $tripid
	 * @param int $timestamp
	 * @param int $timestampsub
	 */
	private function handleMeasurementGetRequest($tripid, $timestamp, $timestampsub) {
		$results = array();
		$empty = true;

		foreach($this->sensors as $sensor) {
			$sql = sprintf("SELECT * FROM %s WHERE Trip_ID = %s AND TimeStamp = %s AND TimeStampSub = %s", $sensor->getTableName(), $this->db->quote($tripid), $this->db->quote($timestamp), $this->db->quote($timestampsub));
			$result = $this->db->query($sql)->fetch(PDO::FETCH_ASSOC);
			$results[$sensor->getSensorName()] = $result;

			if ($result) {
				$empty = false;
				unset($results[$sensor->getSensorName()]['Trip_ID']);
				unset($results[$sensor->getSensorName()]['TimeStamp']);
				unset($results[$sensor->getSensorName()]['TimeStampSub']);
			}
		}

		if (!$empty) {
			$this->httpReponse->addReturnHeader("HTTP/1.0 200 OK");
			$this->httpReponse->setRootElement("report");
			$this->httpReponse->setOutputData($results);
		}
		else {
			$this->httpReponse->addReturnHeader("HTTP/1.0 404 Not Found");
		}
	}

	/**
	 * Finalize this object
	 */
	public function finalize() {
		$this->db = null;
	}
}

?>