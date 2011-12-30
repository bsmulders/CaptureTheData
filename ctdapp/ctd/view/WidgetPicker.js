/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.WidgetPicker', {
	extend : 'Ext.panel.Panel',
	alias : 'widget.widgetpicker',
	defaults : {
		width : 135,
		height : 105,
		margin : 9,
	},
	defaultType : 'button',
	items : [ {
		text : 'Time controller',
		action : 'timeController',
	}, {
		text : 'Measurement info',
		action : 'measurementInfo',
	}, {
		text : 'GSM<hr />Signal Strength',
		action : 'gsmSignalStrength',
	}, {
		text : 'GPS<hr/>Speed',
		action : 'gpsSpeed',
	}, {
		text : 'GPS<hr />Google Maps',
		action : 'gMaps',
	}, {
		text : 'OBD<hr />Vehicle Speed',
		action : 'obdVehicleSpeed',
	}, {
		text : 'OBD<hr />Engine RPM',
		action : 'obdEngineRPM',
	}, {
		text : 'OBD<hr />Throttle Position',
		action : 'obdThrottlePosition',
	}, {
		text : 'OBD<hr />Calculated Engine Load',
		action : 'obdCalculatedEngineLoad',
	}, {
		text : '<br />OBD<hr />Engine Coolant<br />Temperature',
		action : 'obdEngineCoolantTemperature',
	}, {
		text : 'Wii<hr />Test Window',
		action : 'wiiTestWindow',
	} ],
});
