/*
 * FOSOS RTC driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <rtc.h>

const char* dayNames[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const char* monthNames[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

bool rtc_updating() {
	return cmos_read(0x0a) & 0x80;
}

void rtc_printTime() {
	while(rtc_updating());

	UInt8 hours = cmos_read(0x04);

	print("%s %s %u %u:%u:%u %u",
		dayNames[cmos_read(0x06) - 1], // Day of week
		monthNames[cmos_read(0x08) - 1], // Month
		cmos_read(0x07), // Day of month
		hours > 12 ? hours-12:hours, // Hours
		cmos_read(0x02), // Minutes
		cmos_read(0x00), // Seconds
		(cmos_read(0x32) * 100) + cmos_read(0x09) // Century * 100 + years
	);
}

void rtc_init() {
	byte mode = cmos_read(0x0b);
	mode |= 2; // 24hr mode
	mode |= 4; // No BCD
	cmos_write(0x0b, mode);
}