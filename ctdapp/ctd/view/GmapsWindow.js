/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.GmapsWindow', {
	extend : 'Ext.Window',
	alias : 'widget.gmapswindow',
	layout : 'fit',
	width : 450,
	height : 450,
	title : 'Google Maps',
	maximizable : true,
	items : {
		xtype : 'gmappanel',
		zoomLevel : 2,
		gmapType : 'map',
		mapConfOpts : [ 'enableScrollWheelZoom', 'enableDoubleClickZoom',
				'enableDragging' ],
		mapControls : [ 'GSmallMapControl', 'GMapTypeControl',
				'NonExistantControl' ],
		setCenter : {
			geoCodeAddr : '4 Yawkey Way, Boston, MA, 02215-3409, USA',
		},
		listeners : {
			resize : function(p, w, h) {
				this.onResize(w, h);
			}
		}
	}
});
