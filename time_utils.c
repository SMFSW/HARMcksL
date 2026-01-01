/*!\file time_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Time related utilities
**/
/****************************************************************/
#include <time.h>

#include "sarmfsw.h"
#include "time_utils.h"
/****************************************************************/


const char * Get_Day_str(const eWeekdays day)
{
	static const char Weekdays[][10] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "" };

	return Weekdays[(day >= Weekdays_Max) ? Weekdays_Max : day];
}

const char * Get_Month_str(const eMonths month)
{
	static const char Months[][10] = { "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	return Months[(month >= Months_Max) ? 0U : month];
}


eWeekdays Get_Weekday(const uint16_t year, const uint8_t month, const uint8_t day)
{
	static const uintCPU_t century_code[7] = { 4U, 2U, 0U, 6U, 4U, 2U, 0U };		// 1700s, 1800s, 1900s, 2000s, 2100s, 2200s, 2300s
	static const uintCPU_t month_code[12] = { 0U, 3U, 3U, 6U, 1U, 4U, 6U, 2U, 5U, 0U, 3U, 5U };

	uintCPU_t code = (uintCPU_t) -1;

	if ((year >= 1700U) && (year < 2400U))
	{
		const uintCPU_t leap = ((month < 3U) && (is_LeapYear(year))) ? 1U : 0U;

		uintCPU_t year_code = year;
		year_code %= 100U;
		year_code += (year_code / 4U);
		year_code %= 7U;

		code = year_code + month_code[month - 1U] + century_code[(year / 100U) - 17U] + day - leap;
		code %= 7U;
	}

	return (eWeekdays) code;
}


DateTime time_t2DateTime(const time_t time)
{
	DateTime			time_dt;
	const struct tm *	time_tm;

	time_tm = gmtime(&time);

	time_dt.Year = (uint16_t) (time_tm->tm_year - 100);
	time_dt.Month = (eMonths) (time_tm->tm_mon + 1);
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
	time_tm.tm_year = (int) time->Year + 100;
	time_tm.tm_mon = (int) time->Month - 1;
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

