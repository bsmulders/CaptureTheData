<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

class Sensor {

	public $sensorName;
	public $tableName;

	public function __construct($sensorName, $tableName) {
		$this->setSensorName($sensorName);
		$this->setTableName($tableName);
	}

	public function getSensorName() {
		return $this->sensorName;
	}

	public function setSensorname($sensorName) {
		$this->sensorName = $sensorName;
	}

	public function getTableName() {
		return $this->tableName;
	}

	public function setTableName($tableName) {
		$this->tableName = $tableName;
	}

}

?>