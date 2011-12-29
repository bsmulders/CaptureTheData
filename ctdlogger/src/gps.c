/*

 This file is part of CaptureTheData!

 Copyright (c) 2011, Bobbie Smulders

 Contact:  mail@bsmulders.com

 GNU General Public License Usage
 This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

#include "gps.h"
#include "common.h"

#include <stddef.h>
#include <sqlite3.h>
#include <stdio.h>
#include <nmea/nmea.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef NMEA_WIN
#   include <io.h>
#endif

int log_gps(char * device, char * database, int tripid) {
	int retval;
	sqlite3 *handle;
	FILE *file;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("GPS Reading: Database connection failed: %d\n", retval);
		return -1;
	}

	// Open serial connection
	file = fopen(device, "r");
	char line[128];
	if (file == NULL) {
		printf("GPS Reading: Serial connection failed: %s\n", device);
		return -1;
	}

	// Gather serial data
	while (1) {
		fgets(line, sizeof line, file);

		// Insert serial data in database
		char query[200];
		double timestamp = current_time_with_ms();
		sprintf(
				query,
				"INSERT INTO GpsData ( 'Trip_ID', 'TimeStamp', 'RawData' ) VALUES ( %d, %f, '%s')", tripid, timestamp, line);
		do {
			retval = sqlite3_exec(handle, query, 0, 0, 0);
		} while (retval == SQLITE_BUSY);
	}

	// Destroy the evidence!
	sqlite3_close(handle);
	return 0;
}

int parse_gps(char * database, int tripid) {
	int retval;
	sqlite3 *handle;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("GPS Parsing: Database connection failed: %d\n", retval);
		return -1;
	}
	sqlite3_exec(handle, "PRAGMA synchronous = NORMAL", 0, 0, 0);
	sqlite3_exec(handle, "PRAGMA journal_mode = MEMORY", 0, 0, 0);

	// Select data from database
	char selectquery[100];
	sprintf(selectquery,
			"SELECT Gps_ID, RawData FROM GpsData WHERE Trip_ID = %d", tripid);
	sqlite3_stmt *stmt;
	retval = sqlite3_prepare_v2(handle, selectquery, -1, &stmt, 0);
	if (retval) {
		printf("GPS Parsing: Selecting data from DB Failed: %d\n", retval);
		printf("Query: %s\n", selectquery);
		return -1;
	}

	// Initialize NMEA parser
	nmeaINFO info;
	nmeaPARSER parser;
	nmea_zero_INFO(&info);
	nmea_parser_init(&parser);

	// Begin SQL transaction
	sqlite3_exec(handle, "BEGIN TRANSACTION", 0, 0, 0);

	// Step through SELECT query
	while (1) {
		retval = sqlite3_step(stmt);

		if (retval == SQLITE_ROW) {
			// Gather row from database
			int gpsid = (int) sqlite3_column_int(stmt, 0);
			char *rawdata = (char*) sqlite3_column_text(stmt, 1);
			int len = (int) strlen(rawdata);

			// Parse NMEA data
			nmea_parse(&parser, rawdata, len, &info);

			// Convert time format to C_time
			struct tm converttime;
			converttime.tm_year = info.utc.year;
			converttime.tm_mon = info.utc.mon;
			converttime.tm_mday = info.utc.day;
			converttime.tm_hour = info.utc.hour;
			converttime.tm_min = info.utc.min;
			converttime.tm_sec = info.utc.sec;

			// Update row with NMEA data
			char query[200];
			sprintf(
					query,
					"UPDATE GpsData SET UTC = %f, Fix = %d, Latitude = %f, Longitude = %f, Speed = %f, Direction = %f, Declination = %f WHERE Gps_ID = %d", (float) mktime(&converttime), info.fix, nmea_ndeg2degree(info.lat), nmea_ndeg2degree(info.lon), info.speed, info.direction, info.declination, gpsid);
			retval = sqlite3_exec(handle, query, 0, 0, 0);
			if (retval) {
				printf("GPS Parsing: Updating data in DB Failed: %d\n", retval);
				return -1;
			}
		} else if (retval == SQLITE_DONE) {
			break;
		} else {
			printf("GPS Parsing: SQL error whilst reading rows: %d\n", retval);
			return -1;
		}
	}

	// End SQL transaction
	sqlite3_exec(handle, "END TRANSACTION", 0, 0, 0);

	// Remove unnecesary data
	char removequery[100];
	sprintf(removequery,
			"DELETE FROM GpsData WHERE RawData	NOT LIKE '$GPRMC%%'");
	retval = sqlite3_exec(handle, removequery, 0, 0, 0);
	if (retval) {
		printf("GPS Parsing: Removing data from DB Failed: %d\n", retval);
		printf("Query: %s\n", removequery);
		return -1;
	}

	// Destroy the evidence!
	nmea_parser_destroy(&parser);
	sqlite3_close(handle);
	return 0;
}

int generate_gps_report(char * database, int tripid) {
	int retval;
	sqlite3 *handle;
	sqlite3_stmt *stmt;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("GPS Report: Database connection failed: %d\n", retval);
		return -1;
	}
	sqlite3_exec(handle, "PRAGMA synchronous = NORMAL", 0, 0, 0);
	sqlite3_exec(handle, "PRAGMA journal_mode = MEMORY", 0, 0, 0);

	// Remove old reports
	char removequery[100];
	sprintf(removequery, "DELETE FROM GpsReport WHERE Trip_ID = %d", tripid);
	retval = sqlite3_exec(handle, removequery, 0, 0, 0);
	if (retval) {
		printf("GPS Report: Removing data from DB Failed: %d\n", retval);
		printf("Query: %s\n", removequery);
		return -1;
	}

	// Get starttime / endtime
	int starttime, endtime;
	char selectquery[70];
	sprintf(
			selectquery,
			"SELECT MIN(Timestamp), MAX(Timestamp) FROM GpsData WHERE Trip_ID = %d", tripid);
	retval = sqlite3_prepare_v2(handle, selectquery, -1, &stmt, 0);
	if (retval) {
		printf("GPS Report: Selecting trip from DB Failed: %d\n", retval);
		return -1;
	}
	retval = sqlite3_step(stmt);
	if (retval != SQLITE_ROW) {
		printf("GPS Report: SQL error whilst reading rows: %d\n", retval);
		return -1;
	}
	starttime = (int) sqlite3_column_double(stmt, 0);
	endtime = (int) sqlite3_column_double(stmt, 1);
	sqlite3_step(stmt);

	// Begin SQL transaction
	sqlite3_exec(handle, "BEGIN TRANSACTION", 0, 0, 0);

	// Create 10 subrecords for each second between the starttime and endtime
	char insertquery[450];
	for (int second = starttime; second < endtime; second++) {
		for (int subsecond = 0; subsecond < 10; subsecond++) {
			// Insert closest measurement in database
			sprintf(
					insertquery,
					"INSERT INTO GpsReport ( 'Trip_ID', 'TimeStamp', 'TimeStampSub', 'UTC', 'Fix', 'Latitude', 'Longitude', 'Speed', 'Direction', 'Declination' )"
					" SELECT Trip_ID, %1$d, %2$d, UTC, Fix, Latitude, Longitude, Speed, Direction, Declination"
					" FROM GpsData"
					" WHERE Trip_ID = %3$d AND TimeStamp <= %1$d.%2$d AND TimeStamp > %1$d.%2$d - 10 ORDER BY TimeStamp DESC LIMIT 1", second, subsecond, tripid);
			retval = sqlite3_exec(handle, insertquery, 0, 0, 0);
			if (retval) {
				printf("GPS Report: Inserting data in DB Failed: %d\n", retval);
				printf("Query: %s\n", insertquery);
				return -1;
			}
		}
	}

	// End SQL transaction
	sqlite3_exec(handle, "END TRANSACTION", 0, 0, 0);

	// Destroy the evidence!
	sqlite3_close(handle);
	return 0;
}
