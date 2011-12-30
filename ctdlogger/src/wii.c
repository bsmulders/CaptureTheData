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
#include <wiiuse.h>

int log_wii(char * device, char * database, int tripid) {
	int retval;
	sqlite3 *handle;
	wiimote** wiimotes;
	int found, connected;

	// Open database connection
	retval = sqlite3_open(database, &handle);
	if (retval) {
		printf("Wii Reading: Database connection failed: %d\n", retval);
		return -1;
	}

	// Connect to wiimote
	wiimotes = wiiuse_init(1);

	found = wiiuse_find(wiimotes, 1, 5);
	if (!found) {
		printf("Wii Reading: No wiimote found\n");
		return -1;
	}

	connected = wiiuse_connect(wiimotes, 1);
	if (!connected) {
		printf("Wii Reading: Failed to connect to wiimote\n");
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

	// Destroy the evidence!
	sqlite3_close(handle);
	wiiuse_cleanup(wiimotes, 1);

	return 0;
}

int parse_wii(char * database, int tripid) {
	return 0;
}

int generate_wii_report(char * database, int tripid) {
	return 0;
}
