/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.GpsSpeedWindow', {
	extend : 'Ext.Window',
	alias : 'widget.gpsspeedwindow',
	requires : 'CTD.store.GpsReports',
	width : 240,
	height : 170,
	minWidth : 220,
	minHeight : 170,
	maximizable : true,
	title : 'GPS Speed',
	layout : {
		type : 'hbox',
		align : 'stretch'
	},
	items : [ {
		xtype : 'chart',
		style : 'background:#fff',
		animate : {
            duration: 300,
            easing: 'ease',
        },
		store : 'GpsReports',
		insetPadding : 25,
		flex : 1,
		axes : [ {
			type : 'gauge',
			position : 'gauge',
			minimum : 0,
			maximum : 160,
			steps : 8,
			margin : 7,
		} ],
		series : [ {
			type : 'gauge',
			field : 'Speed',
			donut : 80,
			colorSet : [ '#F90', '#ddd' ],
		} ]
	} ]
});