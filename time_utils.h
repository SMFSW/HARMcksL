/*!\file time_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Time related utilities
**/
/****************************************************************/
#ifndef TIME_UTILS_H__
	#define TIME_UTILS_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include <time.h>

#include "sarmfsw.h"
/****************************************************************/

/*!\enum eMonths
** \brief Months enumeration
**/
typedef enum {
	January = 1U,	//!< January
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
typedef enum {
	Sunday = 0U,	//!< Sunday
	Monday,			//!< Monday
	Tuesday,		//!< Tuesday
	Wednesday,		//!< Wednesday
	Thursday,		//!< Thursday
	Friday,			//!< Friday
	Saturday,		//!< Saturday
	Weekdays_Max
} eWeekdays;


/*!\struct DateTime
** \brief Basic Date & Time struct
**/
typedef struct {
	uint16_t	Year;		//!< Year
	eMonths		Month;		//!< Month
	eWeekdays	Day;		//!< Day
	uint8_t		Weekday;	//!< Weekday
	uint8_t		Hours;		//!< Hours
	uint8_t		Minutes;	//!< Minutes
	uint8_t		Seconds;	//!< Seconds
} DateTime;


/*!\brief Get weekday name string
** \param[in] day - Weekday from \ref eWeekdays enum
** \return Weekday string pointer
**/
const char * Get_Day_str(const eWeekdays day);

/*!\brief Get month name string
** \param[in] month - Month from \ref eMonths enum
** \return Month string pointer
**/
const char * Get_Month_str(const eMonths month);


/*!\brief Determine weekday for a given date
** \note The algorithm works for dates between 1700 and 2399
** \warning \b month and \b day starts at 1, and no check is done to ensure the date is valid.
** 			You might be caught in a weekday from a void time if the date is not valid! You've been warned!!!
** \param[in] year - Year of the given date
** \param[in] month - Month of the given date (1 to 12)
** \param[in] day - Day of the given date (1 to 31)
** \return true if leap year / false otherwise
**/
eWeekdays Get_Weekday(const uint16_t year, const uint8_t month, const uint8_t day);

/*!\brief Determine if given year is leap
** \param[in] year - Year to test
** \return true if leap year / false otherwise
**/
__INLINE bool is_LeapYear(const uint32_t year) {
	return (bool)	(	(((year % 4U) == 0U) && ((year % 100U) != 0U))
					 ||	((year % 400U) == 0U) ); }


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

#endif	/* TIME_UTILS_H__ */
/****************************************************************/
