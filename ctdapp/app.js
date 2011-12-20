/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.require('Ext.chart.*');

Ext
		.application({
			name : 'CTD',
			appFolder : 'ctd',
			autoCreateViewport : true,
			controllers : [ 'Trip', 'TimeControl', 'Widget' ],
			models : [ 'GpsReport', 'GsmReport', 'ObdReport', 'Trip' ],
			stores : [ 'GpsReports', 'GsmReports', 'ObdReports', 'TimeControl',
					'Trips', 'TripSummaries' ],
			views : [ 'CTD.view.GsmSignalWindow', 'CTD.view.TimeControl',
					'CTD.view.WidgetPicker', 'CTD.view.GpsSpeedWindow',
					'CTD.view.GpsInfoWindow', 'CTD.view.GmapsWindow',
					'CTD.view.ObdCoolantTemperatureWindow',
					'CTD.view.ObdEngineLoadWindow', 'CTD.view.ObdRpmWindow',
					'CTD.view.ObdSpeedWindow', 'CTD.view.ObdThrottleWindow',
					'CTD.view.TripList', 'CTD.view.Viewport',
					'CTD.view.ObdInfoWindow' ],
			launch : function() {
			}
		});