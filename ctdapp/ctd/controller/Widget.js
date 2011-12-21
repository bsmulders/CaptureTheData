/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

var lat, lng;
Ext
		.define(
				'CTD.controller.Widget',
				{
					extend : 'Ext.app.Controller',
					requires : [ 'CTD.store.Trips', 'CTD.store.Measurements',
							'CTD.view.GsmSignalWindow', 'CTD.view.TimeControl',
							'CTD.view.GsmSignalWindow',
							'CTD.view.GpsSpeedWindow',
							'CTD.view.GpsInfoWindow', 'CTD.view.GmapsWindow',
							'CTD.view.ObdEngineCoolantTemperatureWindow',
							'CTD.view.ObdCalculatedEngineLoadWindow',
							'CTD.view.ObdEngineRPMWindow',
							'CTD.view.ObdVehicleSpeedWindow',
							'CTD.view.ObdThrottlePositionWindow',
							'CTD.view.ObdInfoWindow' ],
					stores : [ 'Trips', 'Measurements' ],
					views : [ 'GsmSignalWindow' ],

					init : function() {
						this
								.control({
									'widgetpicker button[action=timeController]' : {
										click : this.onTimeControlClick
									},
									'widgetpicker button[action=gsmSignalStrength]' : {
										click : this.onGsmSignalStrengthClick
									},
									'widgetpicker button[action=gpsSpeed]' : {
										click : this.onGpsSpeedClick
									},
									'widgetpicker button[action=gpsInfo]' : {
										click : this.onGpsInfoClick
									},
									'widgetpicker button[action=gMaps]' : {
										click : this.onGmapsClick
									},
									'widgetpicker button[action=obdInfo]' : {
										click : this.onObdInfoClick
									},
									'widgetpicker button[action=obdVehicleSpeed]' : {
										click : this.onObdVehicleSpeedClick
									},
									'widgetpicker button[action=obdEngineRPM]' : {
										click : this.onObdEngineRPMClick
									},
									'widgetpicker button[action=obdThrottlePosition]' : {
										click : this.onObdThrottlePositionClick
									},
									'widgetpicker button[action=obdEngineCoolantTemperature]' : {
										click : this.onObdEngineCoolantTemperatureClick
									},
									'widgetpicker button[action=obdCalculatedEngineLoad]' : {
										click : this.onObdCalculatedEngineLoadClick
									},
								});

						this.getMeasurementsStore().addListener('datachanged',
								this.onMeasurementsChange, this);
					},

					onTimeControlClick : function(button, event) {
						Ext.create('CTD.view.TimeControl').show();
					},

					onGsmSignalStrengthClick : function(button, event) {
						Ext.create('CTD.view.GsmSignalWindow').show();
					},

					onGpsSpeedClick : function(button, event) {
						Ext.create('CTD.view.GpsSpeedWindow').show();
					},

					onGpsInfoClick : function(button, event) {
						Ext.create('CTD.view.GpsInfoWindow').show();
					},

					onGmapsClick : function(button, event) {
						Ext.create('CTD.view.GmapsWindow').show();
					},

					onObdInfoClick : function(button, event) {
						Ext.create('CTD.view.ObdInfoWindow').show();
					},

					onObdVehicleSpeedClick : function(button, event) {
						Ext.create('CTD.view.ObdVehicleSpeedWindow').show();
					},

					onObdEngineRPMClick : function(button, event) {
						Ext.create('CTD.view.ObdEngineRPMWindow').show();
					},

					onObdThrottlePositionClick : function(button, event) {
						Ext.create('CTD.view.ObdThrottlePositionWindow').show();
					},

					onObdEngineCoolantTemperatureClick : function(button, event) {
						Ext
								.create(
										'CTD.view.ObdEngineCoolantTemperatureWindow')
								.show();
					},

					onObdCalculatedEngineLoadClick : function(button, event) {
						Ext.create('CTD.view.ObdCalculatedEngineLoadWindow')
								.show();
					},

					onMeasurementsChange : function() {
						var gpsRecord = this.getMeasurementsStore().first();

						if (gpsRecord.get('GPS.Latitude') != lat
								|| gpsRecord.get('GPS.Longitude') != lng) {
							lat = gpsRecord.get('GPS.Latitude');
							lng = gpsRecord.get('GPS.Longitude');

							if (lat != '' && lng != '') {
								var query = Ext.ComponentQuery
										.query('.gmappanel');
								for ( var i in query) {
									var item = query[i];
									item.addMarker(new google.maps.LatLng(lat,
											lng), new google.maps.Marker(lat,
											lng), true, true, null);
								}
							}
						}
					},
				});