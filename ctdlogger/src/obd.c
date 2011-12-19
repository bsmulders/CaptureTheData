/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

#include "obd.h"
#include "ctd.h"
#include "common.h"

#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int log_obd(int tripid) {
	printf("Start OBD logging\n");

	int retval;
	sqlite3 *handle;
	FILE *file;

	// PIDs to send to the OBD unit
	char pids[5][7] = {"0104\r\n","0105\r\n","010C\r\n","010D\r\n","0111\r\n"};
	char pids_count = 5;

	// Open database connection
	retval = sqlite3_open(DATABASE_PATH,&handle);
	if(retval) {
		printf("OBD Reading: Database connection failed: %d\n", retval);
		return -1;
	}

	// Open serial connection
	file = fopen(OBD_PORT, "r+");
	char line[128];
	if(file == NULL) {
		printf("OBD Reading: Serial connection failed: %s\n", OBD_PORT);
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
			sprintf(query, "INSERT INTO ObdData ( 'Trip_ID', 'TimeStamp', 'RawData' ) VALUES ( %d, %f, '%s')", tripid, timestamp, line);
			do {
				retval = sqlite3_exec(handle,query,0,0,0);
			} while (retval == 5);
		}
	}

	// Destroy the evidence!
	fclose(file);
	sqlite3_close(handle);
	return 0;
}

int parse_obd(int tripid) {
	return -1;
}

int generate_obd_report(int tripid) {
	return -1;
}
