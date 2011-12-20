/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

var obdInfoTpl = ''
	+'<tpl for=".">'
	+'	<div class="timeview">'
	+'		<p>{Time}<p>'
	+'		<dl>'
	+'		<dt>Speed:</dt><dd>{VehicleSpeed} km/h</dd>'
	+'		<dt>RPM:</dt><dd>{EngineRPM}</dd>'
	+'		<dt>Throttle:</dt><dd>{ThrottlePosition} %</dd>'
	+'		<dt>Engine Load:</dt><dd>{CalculatedEngineLoad}%</dd>'
	+'		<dt>Coolant temp:</dt><dd>{EngineCoolantTemperature} &#176;</dd>'
	+'		</dl>'
	+'	</div>'
	+'</tpl>';

Ext.define('CTD.view.ObdInfoWindow', {
	extend : 'Ext.Window',
	alias : 'widget.obdinfowindow',
	requires : 'CTD.store.ObdReports',
	width : 250,
	height : 120,
	resizable : false,
	title : 'OBD Info',
	items : [ {
		xtype : 'dataview',
		store : 'ObdReports',
		tpl : obdInfoTpl,
		itemSelector : 'div.timeview',
		loadMask : false,
	} ]
});