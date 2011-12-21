/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

#ifndef __CTD_GSM_H__
#define __CTD_GSM_H__

#define GSM_CELL_SIGNAL_QUALITY	"AT+CSQ"

int log_gsm(char * device, char * database, int tripid);
int parse_gsm(char * database, int tripid);
int generate_gsm_report(char * database, int tripid);

#endif