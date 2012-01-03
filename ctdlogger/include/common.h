/*

 This file is part of CaptureTheData!

 Copyright (c) 2011, Bobbie Smulders

 Contact:  mail@bsmulders.com

 GNU General Public License Usage
 This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

 */

#ifndef __CTD_COMMON_H__
#define __CTD_COMMON_H__

/**
 * Get the current time in milliseconds
 * @return 			Current time in milliseconds
 */
double current_time_with_ms();

/**
 * Convert hex string to integer
 * Found on http://bytes.com/topic/c/answers/562961-hex-string-integer-convertor
 * @param hex 		The hex string to convert
 * @return			The converted integer
 */
int xstrtoi(char *hex);

#endif
