/*

 This file is part of CaptureTheData!

 Copyright (c) 2011, Bobbie Smulders

 Contact:  mail@bsmulders.com

 GNU General Public License Usage
 This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

#include "trip.h"
#include "gsm.h"
#include "gps.h"

#include <stddef.h>
#include <sqlite3.h>
#include <stdio.h>

int create_trip(char * database) {
	int retval;
	sqlite3 *handle;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("Creating Trip: Database connection failed\n");
		return -1;
	}

	// Create a new trip
	char *createquery = "INSERT INTO Trip DEFAULT VALUES";
	retval = sqlite3_exec(handle, createquery, 0, 0, 0);
	if (retval) {
		printf("Create Trip: Creating trip in DB Failed: %d\n", retval);
		return -1;
	}

	// Get new trip id
	int tripid;
	char *selectquery = "SELECT last_insert_rowid() AS tripid";
	sqlite3_stmt *stmt;
	retval = sqlite3_prepare_v2(handle, selectquery, -1, &stmt, 0);
	if (retval) {
		printf("Create Trip: Selecting trip from DB Failed: %d\n", retval);
		return -1;
	}
	retval = sqlite3_step(stmt);
	if (retval != SQLITE_ROW) {
		printf("Create Trip: SQL error whilst reading rows: %d\n", retval);
		return -1;
	}
	tripid = (int) sqlite3_column_int(stmt, 0);

	// Destroy the evidence!
	sqlite3_close(handle);

	return tripid;
}

int update_trip_data(char * database, int tripid) {
	int retval;
	sqlite3 *handle;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("Update trip data: Database connection failed: %d\n", retval);
		return -1;
	}

	// Update trip columns
	char query[850];
	sprintf(
			query,
			"UPDATE Trip SET"
			" FirstData = (SELECT MIN(TimeStamp) FROM ("
			" SELECT Timestamp FROM GpsData WHERE GpsData.Trip_ID = Trip.Trip_ID"
			" UNION SELECT Timestamp FROM GsmData WHERE GsmData.Trip_ID = Trip.Trip_ID"
			" UNION SELECT Timestamp FROM ObdData WHERE ObdData.Trip_ID = Trip.Trip_ID"
			" UNION SELECT Timestamp FROM WiiData WHERE WiiData.Trip_ID = Trip.Trip_ID"
			" )),"
			" LastData = (SELECT MAX(TimeStamp) FROM ("
			" SELECT Timestamp FROM GpsData WHERE GpsData.Trip_ID = Trip.Trip_ID"
			" UNION SELECT Timestamp FROM GsmData WHERE GsmData.Trip_ID = Trip.Trip_ID"
			" UNION SELECT Timestamp FROM ObdData WHERE ObdData.Trip_ID = Trip.Trip_ID"
			" UNION SELECT Timestamp FROM WiiData WHERE WiiData.Trip_ID = Trip.Trip_ID"
			" )),"
			" CalculatedKilometers = (SELECT SUM(((Speed*1000)/60)/60)/1000 FROM GpsData"
			" WHERE GpsData.Trip_ID = Trip.Trip_ID)"
			" WHERE Trip_ID = %d", tripid);

	retval = sqlite3_exec(handle, query, 0, 0, 0);
	if (retval) {
		printf("Update trip data: Updating trips in DB Failed: %d\n", retval);
		printf("Query: %s\n", query);
		return -1;
	}

	// Destroy the evidence!
	sqlite3_close(handle);
	return 0;
}

int update_trip_report(char * database, int tripid) {
	int retval;
	sqlite3 *handle;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("Update trip report: Database connection failed: %d\n", retval);
		return -1;
	}

	// Update trip columns
	char query[2000];
	sprintf(
			query,
			" UPDATE Trip SET "
			" FirstReport = ( "
			" 	SELECT TimeStamp FROM ( "
			" 		SELECT TimeStamp, TimeStampSub FROM GpsReport WHERE GpsReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM GsmReport WHERE GsmReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM ObdReport WHERE ObdReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM WiiReport WHERE WiiReport.Trip_ID = Trip.Trip_ID "
			" 	) ORDER BY TimeStamp ASC, TimeStampSub ASC LIMIT 1 "
			" ), "
			" FirstReportSub = ( "
			" 	SELECT TimeStampSub FROM ( "
			" 		SELECT TimeStamp, TimeStampSub FROM GpsReport WHERE GpsReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM GsmReport WHERE GsmReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM ObdReport WHERE ObdReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM WiiReport WHERE WiiReport.Trip_ID = Trip.Trip_ID "
			" 	) ORDER BY TimeStamp ASC, TimeStampSub ASC LIMIT 1 "
			" ), "
			" LastReport = ( "
			" 	SELECT TimeStamp FROM ( "
			" 		SELECT TimeStamp, TimeStampSub FROM GpsReport WHERE GpsReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM GsmReport WHERE GsmReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM ObdReport WHERE ObdReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM WiiReport WHERE WiiReport.Trip_ID = Trip.Trip_ID "
			" 	) ORDER BY TimeStamp DESC, TimeStampSub DESC LIMIT 1 "
			" ), "
			" LastReportSub = ( "
			" 	SELECT TimeStampSub FROM ( "
			" 		SELECT TimeStamp, TimeStampSub FROM GpsReport WHERE GpsReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM GsmReport WHERE GsmReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM ObdReport WHERE ObdReport.Trip_ID = Trip.Trip_ID UNION "
			" 		SELECT TimeStamp, TimeStampSub FROM WiiReport WHERE WiiReport.Trip_ID = Trip.Trip_ID "
			" 	) ORDER BY TimeStamp DESC, TimeStampSub DESC LIMIT 1 "
			" ) "
			" WHERE Trip_ID = %d", tripid);

	retval = sqlite3_exec(handle, query, 0, 0, 0);
	if (retval) {
		printf("Update trip report: Updating trips in DB Failed: %d\n", retval);
		printf("Query: %s\n", query);
		return -1;
	}

	// Destroy the evidence!
	sqlite3_close(handle);
	return 0;
}
