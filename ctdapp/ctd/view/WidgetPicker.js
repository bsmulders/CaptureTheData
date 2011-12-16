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
		width : 105,
		height : 105,
		margin : 9,
	},
	defaultType : 'button',
	items : [ {
		text : 'Time controller',
		action : 'timeController',
	}, {
		text : 'GSM<hr />Signal Strength',
		action : 'gsmSignalStrength',
	}, {
		text : 'GPS<hr/>Speed',
		action : 'gpsSpeed',
	}, {
		text : 'GPS<hr/>Info',
		action : 'gpsInfo',
	}, {
		text : 'GPS<hr />Google Maps',
		action : 'gMaps',
	}, {
		text : 'OBD<hr />Speed',
		action : 'obsSpeed',
	}, {
		text : 'OBD<hr />RPM',
		action : 'obdRPM',
	}, {
		text : 'OBD<hr />Throttle',
		action : 'obdThrottle',
	}, {
		text : 'OBD<hr />Engine Load',
		action : 'obdEngineLoad',
	}, {
		text : '<br />OBD<hr />Coolant<br />Temperature',
		action : 'obdCoolantTemperature',
	} ],
});
