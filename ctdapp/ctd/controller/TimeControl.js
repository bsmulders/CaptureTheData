/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.controller.TimeControl', {
	extend : 'Ext.app.Controller',
	stores : [ 'TimeControl' ],

	init : function() {
		this.getTimeControlStore().addListener('update',
				this.onTimeControlUpdate, this);
	},

	onTimeControlUpdate : function() {
		var timeRecord = this.getTimeControlStore().first();

		var query = Ext.ComponentQuery.query('.timecontrol');
		for ( var i in query) {
			var item = query[i];
			var playButton = item.items.items[0];
			var speedButton = item.items.items[1];
			var slider = item.items.items[2];
			var status = item.dockedItems.items[1].items.items[0];

			playButton.toggle(timeRecord.get('Running'), true);
			speedButton.toggle(timeRecord.get('Speed'), true);
			slider.setMinValue(timeRecord.get('StartTimeStamp'));
			slider.setMaxValue(timeRecord.get('EndTimeStamp'));
			slider.setValue(timeRecord.get('TimeStamp'), true);
			status.setText(new Date(timeRecord.get('TimeStamp') * 1000)
					.toUTCString());

			if (!playButton.hasListener('toggle'))
				playButton.addListener('toggle', this.onPlayToggle, this);
			if (!speedButton.hasListener('toggle'))
				speedButton.addListener('toggle', this.onSpeedToggle, this);
			if (!slider.hasListener('change'))
				slider.addListener('change', this.onSliderChange, this);
		}
	},

	onPlayToggle : function(button, pressed) {
		this.getTimeControlStore().first().set('Running', pressed);
	},

	onSpeedToggle : function(button, pressed) {
		this.getTimeControlStore().first().set('Speed', pressed);
	},

	onSliderChange : function(slider, newValue) {
		this.getTimeControlStore().first().set('TimeStamp', newValue);
	}
});