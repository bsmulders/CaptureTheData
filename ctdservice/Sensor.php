<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

class Sensor {

	/**
	 * The name of a sensor
	 * @var string
	 */
	private $sensorName;
	
	/**
	 * The name of the corresponding SQL table
	 * @var string
	 */
	private $tableName;

	/**
	 * Constructor. Creates a new sensor type using the parameters
	 * @param string $sensorName
	 * @param string $tableName
	 */
	public function __construct($sensorName, $tableName) {
		$this->setSensorName($sensorName);
		$this->setTableName($tableName);
	}

	/**
	 * Get the name of the sensor
	 * @return string
	 */
	public function getSensorName() {
		return $this->sensorName;
	}

	/**
	 * Set the name of the sensor
	 * @param string $sensorName
	 */
	public function setSensorname($sensorName) {
		$this->sensorName = $sensorName;
	}

	/**
	 * Get the name of the corresponding SQL table
	 * @return string
	 */
	public function getTableName() {
		return $this->tableName;
	}

	/**
	 * Set the name of the corresponding SQL table
	 * @param string $tableName
	 */
	public function setTableName($tableName) {
		$this->tableName = $tableName;
	}

}

?>