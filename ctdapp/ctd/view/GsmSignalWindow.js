/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.GsmSignalWindow', {
	extend : 'Ext.Window',
	alias : 'widget.gsmsignalwindow',
	requires : 'CTD.store.GsmReports',
	width : 220,
	height : 170,
	minWidth : 220,
	minHeight : 170,
	maximizable : true,
	title : 'GSM Signal Strength',
	layout : {
		type : 'hbox',
		align : 'stretch'
	},
	items : [ {
		xtype : 'chart',
		style : 'background: #fff',
		animate : true,
		store : 'GsmReports',
		insetPadding : 25,
		flex : 1,
		axes : [ {
			type : 'gauge',
			position : 'gauge',
			minimum : 0,
			maximum : 32,
			steps : 8,
			margin : 7,
		} ],
		series : [ {
			type : 'gauge',
			field : 'SignalStrenght',
			donut : 80,
			colorSet : [ '#F90', '#ddd' ]
		} ]
	} ]
});