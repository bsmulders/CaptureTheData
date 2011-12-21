/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

#include "trip.h"
#include "gps.h"
#include "gsm.h"
#include "obd.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** args) {
	if (args[1] == NULL) {
		printf("No arguments given. Use create, log {gps|gsm|obd} {tripid}, parse {tripid}, report {tripid}\n");
		return -1;
	}
	else if (strcmp(args[1], "create") == 0) {
		if (args[2] == NULL) {
			printf("Please enter a database\n");
			return -1;
		}
		
		int tripid = create_trip(args[2]);

		if (tripid > 0)
			return tripid;
		
		return 0;
	}
	else if (strcmp(args[1], "log") == 0) {
		if (args[2] == NULL) {
			printf("Please enter a sensor type\n");
			return -1;
		}
		else if (args[3] == NULL) {
			printf("Please enter a device name\n");
			return -1;
		} else if (args[4] == NULL) {
			printf("Please enter a database\n");
			return -1;
		} else if (args[5] == NULL) {
			printf("Please enter a trip id\n");
			return -1;
		}
		
		if (strcmp(args[2], "gsm") == 0) 
			return log_gsm(args[3], args[4], atoi(args[5]));
		else if (strcmp(args[2], "gps") == 0)
			return log_gps(args[3], args[4], atoi(args[5]));
		else if (strcmp(args[2], "obd") == 0)
			return log_obd(args[3], args[4], atoi(args[5]));
	}
	else if (strcmp(args[1], "parse") == 0) {
		if (args[2] == NULL) {
			printf("Please enter a database\n");
			return -1;
		} else if (args[3] == NULL) {
			printf("Please enter a trip id\n");
			return -1;
		}
		
		if (parse_gps(args[2], atoi(args[3])) < 0) 
			return -1;
		if (parse_gsm(args[2], atoi(args[3])) < 0) 
			return -1;
		if (parse_obd(args[2], atoi(args[3])) < 0) 
			return -1;
		if (parse_trip(args[2], atoi(args[3])) < 0) 
			return -1;

		return 0;
	} 
	else if (strcmp(args[1], "report") == 0) {
		if (args[2] == NULL) {
			printf("Please enter a database\n");
			return -1;
		} else if (args[3] == NULL) {
			printf("Please enter a trip id\n");
			return -1;
		}

		if (generate_gps_report(args[2], atoi(args[3])) < 0) 
			return -1;
		if (generate_gsm_report(args[2], atoi(args[3])) < 0) 
			return -1;
		if (generate_obd_report(args[2], atoi(args[3])) < 0)
			return -1;
		
		return 0;
	}
	else {
		printf("Wrong arguments given. Use create, log {gps|gsm|obd} {tripid}, parse {tripid}, report {tripid}\n");
		return -1;
	}
}