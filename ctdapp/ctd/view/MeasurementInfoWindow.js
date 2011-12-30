/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

var measurementInfoTpl = ''
		+ '<tpl for=".">'
		+ '	<div class="infowindow">'
		+ '		<p>GSM</p>'
		+ '		<dl>'
		+ '		<dt>Signal Strength:</dt><dd>{GSM.SignalStrength}</dd>'
		+ '		</dl>'
		+ '		<p>GPS<p>'
		+ '		<dl>'
		+ '		<dt>UTC:</dt><dd>{GPS.UTC}</dd>'
		+ '		<dt>Fix:</dt><dd>{GPS.Fix}</dd>'
		+ '		<dt>Latitude:</dt><dd>{GPS.Latitude}</dd>'
		+ '		<dt>Longitude:</dt><dd>{GPS.Longitude}</dd>'
		+ '		<dt>Speed:</dt><dd>{GPS.Speed} km/h</dd>'
		+ '		<dt>Direction:</dt><dd>{GPS.Direction}&#176;</dd>'
		+ '		<dt>Declination:</dt><dd>{GPS.Declination}&#176;</dd>'
		+ '		</dl>'
		+ '		<p>OBD</p>'
		+ '		<dl>'
		+ '		<dt>Vehicle Speed:</dt><dd>{OBD.VehicleSpeed} km/h</dd>'
		+ '		<dt>Engine RPM:</dt><dd>{OBD.EngineRPM} RPM</dd>'
		+ '		<dt>Throttle Position:</dt><dd>{OBD.ThrottlePosition} %</dd>'
		+ '		<dt>Calculated Engine Load:</dt><dd>{OBD.CalculatedEngineLoad}%</dd>'
		+ '		<dt>Engine Coolant Temperature:</dt><dd>{OBD.EngineCoolantTemperature} &#176;</dd>'		
		+ '		</dl>'
		+ '		<p>Wii<p>'
		+ '		<dl>'
		+ '		<dt>G-force X:</dt><dd>{Wii.GforceX} g</dd>'
		+ '		<dt>G-force Y:</dt><dd>{Wii.GforceY} g</dd>'
		+ '		<dt>G-force Z:</dt><dd>{Wii.GforceZ} g</dd>'
		+ '		<dt>Roll:</dt><dd>{Wii.Roll} &deg;</dd>'
		+ '		<dt>Pitch:</dt><dd>{Wii.Pitch} &deg;</dd>'
		+ '		<dt>Yaw:</dt><dd>{Wii.Yaw} &deg;</dd>'
		+ '		<dt>Speed:</dt><dd>{Wii.Speed} km/h</dd>'
		+ '		</dl>'
		+ '	</div>' + '</tpl>';

Ext.define('CTD.view.MeasurementInfoWindow', {
	extend : 'CTD.view.WidgetWindow',
	alias : 'widget.measurementinfowindow',
	requires : 'CTD.store.Measurements',
	resizable : false,
	layout : 'fit',
	title : 'Measurement info',
	height : '370',
	items : [ {
		xtype : 'dataview',
		store : 'Measurements',
		tpl : measurementInfoTpl,
		itemSelector : 'div.timeview',
		loadMask : false,
	} ]
});