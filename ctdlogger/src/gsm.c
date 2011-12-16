/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

#include "gsm.h"
#include "ctd.h"
#include "common.h"

#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int log_gsm(int tripid) {
	printf("Start GSM logging\n");

	int retval;
	sqlite3 *handle;
	FILE *file;

	// AT-Commands to send to the phone
	char atcommands[1][12] = {"AT+CSQ\r\n"};
	char atcommands_count = 1;

	// Open database connection
	retval = sqlite3_open(DATABASE_PATH,&handle);
	if(retval) {
		printf("GSM Reading: Database connection failed: %d\n", retval);
		return -1;
	}

	// Open serial connection
	file = fopen(GSM_PORT, "r+");
	char line[128];
	if(file == NULL) {
		printf("GSM Reading: Serial connection failed: %s\n", GSM_PORT);
		return -1;
	}

	// Disable serial echo
	fputs("ATE=0\r\n", file);
	fgets(line, sizeof line, file);

	// Gather serial data
	while (1) {
		for (int i = 0; i < atcommands_count; i++) {
			fputs(atcommands[i], file);
			usleep(500*1000);
			while (fgetc(file) != '\n');
			fgets(line, sizeof line, file);

			// Insert serial data in database
			char query[200];
			double timestamp = current_time_with_ms();
			sprintf(query, "INSERT INTO GsmData ( 'Trip_ID', 'TimeStamp', 'RawData' ) VALUES ( %d, %f, '%s')", tripid, timestamp, line);
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

int parse_gsm(int tripid) {
	int retval;
	sqlite3 *handle;

	// Open database connection
	retval = sqlite3_open(DATABASE_PATH,&handle);
	if(retval) {
		printf("GSM Parsing: Database connection failed: %d\n", retval);
		return -1;
	}

	// Select data from database
	char query [100];
	sprintf(query, "SELECT Gsm_ID, RawData FROM GsmData WHERE Trip_ID = %d", tripid);
	sqlite3_stmt *stmt;
	retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
	if(retval) {
		printf("GSM Parsing: Selecting data from DB Failed: %d\n", retval);
		printf("Query: %s\n", query);
		return -1;
	}

	while(1) {
		// Step through SQL data
		retval = sqlite3_step(stmt);

		if(retval == SQLITE_ROW) {
			// Gather row from database
			int gsmid = (int) sqlite3_column_int(stmt, 0);
			char *rawdata = (char*) sqlite3_column_text(stmt, 1);
			int len = (int) strlen(rawdata);

			// Parse row
			char * command = (char *) malloc(10);
			char * value = (char * ) malloc(10);
			char * pospoint = strchr(rawdata, ':');
			if (pospoint > 0) {
				int pos = pospoint - rawdata;
				strncpy(command, rawdata, pos);
				strncpy(value, rawdata+pos+2, sizeof(rawdata)-2);
			}

			// Update row with new data
			char query [200];
			sprintf(query, "UPDATE GsmData SET Command = '%s', Value = '%s' WHERE Gsm_ID = %d", command, value, gsmid);
			retval = sqlite3_exec(handle,query,0,0,0);
			if(retval) {
				printf("GSM Parsing: Updating data in DB Failed: %d\n", retval);
				return -1;
			}
		}
		else if(retval == SQLITE_DONE) {
			break;
		}
		else {
			printf("GSM Parsing: SQL error whilst reading rows: %d\n", retval);
			return -1;
		}
	}

	// Destroy the evidence!
	sqlite3_close(handle);
	return 0;
}

int generate_gsm_report(int tripid) {
	int retval;
	sqlite3 *handle;
	sqlite3_stmt *stmt;

	// Open database connection
	retval = sqlite3_open(DATABASE_PATH,&handle);
	if(retval) {
		printf("GSM Report: Database connection failed: %d\n", retval);
		return -1;
	}	

	// Get starttime / endtime
	int starttime, endtime;
	char selectquery [70];
	sprintf(selectquery, "SELECT MIN(Timestamp), MAX(Timestamp) FROM GsmData WHERE Trip_ID = %d", tripid);
	retval = sqlite3_prepare_v2(handle,selectquery,-1,&stmt,0);
	if(retval) {
		printf("GSM Report: Selecting trip from DB Failed: %d\n", retval);
		return -1;
	}
	retval = sqlite3_step(stmt);
	if(retval != SQLITE_ROW) {
		printf("GSM Report: SQL error whilst reading rows: %d\n", retval);
		return -1;
	}
	starttime = (int) sqlite3_column_double(stmt, 0);
	endtime = (int) sqlite3_column_double(stmt, 1);
	sqlite3_step(stmt);

	// Create 10 subrecords for each second between the starttime and endtime
	char insertquery [500];
	for (int second = starttime; second < endtime; second++) {
		for (int subsecond = 0; subsecond < 10; subsecond++) {
			// Insert closest measurement in database
			sprintf(insertquery, "INSERT INTO GsmReport ( 'Trip_ID', 'TimeStamp', 'TimeStampSub', 'SignalStrenght' )"
							 	" SELECT Trip_ID, %d, %d, CAST(Value AS INTEGER)"
							 	" FROM GsmData"
							 	" WHERE Trip_ID = %d AND TimeStamp < %d+10 AND TimeStamp > %d-10"
							 	" ORDER BY ABS(TimeStamp - %d.%d) ASC LIMIT 1",
								second, subsecond, tripid, second, second, second, subsecond);			
			retval = sqlite3_exec(handle,insertquery,0,0,0);			
			if(retval) {
				printf("GSM Report: Inserting data in DB Failed: %d\n", retval);
				printf("Query: %s\n", insertquery);
				return -1;
			}
		}
	}

	// Destroy the evidence!
	sqlite3_close(handle);
	return 0;
}