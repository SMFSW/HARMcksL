/*!\file time_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Time related utilities
**/
/****************************************************************/
#include <time.h>

#include "sarmfsw.h"
#include "time_utils.h"
/****************************************************************/


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


time_t DateTime2time_t(const DateTime * time)
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


DateTime diffDateTime(const DateTime * time2, const DateTime * time1)
{
	time_t t2 = DateTime2time_t(time2);
	time_t t1 = DateTime2time_t(time1);
	time_t diff = difftime(t2, t1);
	return time_t2DateTime(diff);
}

