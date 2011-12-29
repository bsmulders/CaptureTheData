/*

 This file is part of CaptureTheData!

 Copyright (c) 2011, Bobbie Smulders

 Contact:  mail@bsmulders.com

 GNU General Public License Usage
 This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

#include "obd.h"
#include "common.h"

#include <stddef.h>
#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int log_obd(char * device, char * database, int tripid) {
	int retval;
	sqlite3 *handle;
	FILE *file;

	// PIDs to send to the OBD unit
	char pids[5][7];
	sprintf(pids[0], "01%02x\r\n", OBD_CALCULATED_ENGINE_LOAD);
	sprintf(pids[1], "01%02x\r\n", OBD_ENGINE_COOLANT_TEMPERATURE);
	sprintf(pids[2], "01%02x\r\n", OBD_ENGINE_RPM);
	sprintf(pids[3], "01%02x\r\n", OBD_VEHICLE_SPEED);
	sprintf(pids[4], "01%02x\r\n", OBD_THROTTLE_POSITION);
	int pids_count = 5;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("OBD Reading: Database connection failed: %d\n", retval);
		return -1;
	}

	// Open serial connection
	file = fopen(device, "r+");
	char line[128];
	if (file == NULL) {
		printf("OBD Reading: Serial connection failed: %s\n", device);
		return -1;
	}

	// Gather serial data
	while (1) {
		for (int i = 0; i < pids_count; i++) {
			fputs(pids[i], file);

			int i = 0;
			while (i < sizeof line) {
				int chr = fgetc(file);

				if (chr == '>')
					break;
				else if (chr != 13)
					line[i++] = chr;
			}
			line[i] = '\0';

			// Insert serial data in database
			char query[200];
			double timestamp = current_time_with_ms();
			sprintf(
					query,
					"INSERT INTO ObdData ( 'Trip_ID', 'TimeStamp', 'RawData' ) VALUES ( %d, %f, '%s')",
					tripid, timestamp, line);
			do {
				retval = sqlite3_exec(handle, query, 0, 0, 0);
			} while (retval == SQLITE_BUSY);
		}
	}

	// Destroy the evidence!
	fclose(file);
	sqlite3_close(handle);
	return 0;
}

int parse_obd(char * database, int tripid) {
	int retval;
	sqlite3 *handle;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("OBD Parsing: Database connection failed: %d\n", retval);
		return -1;
	}

	// Select data from database
	char query[100];
	sprintf(query, "SELECT Obd_ID, RawData FROM ObdData WHERE Trip_ID = %d",
			tripid);
	sqlite3_stmt *stmt;
	retval = sqlite3_prepare_v2(handle, query, -1, &stmt, 0);
	if (retval) {
		printf("OBD Parsing: Selecting data from DB Failed: %d\n", retval);
		printf("Query: %s\n", query);
		return -1;
	}

	// Begin SQL transaction
	sqlite3_exec(handle, "BEGIN TRANSACTION", 0, 0, 0);

	// Step through SELECT query
	while (1) {
		retval = sqlite3_step(stmt);

		if (retval == SQLITE_ROW) {
			// Gather row from database
			int obdid = (int) sqlite3_column_int(stmt, 0);
			char *rawdata = (char*) sqlite3_column_text(stmt, 1);

			char * moderaw = (char *) malloc(3);
			strncpy(moderaw, rawdata, 2);
			int mode = xstrtoi(moderaw);

			char * pidraw = (char *) malloc(3);
			strncpy(pidraw, rawdata + 2, 2);
			int pid = xstrtoi(pidraw);

			char * avalraw = (char *) malloc(3);
			strncpy(avalraw, rawdata + 10, 2);
			int aval = xstrtoi(avalraw);

			char * bvalraw = (char *) malloc(3);
			strncpy(bvalraw, rawdata + 7, 2);
			int bval = xstrtoi(bvalraw);

			char * cvalraw = (char *) malloc(3);
			strncpy(cvalraw, rawdata + 4, 2);
			int cval = xstrtoi(cvalraw);

			char * dvalraw = (char *) malloc(3);
			if (strlen(rawdata) > 13)
				strncpy(dvalraw, rawdata + 13, 2);
			else
				sprintf(dvalraw, "00");
			int dval = xstrtoi(dvalraw);

			// Parse row
			int value = calculate_pid_value(pid, aval, bval, cval, dval);

			// Update row with new data
			char query[200];
			sprintf(
					query,
					"UPDATE ObdData SET Mode = '%d', PID = '%d', Value = '%d' WHERE Obd_ID = %d",
					mode, pid, value, obdid);
			retval = sqlite3_exec(handle, query, 0, 0, 0);
			if (retval) {
				printf("OBD Parsing: Updating data in DB Failed: %d\n", retval);
				return -1;
			}
		} else if (retval == SQLITE_DONE) {
			break;
		} else {
			printf("OBD Parsing: SQL error whilst reading rows: %d\n", retval);
			return -1;
		}
	}

	// End SQL transaction
	sqlite3_exec(handle, "END TRANSACTION", 0, 0, 0);

	// Destroy the evidence!
	sqlite3_close(handle);
	return 0;
}

int generate_obd_report(char * database, int tripid) {
	int retval;
	sqlite3 *handle;
	sqlite3_stmt *stmt;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("OBD Report: Database connection failed: %d\n", retval);
		return -1;
	}
	sqlite3_exec(handle, "PRAGMA synchronous = NORMAL", 0, 0, 0);
	sqlite3_exec(handle, "PRAGMA journal_mode = MEMORY", 0, 0, 0);

	// Remove old reports
	char removequery[100];
	sprintf(removequery, "DELETE FROM ObdReport WHERE Trip_ID = %d", tripid);
	retval = sqlite3_exec(handle, removequery, 0, 0, 0);
	if (retval) {
		printf("OBD Report: Removing data from DB Failed: %d\n", retval);
		printf("Query: %s\n", removequery);
		return -1;
	}

	// Get starttime / endtime
	int starttime, endtime;
	char selectquery[70];
	sprintf(
			selectquery,
			"SELECT MIN(Timestamp), MAX(Timestamp) FROM ObdData WHERE Trip_ID = %d",
			tripid);
	retval = sqlite3_prepare_v2(handle, selectquery, -1, &stmt, 0);
	if (retval) {
		printf("OBD Report: Selecting trip from DB Failed: %d\n", retval);
		return -1;
	}
	retval = sqlite3_step(stmt);
	if (retval != SQLITE_ROW) {
		printf("OBD Report: SQL error whilst reading rows: %d\n", retval);
		return -1;
	}
	starttime = (int) sqlite3_column_double(stmt, 0);
	endtime = (int) sqlite3_column_double(stmt, 1);
	sqlite3_step(stmt);

	// Begin SQL transaction
	sqlite3_exec(handle, "BEGIN TRANSACTION", 0, 0, 0);

	// Create 10 subrecords for each second between the starttime and endtime
	char insertquery[1000];
	for (int second = starttime; second < endtime; second++) {
		for (int subsecond = 0; subsecond < 10; subsecond++) {
			// Insert closest measurement in database
			sprintf(
					insertquery,
					"INSERT INTO ObdReport ( 'Trip_ID', 'TimeStamp', 'TimeStampSub', 'CalculatedEngineLoad', 'EngineCoolantTemperature', 'EngineRPM', 'VehicleSpeed', 'ThrottlePosition' )"
							" SELECT %3$d, %1$d, %2$d, "
							" (SELECT Value FROM ObdData"
							" WHERE Trip_ID = %3$d AND PID = %4$d AND TimeStamp <= %1$d.%2$d AND TimeStamp > %1$d.%2$d - 10 ORDER BY TimeStamp DESC LIMIT 1),"
							" (SELECT Value FROM ObdData"
							" WHERE Trip_ID = %3$d AND PID = %5$d AND TimeStamp <= %1$d.%2$d AND TimeStamp > %1$d.%2$d - 10 ORDER BY TimeStamp DESC LIMIT 1),"
							" (SELECT Value FROM ObdData"
							" WHERE Trip_ID = %3$d AND PID = %6$d AND TimeStamp <= %1$d.%2$d AND TimeStamp > %1$d.%2$d - 10 ORDER BY TimeStamp DESC LIMIT 1),"
							" (SELECT Value FROM ObdData"
							" WHERE Trip_ID = %3$d AND PID = %7$d AND TimeStamp <= %1$d.%2$d AND TimeStamp > %1$d.%2$d - 10 ORDER BY TimeStamp DESC LIMIT 1),"
							" (SELECT Value FROM ObdData"
							" WHERE Trip_ID = %3$d AND PID = %8$d AND TimeStamp <= %1$d.%2$d AND TimeStamp > %1$d.%2$d - 10 ORDER BY TimeStamp DESC LIMIT 1)"
							, second, subsecond, tripid,
					OBD_CALCULATED_ENGINE_LOAD, OBD_ENGINE_COOLANT_TEMPERATURE,
					OBD_ENGINE_RPM, OBD_VEHICLE_SPEED, OBD_THROTTLE_POSITION);
			retval = sqlite3_exec(handle, insertquery, 0, 0, 0);
			if (retval) {
				printf("OBD Report: Inserting data in DB Failed: %d\n", retval);
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

int calculate_pid_value(int pid, int a, int b, int c, int d) {
	switch (pid) {
	case OBD_CALCULATED_ENGINE_LOAD:
		return (int) ((a * 100) / 255.0);
	case OBD_ENGINE_COOLANT_TEMPERATURE:
		return a - 40;
	case OBD_ENGINE_RPM:
		return (int) (((a * 256) + b) / 4.0);
	case OBD_VEHICLE_SPEED:
		return a;
	case OBD_THROTTLE_POSITION:
		return (int) ((a * 100) / 255.0);
	}
}
