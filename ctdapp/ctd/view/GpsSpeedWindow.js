/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.GpsSpeedWindow', {
	extend : 'CTD.view.WidgetWindow',
	alias : 'widget.gpsspeedwindow',
	requires : 'CTD.store.Measurements',
	title : 'GPS Speed',
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
			maximum : 160,
			steps : 8,
			margin : -7,
			title : 'km/h',
		} ],
		series : [ {
			type : 'gauge',
			field : 'GPS.Speed',
			donut : 72,
			colorSet : [ '#F90', '#ddd' ],
		} ]
	} ]
});