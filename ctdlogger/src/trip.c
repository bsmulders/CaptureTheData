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

#include <sqlite3.h>
#include <stdio.h>

int create_trip(char * database) {
	int retval;
	sqlite3 *handle;

	// Open database connection
	retval = sqlite3_open(database,&handle);
	if(retval) {
		printf("Creating Trip: Database connection failed\n");
		return -1;
	}

	// Create a new trip
	char *createquery = "INSERT INTO Trip DEFAULT VALUES";
	retval = sqlite3_exec(handle,createquery,0,0,0);
	if(retval) {
		printf("Create Trip: Creating trip in DB Failed: %d\n", retval);
		return -1;
	}

	// Get new trip id
	int tripid;
	char *selectquery = "SELECT last_insert_rowid() AS tripid";
	sqlite3_stmt *stmt;
	retval = sqlite3_prepare_v2(handle,selectquery,-1,&stmt,0);
	if(retval) {
		printf("Create Trip: Selecting trip from DB Failed: %d\n", retval);
		return -1;
	}
	retval = sqlite3_step(stmt);
	if(retval != SQLITE_ROW) {
		printf("Create Trip: SQL error whilst reading rows: %d\n", retval);
		return -1;
	}
	tripid = (int) sqlite3_column_int(stmt, 0);

	// Destroy the evidence!
	sqlite3_close(handle);

	return tripid;
}

int parse_trip(char * database, int tripid) {
	int retval;
	sqlite3 *handle;

	// Open database connection
	retval = sqlite3_open(database,&handle);
	if(retval) {
		printf("Parse trip: Database connection failed: %d\n", retval);
		return -1;
	}

	// Update trip columns
	char query [500];
	sprintf(query, "UPDATE Trip"
		" SET StartTime = (SELECT Min(Timestamp) FROM GpsData WHERE GpsData.Trip_ID = Trip.Trip_ID GROUP BY Trip_ID),"
		" EndTime = (SELECT Max(Timestamp) FROM GpsData WHERE GpsData.Trip_ID = Trip.Trip_ID GROUP BY Trip_ID),"
		" CalculatedKilometers = (SELECT SUM(((Speed*1000)/60)/60)/1000 FROM GpsData WHERE GpsData.Trip_ID = Trip.Trip_ID GROUP BY Trip_ID)"
		" WHERE Trip_ID = %d", tripid);

	retval = sqlite3_exec(handle,query,0,0,0);
	if(retval) {
		printf("Parse trip: Updating trips in DB Failed: %d\n", retval);
		printf("Query: %s\n", query);
		return -1;
	}

	// Destroy the evidence!
	sqlite3_close(handle);
	return 0;	
}