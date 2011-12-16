/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

#include "ctd.h"
#include "trip.h"
#include "gps.h"
#include "gsm.h"
#include "obd.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** args) {
	if (args[1] == NULL) {
		printf("No arguments given. Use create, gsm {tripid}, gps {tripid}, parse {tripid}, report {tripid}\n");
		return -1;
	}
	else if (strcmp(args[1], "parse") == 0) {
		if (args[2] == NULL) {
			printf("Please enter a trip id\n");
			return -1;
		}
		
		if (parse_gps(atoi(args[2])) < 0) 
			return -1;
		if (parse_gsm(atoi(args[2])) < 0) 
			return -1;
		if (parse_trip(atoi(args[2])) < 0) 
			return -1;

		return 0;
	} 
	else if (strcmp(args[1], "report") == 0) {
		if (args[2] == NULL) {
			printf("Please enter a trip id\n");
			return -1;
		}

		if (generate_gps_report(atoi(args[2])) < 0) 
			return -1;
		if (generate_gsm_report(atoi(args[2])) < 0) 
			return -1;
		
		return 0;
	} 
	else if (strcmp(args[1], "create") == 0) {
		int tripid = create_trip();

		if (tripid > 0) {
			printf("Created a new trip: %d\n", tripid);
		}

		return 0;
	} 
	else if (strcmp(args[1], "gsm") == 0) {
		if (args[2] == NULL) {
			printf("Please enter a trip id\n");
			return -1;
		}

		return log_gsm(atoi(args[2]));
	} 
	else if (strcmp(args[1], "gps") == 0) {
		if (args[2] == NULL) {
			printf("Please enter a trip id\n");
			return -1;
		}
		
		return log_gps(atoi(args[2]));
	} 
	else {
		printf("Wrong arguments given. Use create, gsm {tripid}, gps {tripid}, parse {tripid}, report {tripid}\n");
		return -1;
	}
}