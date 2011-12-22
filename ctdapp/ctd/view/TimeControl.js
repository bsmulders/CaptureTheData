/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.TimeControl', {
	extend : 'CTD.view.WidgetWindow',
	alias : 'widget.timecontrol',
	height : 85,
	minHeight : 85,
	resizable : false,
	title : 'Time controller',
	layout : 'absolute',
	bbar : [ {
		xtype : 'label',
		x : 5,
		y : 35,
		text : 'Ready',
		style : 'color : #FFF',
	} ],
	items : [ {
		xtype : 'button',
		x : 5,
		y : 6,
		icon : 'resources/images/play.gif',
		enableToggle : true,
	}, {
		xtype : 'button',
		x : 30,
		y : 6,
		icon : 'resources/images/forward.gif',
		enableToggle : true,
	}, {
		xtype : 'sliderfield',
		x : 65,
		y : 9,
		width : 150,
		useTips : false,
	} ],
});