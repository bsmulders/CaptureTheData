/*

 This file is part of CaptureTheData!

 Copyright (c) 2011, Bobbie Smulders

 Contact:  mail@bsmulders.com

 GNU General Public License Usage
 This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

#ifndef __CTD_OBD_H__
#define __CTD_OBD_H__

#define OBD_FUEL_SYSTEM_STATUS			0x03
#define OBD_CALCULATED_ENGINE_LOAD 		0x04
#define OBD_ENGINE_COOLANT_TEMPERATURE 	0x05
#define OBD_FUEL_PRESSURE				0x0A
#define OBD_INTAKE_MAP					0x0B
#define OBD_ENGINE_RPM 					0x0C
#define OBD_VEHICLE_SPEED				0x0D
#define OBD_TIMING_ADVANCE				0x0E
#define OBD_INTAKE_AIR_TEMPERATURE		0x0F
#define OBD_MAF_AIR_FLOW_RATE			0x10
#define OBD_THROTTLE_POSITION			0x11

int log_obd(char * device, char * database, int tripid);
int parse_obd(char * database, int tripid);
int generate_obd_report(char * database, int tripid);
int calculate_pid_value(int pid, int a, int b, int c, int d);

#endif
