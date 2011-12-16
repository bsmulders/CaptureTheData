/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

var lat, lng;
Ext.define('CTD.controller.Widget', {
	extend : 'Ext.app.Controller',
	requires : [ 'CTD.store.Trips', 'CTD.store.GpsReports',
			'CTD.view.GsmSignalWindow', 'CTD.view.TimeControl',
			'CTD.view.GsmSignalWindow', 'CTD.view.GpsSpeedWindow',
			'CTD.view.GpsInfoWindow', 'CTD.view.GmapsWindow' ],
	stores : [ 'Trips', 'GpsReports' ],
	views : [ 'GsmSignalWindow' ],

	init : function() {
		this.control({
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
		});

		this.getGpsReportsStore().addListener('datachanged',
				this.onGpsReportsChange, this);
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

	onGpsReportsChange : function() {
		var gpsRecord = this.getGpsReportsStore().first();

		if (gpsRecord.get('Latitude') != lat
				|| gpsRecord.get('Longitude') != lng) {
			lat = gpsRecord.get('Latitude');
			lng = gpsRecord.get('Longitude');

			var query = Ext.ComponentQuery.query('.gmappanel');
			for ( var i in query) {
				var item = query[i];
				item.addMarker(new google.maps.LatLng(lat, lng),
						new google.maps.Marker(lat, lng), true, true, null);
			}
		}
	},
});