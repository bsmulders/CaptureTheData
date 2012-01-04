/*

 This file is part of CaptureTheData!

 Copyright (c) 2011, Bobbie Smulders

 Contact:  mail@bsmulders.com

 GNU General Public License Usage
 This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

#include "wii.h"
#include "common.h"

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiiuse.h>

sqlite3 *handle;
wiimote** wiimotes;

void wii_sighandler(int signum) {
	sqlite3_close(handle);
	wiiuse_cleanup(wiimotes, 1);
	exit(-1);
}

int log_wii(char * device, char * database, int tripid) {
	signal(SIGINT, (void*) wii_sighandler);
	int retval;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("Wii Reading: Database connection failed: %d\n", retval);
		return -1;
	}

	// Connect to wiimote
	int found, connected;
	wiimotes = wiiuse_init(1);

	if (!wiiuse_find(wiimotes, 1, 5)) {
		printf("Wii Reading: Wiimote not found: %s\n", device);
		return -1;
	}

	if (!wiiuse_connect(wiimotes, 1)) {
		printf("Wii Reading: Wiimote connection failed: %s\n", device);
		return -1;
	}

	wiimote* wiimote = wiimotes[0];
	wiiuse_motion_sensing(wiimote, 1);

	// Gather wii data
	while (1) {
		if (wiiuse_poll(wiimotes, 1)) {
			if (wiimote->event == WIIUSE_EVENT && WIIUSE_USING_ACC(wiimotes[0])) {
				//Insert serial data in database
				char query[250];
				double timestamp = current_time_with_ms();
				sprintf(
						query,
						"INSERT INTO WiiData ('Trip_ID', 'TimeStamp', 'GforceX', 'GForceY', 'GForceZ', 'Roll', 'Pitch', 'Yaw' ) VALUES ( %d, %f, %f, %f, %f, %f, %f, %f )", tripid, timestamp, wiimote->gforce.x, wiimote->gforce.y, wiimote->gforce.z, wiimote->orient.roll, wiimote->orient.pitch, wiimote->orient.roll);
				do {
					retval = sqlite3_exec(handle, query, 0, 0, 0);
				} while (retval == SQLITE_BUSY);
			}
		}
	}

	return 0;
}

int parse_wii(char * database, int tripid) {
	// Wii parsing isn't necessary because Wiiuse already calculates the g-force
	// and orientation during the event polling. In the future, the calculation
	// of derived values such as velocity will be performed in this method
	return 0;
}

int generate_wii_report(char * database, int tripid) {
	int retval;
	sqlite3_stmt *stmt;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("Wii Report: Database connection failed: %d\n", retval);
		return -1;
	}
	sqlite3_exec(handle, "PRAGMA synchronous = NORMAL", 0, 0, 0);
	sqlite3_exec(handle, "PRAGMA journal_mode = MEMORY", 0, 0, 0);

	// Remove old reports
	char removequery[100];
	sprintf(removequery, "DELETE FROM WiiReport WHERE Trip_ID = %d", tripid);
	retval = sqlite3_exec(handle, removequery, 0, 0, 0);
	if (retval) {
		printf("Wii Report: Removing data from DB Failed: %d\n", retval);
		printf("Query: %s\n", removequery);
		return -1;
	}

	// Get starttime / endtime
	int starttime, endtime;
	char selectquery[70];
	sprintf(
			selectquery,
			"SELECT MIN(Timestamp), MAX(Timestamp) FROM WiiData WHERE Trip_ID = %d", tripid);
	retval = sqlite3_prepare_v2(handle, selectquery, -1, &stmt, 0);
	if (retval) {
		printf("Wii Report: Selecting trip from DB Failed: %d\n", retval);
		return -1;
	}
	retval = sqlite3_step(stmt);
	if (retval != SQLITE_ROW) {
		printf("Wii Report: SQL error whilst reading rows: %d\n", retval);
		return -1;
	}
	starttime = (int) sqlite3_column_double(stmt, 0);
	endtime = (int) sqlite3_column_double(stmt, 1);
	sqlite3_step(stmt);

	// Begin SQL transaction
	sqlite3_exec(handle, "BEGIN TRANSACTION", 0, 0, 0);

	// Create 10 subrecords for each second between the starttime and endtime
	char insertquery[450];
	int second, subsecond;
	for (second = starttime; second <= endtime; second++) {
		for (subsecond = 0; subsecond < 10; subsecond++) {
			// Insert closest measurement in database
			sprintf(
					insertquery,
					"INSERT INTO WiiReport ( 'Trip_ID', 'TimeStamp', 'TimeStampSub', 'GforceX', 'GforceY', 'GforceZ', 'Roll', 'Pitch', 'Yaw', 'Speed' )"
					" SELECT Trip_ID, %1$d, %2$d, GforceX, GforceY, GforceZ, Roll, Pitch, Yaw, Speed"
					" FROM WiiData"
					" WHERE Trip_ID = %3$d AND TimeStamp <= %1$d.%2$d AND TimeStamp > %1$d.%2$d - 10 ORDER BY TimeStamp DESC LIMIT 1", second, subsecond, tripid);
			retval = sqlite3_exec(handle, insertquery, 0, 0, 0);
			if (retval) {
				printf("Wii Report: Inserting data in DB Failed: %d\n", retval);
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
