/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.view.TripList', {
	extend : 'Ext.grid.Panel',
	alias : 'widget.triplist',
	requires : 'CTD.store.TripSummaries',
	store : 'TripSummaries',
	height : '100%',
	columns : [ {
		header : 'ID',
		dataIndex : 'Trip_ID',
		width : 30,
	}, {
		header : 'Description',
		dataIndex : 'Name',
		field : {
			xtype : 'textfield',
		},
		width : 215,
	}, {
		header : 'Start time',
		dataIndex : 'FirstReport',
		renderer : function(date) {
			return new Date(date * 1000).toLocaleString();
		},
		width : 210,
	}, {
		header : 'End time',
		dataIndex : 'LastReport',
		renderer : function(date) {
			return new Date(date * 1000).toLocaleString();
		},
		width : 210,
	}, {
		header : 'Distance',
		dataIndex : 'CalculatedKilometers',
		renderer : function(kmh) {
			return (kmh == null) ? 0 : kmh.toFixed(2) + " km";
		},
		width : 60,
	}, {
		header : 'Fuel',
		dataIndex : 'CalculatedFuel',
		width : 60,
	}, ],
	plugins : [ Ext.create('Ext.grid.plugin.RowEditing') ]
});