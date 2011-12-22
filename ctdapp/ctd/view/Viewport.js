/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.Viewport', {
	extend : 'Ext.container.Viewport',
	requires : [ 'CTD.view.TripList', 'CTD.view.WidgetPicker' ],
	layout : 'fit',
	items : [ {
		xtype : 'panel',
		bodyStyle : 'background: none !important;',
		layout : {
			type : 'border',
			padding : 0
		},
		border : false,
		items : [ {
			region : 'center',
			bodyStyle : 'background: none !important;',
			border : false,
		}, {
			region : 'south',
			collapsible : true,
			title : 'Settings',
			height : 177,
			layout : {
				type : 'border',
				padding : 0,
			},
			items : [ {
				region : 'center',
				title : 'Widgets',
				autoScroll : true,
				items : [ {
					xtype : 'widgetpicker',
				} ]
			}, {
				region : 'west',
				collapsible : true,
				title : 'Trips',
				minWidth : 790,
				items : [ {
					xtype : 'triplist',
				} ]
			} ]
		}, ]
	} ],
});