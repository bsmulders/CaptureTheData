/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

var obdInfoTpl = ''
		+ '<tpl for=".">'
		+ '	<div class="infowindow obdinfo">'
		+ '		<p>{OBD.Time}<p>'
		+ '		<dl>'
		+ '		<dt>Vehicle Speed:</dt><dd>{OBD.VehicleSpeed} km/h</dd>'
		+ '		<dt>Engine RPM:</dt><dd>{OBD.EngineRPM} RPM</dd>'
		+ '		<dt>Throttle Position:</dt><dd>{OBD.ThrottlePosition} %</dd>'
		+ '		<dt>Calculated Engine Load:</dt><dd>{OBD.CalculatedEngineLoad}%</dd>'
		+ '		<dt>Engine Coolant Temperature:</dt><dd>{OBD.EngineCoolantTemperature} &#176;</dd>'
		+ '		</dl>' + '	</div>' + '</tpl>';

Ext.define('CTD.view.ObdInfoWindow', {
	extend : 'CTD.view.WidgetWindow',
	alias : 'widget.obdinfowindow',
	requires : 'CTD.store.Measurements',
	resizable : false,
	layout : 'fit',
	title : 'OBD Info',
	items : [ {
		xtype : 'dataview',
		store : 'Measurements',
		tpl : obdInfoTpl,
		itemSelector : 'div.timeview',
		loadMask : false,
	} ]
});