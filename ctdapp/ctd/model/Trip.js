/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

Ext.define('CTD.model.Trip',
		{
			extend : 'Ext.data.Model',
			fields : [ 'Trip_ID', 'Name', 'FirstData', 'LastData',
					'CalculatedKilometers', 'CalculatedFuel', 'FirstReport',
					'FirstReportSub', 'LastReport', 'LastReportSub', 'URI',
					'Sensors' ],
		});