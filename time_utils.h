/*!\file time_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief Time related utilities
**/
/****************************************************************/
#ifndef __TIME_UTILS_H
	#define __TIME_UTILS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <time.h>

#include "sarmfsw.h"
/****************************************************************/

/*!\enum eMonths
** \brief Months enumeration
**/
typedef enum eMonths {
	January = 0U,	//!< January
	February,		//!< February
	March,			//!< March
	April,			//!< April
	May,			//!< May
	June,			//!< June
	July,			//!< July
	August,			//!< August
	September,		//!< September
	October,		//!< October
	November,		//!< November
	December,		//!< December
	Months_Max
} eMonths;

/*!\enum eWeekdays
** \brief Weekdays enumeration
**/
typedef enum eWeekdays {
	Sunday = 0U,	//!< Sunday
	Monday,			//!< Monday
	Tuesday,		//!< Tuesday
	Wednesday,		//!< Wednesday
	Thursday,		//!< Thursday
	Friday,			//!< Friday
	Saturday,		//!< Saturday
	Weekdays_Max
} eWeekdays;


extern const char Weekdays[Weekdays_Max][10];	//!< Weekday names strings (English)
extern const char Months[Months_Max][10];		//!< Month names strings (English)


/*!\struct DateTime
** \brief Basic Date & Time struct
**/
typedef struct DateTime {
	uint16_t	Year;		//!< Year
	uint8_t		Month;		//!< Month
	uint8_t		Day;		//!< Day
	uint8_t		Weekday;	//!< Weekday
	uint8_t		Hours;		//!< Hours
	uint8_t		Minutes;	//!< Minutes
	uint8_t		Seconds;	//!< Seconds
} DateTime;


/*!\brief Determine weekday for a given date
** \note The algorithm works for dates between 1700 and 2399
** \warning \b month and \b day starts at 1, and no check is done to ensure the date is valid.
** 			You might be caught in a weekday from a void time if the date is not valid! You've been warned!!!
** \param[in] year - Year of the given date
** \param[in] month - Month of the given date (1 to 12)
** \param[in] day - Day of the given date (1 to 31)
** \return true if leap year / false otherwise
**/
eWeekdays Get_Weekday(uint16_t year, uint8_t month, uint8_t day);

/*!\brief Determine if given year is leap
** \param[in] year - Year to test
** \return true if leap year / false otherwise
**/
__INLINE bool is_LeapYear(const uint32_t year) {
	return (bool) (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)); }


/*!\brief Convert time_t to DateTime
** \param[in] time - time_t representation
** \return Broken down time representation (DateTime)
**/
DateTime time_t2DateTime(const time_t time);

/*!\brief Convert DateTime to time_t
** \param[in] time - DateTime representation (broken down time)
** \return time_t representation
**/
time_t NONNULL__ DateTime2time_t(const DateTime * const time);

/*!\brief Calculate DateTime difference
** \param[in] time2 - pointer to closest DateTime representation (broken down time)
** \param[in] time1 - pointer to oldest DateTime representation (broken down time)
** \return DateTime difference
**/
DateTime NONNULL__ diffDateTime(const DateTime * const time2, const DateTime * const time1);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __TIME_UTILS_H */
/****************************************************************/
