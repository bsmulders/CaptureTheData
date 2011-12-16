<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

$db = new SQLite3('ctd.sqlite');

$uri = strtok($HTTP_SERVER_VARS['REQUEST_URI'], '?');
$host = $HTTP_SERVER_VARS['HTTP_HOST'];
$urlparts = explode('/', $uri);

// No request
if (count($urlparts) == 3 && $urlparts[1] == "ctdservice" && $urlparts[2] == "") {
	echo "Homepage<br />";
}
// Requested all trips
else if (count($urlparts) == 3 && $urlparts[1] == "ctdservice" && $urlparts[2] == "trips") {
	//echo "Requested all trips<br />";
	
	$result = array();
	
	$query = $db->query("SELECT * FROM Trip");
	while ($row = $query->fetchArray(SQLITE3_ASSOC)) {
		$row['StartTimeStamp'] = (int) $row['StartTime'];
		$row['EndTimeStamp'] = (int) $row['EndTime'];
		$row['StartTime'] = date(DATE_RFC850, $row['StartTime']);
		$row['EndTime'] = date(DATE_RFC850, $row['EndTime']);
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
	$result['StartTimeStamp'] = (int) $result['StartTime'];
	$result['EndTimeStamp'] = (int) $result['EndTime'];
	$result['StartTime'] = date(DATE_RFC850, $result['StartTime']);
	$result['EndTime'] = date(DATE_RFC850, $result['EndTime']);
	$result['URI'] = "http://" . $host . "/ctdservice/trips/" . $tripid;
	
	$sensors = array("gsm"=>"GsmReport", "gps"=>"GpsReport");
	foreach ($sensors as $sensor=>$tablename) {
		$report = array("URI" => $result['URI'] . "/" . $sensor);
		$query = $db->query("SELECT MIN(TimeStamp) AS min, MAX(TimeStamp) AS max FROM ".$tablename." WHERE Trip_ID = " . sqlite_escape_string($tripid));		
		$timestamps = $query->fetchArray(SQLITE3_ASSOC);
		$report['FirstReport'] = array(	"Time" => date(DATE_RFC850, $timestamps['min']),
										"TimeStamp" => $timestamps['min'],
										"URI" => "http://" . $host . "/ctdservice/trips/" . $tripid . "/" . $sensor . "/" . $timestamps['min'] );
		$report['LastReport'] = array(	"Time" => date(DATE_RFC850, $timestamps['max']),
										"TimeStamp" => $timestamps['min'],
										"URI" => "http://" . $host . "/ctdservice/trips/" . $tripid . "/" . $sensor . "/" . $timestamps['max']);
		$result['Sensors'][$sensor] = $report;
	}

	echo json_encode(array("trip" => array($result)));
}
// Requested a certain sensor
else if (count($urlparts) == 5 && $urlparts[1] == "ctdservice" && $urlparts[2] == "trips" && is_numeric($urlparts[3]) && $urlparts[4] != "") {
	$tripid = $urlparts[3];
	$sensor = $urlparts[4];
	
	$result = array();
	
	if ($sensor == "gps") {
		$query = $db->query("SELECT MIN(TimeStamp) AS min, MAX(TimeStamp) AS max FROM GpsReport WHERE Trip_ID = " . sqlite_escape_string($tripid));
	}
	else if ($sensor == "gsm") {
		$query = $db->query("SELECT MIN(TimeStamp) AS min, MAX(TimeStamp) AS max FROM GsmReport WHERE Trip_ID = " . sqlite_escape_string($tripid));		
	}
	
	if ($query) {
		$timestamps = $query->fetchArray(SQLITE3_ASSOC);
		$result['FirstReport'] = array(	"Time" => date(DATE_RFC850, $timestamps['min']),
										"TimeStamp" => $timestamps['min'],
										"URI" => "http://" . $host . "/ctdservice/trips/" . $tripid . "/" . $sensor . "/" . $timestamps['min']);
		$result['LastReport'] = array(	"Time" => date(DATE_RFC850, $timestamps['max']),
										"TimeStamp" => $timestamps['max'],
										"URI" => "http://" . $host . "/ctdservice/trips/" . $tripid . "/" . $sensor . "/" . $timestamps['max']);
	}
	
	echo json_encode(array("sensor" => array($result)));
}
// Request a certain report
else if (count($urlparts) == 6 && $urlparts[1] == "ctdservice" && $urlparts[2] == "trips" && is_numeric($urlparts[3]) && $urlparts[4] != "" && is_numeric($urlparts[5])) {
	$tripid = $urlparts[3];
	$sensor = $urlparts[4];
	$timestamp = $urlparts[5];
	
	$result = array();
	
	if ($sensor == "gps") {
		$query = $db->query("SELECT * FROM GpsReport WHERE Trip_ID = ".sqlite_escape_string($tripid)." AND (TimeStamp + (TimeStampSub / 10.0)) = " . sqlite_escape_string($timestamp));
	}
	else if ($sensor == "gsm") {
		$query = $db->query("SELECT * FROM GsmReport WHERE Trip_ID = ".sqlite_escape_string($tripid)." AND (TimeStamp + (TimeStampSub / 10.0)) = " . sqlite_escape_string($timestamp));		
	}
	
	if ($query) {
		$result = $query->fetchArray(SQLITE3_ASSOC);
		//$result['NextMeasurement'] = "http://" . $host . "/ctdservice/trips/" . $tripid . "/" . $sensor . "/" . sprintf("%.1f", $result['TimeStampFull'] + 0.1);
		//$result['Time'] = date(DATE_RFC850, $result['TimeStamp']);
		unset($result['Trip_ID']);
	}
	
	echo json_encode(array("report" => array($result)));
}

$db->close();

?>