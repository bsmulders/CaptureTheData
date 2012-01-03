/*

 This file is part of CaptureTheData!

 Copyright (c) 2011, Bobbie Smulders

 Contact:  mail@bsmulders.com

 GNU General Public License Usage
 This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

#ifndef __CTD_TRIP_H__
#define __CTD_TRIP_H__

/**
 * Create a new trip in the database
 * @param database
 * @return 			The new trip id (or -1 on failure)
 */
int create_trip(char * database);

/**
 * Add additional data to a trip after all data is parsed
 * @param database 	The path to the database
 * @param tripid 	The trip id of the trip to be updated
 * @return 			0 on succes, -1 on failure
 */
int update_trip_data(char * database, int tripid);

/**
 * Add additional data to a trip after all reports are generated
 * @param database 	The path to the database
 * @param tripid 	The trip id of the trip to be updated
 * @return 			0 on succes, -1 on failure
 */
int update_trip_report(char * database, int tripid);

#endif
