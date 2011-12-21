/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.ObdCoolantTemperatureWindow', {
	extend : 'Ext.Window',
	alias : 'widget.obdcoolanttemperaturewindow',
	requires : 'CTD.store.Measurements',
	width : 220,
	height : 140,
	minWidth : 220,
	minHeight : 140,
	maximizable : true,
	title : 'OBD Coolant Temperature',
	layout : {
		type : 'hbox',
		align : 'stretch'
	},
	items : [ {
		xtype : 'chart',
		style : 'background:#fff',
		animate : {
			duration : 300,
			easing : 'ease',
		},
		store : 'Measurements',
		insetPadding : 5,
		flex : 1,
		axes : [ {
			type : 'gauge',
			position : 'gauge',
			minimum : 0,
			maximum : 215,
			steps : 10,
			margin : -7,
			title : '&#8451;',
		} ],
		series : [ {
			type : 'gauge',
			field : 'OBD.EngineCoolantTemperature',
			donut : 72,
			colorSet : [ '#F90', '#ddd' ],
		} ]
	} ]
});