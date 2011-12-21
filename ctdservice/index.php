<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

$db = new SQLite3('ctd.sqlite');
$sensors = array("GSM"=>"GsmReport", "GPS"=>"GpsReport", "OBD"=>"ObdReport");

$uri = strtok($HTTP_SERVER_VARS['REQUEST_URI'], '?');
$host = $HTTP_SERVER_VARS['HTTP_HOST'];
$urlparts = explode('/', $uri);

// No request
if (count($urlparts) == 3 && $urlparts[1] == "ctdservice" && $urlparts[2] == "") {
	echo "Homepage<br />";
}
// Requested all trips
else if (count($urlparts) == 3 && $urlparts[1] == "ctdservice" && $urlparts[2] == "trips") {	
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
	$result['FirstMeasurement'] = "http://" . $host . "/ctdservice/trips/" . $tripid . "/measurement/" . $result['StartTimeStamp'];
	$result['Sensors'] = array_keys($sensors);
	
	echo json_encode(array("trip" => $result));
}
// Request a certain measurement
else if (count($urlparts) == 6 && $urlparts[1] == "ctdservice" && $urlparts[2] == "trips" && is_numeric($urlparts[3]) && $urlparts[4] == "measurement" && is_numeric($urlparts[5])) {
	$tripid = $urlparts[3];
	$timestamp = $urlparts[5];
	
	$result = array();
	
	foreach($sensors as $sensor=>$tablename) {
		$query = $db->query("SELECT * FROM " . $tablename . " WHERE Trip_ID = ".sqlite_escape_string($tripid)." AND (TimeStamp + (TimeStampSub / 10.0)) = " . sqlite_escape_string($timestamp));
		$result[$sensor] = $query->fetchArray(SQLITE3_ASSOC);
		unset($result[$sensor]['Trip_ID']);
		unset($result[$sensor]['TimeStamp']);
		unset($result[$sensor]['TimeStampSub']);
	}

	$result['NextMeasurement'] = "http://" . $host . "/ctdservice/trips/" . $tripid . "/measurement/" . ($timestamp + 0.1);

	echo json_encode(array("report" => $result));
}

$db->close();

?>