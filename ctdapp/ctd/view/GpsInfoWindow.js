/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

var gpsInfoTpl = ''
	+'<tpl for=".">'
	+'	<div class="timeview">'
	+'		<p>{Time}<p>'
	+'		<dl>'
	+'		<dt>UTC:</dt><dd>{UTC}</dd>'
	+'		<dt>Fix:</dt><dd>{Fix}</dd>'
	+'		<dt>Lat:</dt><dd>{Latitude}</dd>'
	+'		<dt>Long:</dt><dd>{Longitude}</dd>'
	+'		<dt>Speed:</dt><dd>{Speed} km/h</dd>'
	+'		<dt>Direction:</dt><dd>{Direction}&#176;</dd>'
	+'		<dt>Declination:</dt><dd>{Declination}&#176;</dd>'
	+'		</dl>'
	+'	</div>'
	+'</tpl>';

Ext.define('CTD.view.GpsInfoWindow', {
	extend : 'Ext.Window',
	alias : 'widget.gpsinfowindow',
	requires : 'CTD.store.GpsReports',
	width : 250,
	height : 160,
	resizable : false,
	title : 'GPS Info',
	items : [ {
		xtype : 'dataview',
		store : 'GpsReports',
		tpl : gpsInfoTpl,
		itemSelector : 'div.timeview',
		loadMask : false,
	} ]
});