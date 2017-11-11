/*!\file time_utils.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Time related utilities
**/
/****************************************************************/
#ifndef __TIME_UTILS_H
	#define __TIME_UTILS_H

#include <time.h>

#include "sarmfsw.h"
/****************************************************************/


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


/*!\brief Convert time_t to DateTime
** \param[in] time - time_t representation
** \return Broken down time representation (DateTime)
**/
DateTime time_t2DateTime(const time_t time);

/*!\brief Convert DateTime to time_t
** \param[in] time - DateTime representation (broken down time)
** \return time_t representation
**/
time_t DateTime2time_t(const DateTime * time);

/*!\brief Calculate DateTime difference
** \param[in] time2 - pointer to closest DateTime representation (broken down time)
** \param[in] time1 - pointer to oldest DateTime representation (broken down time)
** \return DateTime difference
**/
DateTime diffDateTime(const DateTime * time2, const DateTime * time1);


/****************************************************************/
#endif	/* __TIME_UTILS_H */
/****************************************************************/
