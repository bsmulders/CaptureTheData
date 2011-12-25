<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

include('Sensor.php');

$db = new SQLite3('testset.sqlite');

$config = parse_ini_file("settings.ini", true);
$sensors = array();
foreach($config['sensors'] as $sensorName=>$tableName) {	
	$sensors[] = new Sensor($sensorName, $tableName);
}

$uri = strtok($HTTP_SERVER_VARS['REQUEST_URI'], '?');
$host = $HTTP_SERVER_VARS['HTTP_HOST'];
$urlparts = explode('/', $uri);

// No request
if (count($urlparts) == 3 && $urlparts[1] == "ctdservice" && $urlparts[2] == "") {
	echo "Use /trips<br />";
}
// Requested all trips
else if (count($urlparts) == 3 && $urlparts[1] == "ctdservice" && $urlparts[2] == "trips") {
	if ($HTTP_SERVER_VARS['REQUEST_METHOD'] == "POST") {
		$input = json_decode(file_get_contents('php://input'), true);
		$data = $input['trips'];
		unset($data['URI']);
		unset($data['Sensors']);		
		foreach ($data as $field=>$value) {
			$fields[] = "'" . $field . "'";
			$values[] = "'" . sqlite_escape_string($value) . "'";
		}
		$field_list = join(",", $fields);
		$value_list = join(",", $values);
		$db->query("REPLACE INTO Trip (" . $field_list . ") VALUES (" . $value_list . ")");
	} 

	$result = array();
	
	$query = $db->query("SELECT * FROM Trip");
	while ($row = $query->fetchArray(SQLITE3_ASSOC)) {
		$row['URI'] = "http://" . $host . "/ctdservice/trips/" . $row['Trip_ID'];
		$result[] = $row;
	}
	
	echo json_encode(array("trips" => $result));
}
// Requested a certain trip
else if (count($urlparts) == 4 && $urlparts[1] == "ctdservice" && $urlparts[2] == "trips" && is_numeric($urlparts[3])) {
	$tripid = $urlparts[3];
	
	$result = array();
	
	$query = $db->query("SELECT * FROM Trip WHERE Trip_ID = " . sqlite_escape_string($tripid));
	$result = $query->fetchArray(SQLITE3_ASSOC);
	$result['URI'] = "http://" . $host . "/ctdservice/trips/" . $tripid;
	$result['FirstMeasurement'] = "http://" . $host . "/ctdservice/trips/" . $tripid . "/measurement/" . (int) $result['StartTime'] . "/0";
	$result['Sensors'] = array_keys($sensors);
	
	echo json_encode(array("trip" => $result));
}
// Request a certain measurement
else if (count($urlparts) == 7 && $urlparts[1] == "ctdservice" && $urlparts[2] == "trips" && is_numeric($urlparts[3]) && $urlparts[4] == "measurement" && is_numeric($urlparts[5]) && is_numeric($urlparts[6])) {
	$tripid = $urlparts[3];
	$timestamp = $urlparts[5];
	$timestampsub = $urlparts[6];
	
	$result = array();
	
	foreach($sensors as $sensor) {
		$query = $db->query("SELECT * FROM " . $sensor->getTableName() . " WHERE Trip_ID = ".sqlite_escape_string($tripid)." AND TimeStamp = " . sqlite_escape_string($timestamp) . " AND TimeStampSub = " . sqlite_escape_string($timestampsub));
		$result[$sensor->getSensorName()] = $query->fetchArray(SQLITE3_ASSOC);
		unset($result[$sensor]['Trip_ID']);
		unset($result[$sensor]['TimeStamp']);
		unset($result[$sensor]['TimeStampSub']);
	}

	echo json_encode(array("report" => $result));
}

$db->close();

?>