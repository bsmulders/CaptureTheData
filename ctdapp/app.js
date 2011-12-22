/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.require('Ext.chart.*');

Ext.application({
	name : 'CTD',
	appFolder : 'ctd',
	autoCreateViewport : true,
	controllers : [ 'Trip', 'TimeControl', 'Widget' ],
	models : [ 'Measurement', 'Trip' ],
	stores : [ 'Measurements', 'TimeControl', 'Trips', 'TripSummaries' ],
	views : [ 'CTD.view.GsmSignalWindow', 'CTD.view.TimeControl',
			'CTD.view.WidgetPicker', 'CTD.view.GpsSpeedWindow',
			'CTD.view.GpsInfoWindow', 'CTD.view.GmapsWindow',
			'CTD.view.ObdEngineCoolantTemperatureWindow',
			'CTD.view.ObdCalculatedEngineLoadWindow',
			'CTD.view.ObdEngineRPMWindow', 'CTD.view.ObdVehicleSpeedWindow',
			'CTD.view.ObdThrottlePositionWindow', 'CTD.view.TripList',
			'CTD.view.Viewport', 'CTD.view.ObdInfoWindow' ],
	launch : function() {
		if (!self.document.location.hash) {
			var tc = Ext.create('CTD.view.TimeControl');
			var gmaps = Ext.create('CTD.view.GmapsWindow');
			var speed = Ext.create('CTD.view.GpsSpeedWindow');
			var rpm = Ext.create('CTD.view.ObdEngineRPMWindow');
			var duration = 500;
		
			gmaps.show();
			tc.show();
			speed.show();
			rpm.show();
			gmaps.setSize(450, 375);

			tc.setPosition(10, 10, { duration: duration });
			speed.setPosition(10, 100, { duration: duration });
			gmaps.setPosition(250, 10, { duration: duration });
			rpm.setPosition(10, 245, { duration: duration });
		}		
	}
});