/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.model.Measurement', {
	extend : 'Ext.data.Model',
	fields : [ 'GSM', 'GPS', 'OBD', 'GSM.Gsm_ID', 'GSM.SignalStrenght',
			'GPS.Gps_ID', 'GPS.UTC', 'GPS.Fix', 'GPS.Latitude',
			'GPS.Longitude', 'GPS.Speed', 'GPS.Direction', 'GPS.Declination',
			'OBD.Obd_ID', 'OBD.CalculatedEngineLoad',
			'OBD.EngineCoolantTemperature', 'OBD.EngineRPM',
			'OBD.VehicleSpeed', 'OBD.ThrottlePosition', 'NextMeasurement' ],
});
