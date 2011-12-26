/*

 This file is part of CaptureTheData!

 Copyright (c) 2011, Bobbie Smulders

 Contact:  mail@bsmulders.com

 GNU General Public License Usage
 This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

#include "common.h"

#include <stddef.h>
#include <sys/time.h>
#include <string.h>

double current_time_with_ms() {
	struct timeval tim;
	gettimeofday(&tim, NULL);
	return tim.tv_sec + (tim.tv_usec / 1000000.0);
}

// http://bytes.com/topic/c/answers/562961-hex-string-integer-convertor
char xtod(char c) {
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return c = 0; // not Hex digit
}

int hextodec(char *hex) {
	if (*hex == 0)
		return 0;
	return hextodec(hex - 1) * 16 + xtod(*hex);
}

int xstrtoi(char *hex) {
	return hextodec(hex + strlen(hex) - 1);
}
