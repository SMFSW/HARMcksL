/*!\file time_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief Time related utilities
**/
/****************************************************************/
#include <time.h>

#include "sarmfsw.h"
#include "time_utils.h"
/****************************************************************/

const char Weekdays[Weekdays_Max][10] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const char Months[Months_Max][10] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };


eWeekdays Get_Weekday(uint16_t year, uint8_t month, uint8_t day)
{
	if ((year < 1700) || (year >= 2400))	{ return -1; }

	static const uint8_t century_code[7] = { 4, 2, 0, 6, 4, 2, 0 };		// 1700s, 1800s, 1900s, 2000s, 2100s, 2200s, 2300s
	static const uint8_t month_code[Months_Max] = { 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };

	const uint8_t leap = ((month < 3) && (is_LeapYear(year))) ? 1 : 0;

	uint16_t year_code = year % 100;
	year_code = (year_code + (year_code / 4)) % 7;

	const uint8_t code = (year_code + month_code[month - 1] + century_code[(year / 100) - 17] + day - leap) % 7;

	return code;
}


DateTime time_t2DateTime(const time_t time)
{
	DateTime	time_dt;
	struct tm *	time_tm;

	time_tm = gmtime(&time);

	time_dt.Year = time_tm->tm_year - 100;
	time_dt.Month = time_tm->tm_mon + 1;
	time_dt.Day = time_tm->tm_mday;
	time_dt.Weekday = time_tm->tm_wday;
	time_dt.Hours = time_tm->tm_hour;
	time_dt.Minutes = time_tm->tm_min;
	time_dt.Seconds = time_tm->tm_sec;

	return time_dt;
}


time_t NONNULL__ DateTime2time_t(const DateTime * const time)
{
	struct tm time_tm;

	time_tm.tm_isdst = 0;
	time_tm.tm_year = time->Year + 100;
	time_tm.tm_mon = time->Month - 1;
	time_tm.tm_mday = time->Day;
	time_tm.tm_wday = time->Weekday;
	time_tm.tm_hour = time->Hours;
	time_tm.tm_min = time->Minutes;
	time_tm.tm_sec = time->Seconds;

	return mktime(&time_tm);
}


DateTime NONNULL__ diffDateTime(const DateTime * const time2, const DateTime * const time1)
{
	const time_t t2 = DateTime2time_t(time2);
	const time_t t1 = DateTime2time_t(time1);
	const time_t diff = difftime(t2, t1);
	return time_t2DateTime(diff);
}

