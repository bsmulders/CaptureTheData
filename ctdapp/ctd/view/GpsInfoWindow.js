/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

var gpsInfoTpl = ''
	+'<tpl for=".">'
	+'	<div class="infowindow gpsinfo">'
	+'		<p>{Time}<p>'
	+'		<dl>'
	+'		<dt>UTC:</dt><dd>{GPS.UTC}</dd>'
	+'		<dt>Fix:</dt><dd>{GPS.Fix}</dd>'
	+'		<dt>Lat:</dt><dd>{GPS.Latitude}</dd>'
	+'		<dt>Long:</dt><dd>{GPS.Longitude}</dd>'
	+'		<dt>Speed:</dt><dd>{GPS.Speed} km/h</dd>'
	+'		<dt>Direction:</dt><dd>{GPS.Direction}&#176;</dd>'
	+'		<dt>Declination:</dt><dd>{GPS.Declination}&#176;</dd>'
	+'		</dl>'
	+'	</div>'
	+'</tpl>';

Ext.define('CTD.view.GpsInfoWindow', {
	extend : 'CTD.view.WidgetWindow',
	alias : 'widget.gpsinfowindow',
	requires : 'CTD.store.Measurements',
	resizable : false,
	layout : 'fit',
	title : 'GPS Info',
	items : [ {
		xtype : 'dataview',
		store : 'Measurements',
		tpl : gpsInfoTpl,
		itemSelector : 'div.timeview',
		loadMask : false,
	} ]
});