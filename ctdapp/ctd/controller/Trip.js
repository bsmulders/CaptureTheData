/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

var lat, lng;

Ext.define('CTD.controller.Trip', {
	extend : 'Ext.app.Controller',
	stores : [ 'Trips', 'TimeControl', 'GpsReports', 'GsmReports' ],
	controllers : [ 'TimeControl' ],

	init : function() {
		this.control({
			'triplist' : {
				itemclick : this.onTripClick
			},
		});
		this.getTripsStore().addListener('load', this.onTripLoad, this);
	},

	onTripClick : function(dataview, record, item, index, e) {
		this.getTripsStore().proxy.url = record.data.URI;
		this.getTripsStore().load();
	},

	onTripLoad : function() {
		var timeRecord = this.getTimeControlStore().first();
		var tripRecord = this.getTripsStore().first();

		if (tripRecord.data) {
			timeRecord.set('StartTimeStamp', tripRecord.get('StartTimeStamp'));
			timeRecord.set('EndTimeStamp', tripRecord.get('EndTimeStamp'));
			timeRecord.set('TimeStamp', tripRecord.get('StartTimeStamp'));
			timeRecord.set('Running', true);
			timeRecord.set('Speed', false);

			var task = {
				run : this.onTick,
				interval : 100,
				scope : this,
			};

			var runner = new Ext.util.TaskRunner();
			runner.start(task);
		}
	},

	onTick : function() {
		var timeRecord = this.getTimeControlStore().first();
		var tripRecord = this.getTripsStore().first();

		if (timeRecord.get('Running') == true && tripRecord.data) {
			if (timeRecord.get('TimeStampSub') >= 9) {
				timeRecord.set('TimeStamp', timeRecord.get('TimeStamp') + 1);
				timeRecord.set('TimeStampSub', 0);

				if (timeRecord.get('TimeStamp') >= tripRecord
						.get('EndTimeStamp')) {
					timeRecord.set('Running', false);
				}
			} else {
				timeRecord.set('TimeStampSub', timeRecord.get('TimeStampSub')
						+ (timeRecord.get('Speed') ? 9 : 1));
			}

			this.getGsmReportsStore().getProxy().url = tripRecord
					.get('Sensors').gsm.URI
					+ "/"
					+ timeRecord.get('TimeStamp')
					+ "."
					+ timeRecord.get('TimeStampSub');
			this.getGsmReportsStore().load();

			this.getGpsReportsStore().getProxy().url = tripRecord
					.get('Sensors').gps.URI
					+ "/"
					+ timeRecord.get('TimeStamp')
					+ "."
					+ timeRecord.get('TimeStampSub');
			this.getGpsReportsStore().load();
		}
	},
});