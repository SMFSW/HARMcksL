/*!\file RTC_ex.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Basic RTC handling
**/
/****************************************************************/
#ifndef __RTC_EX_H
	#define __RTC_EX_H

#include "sarmfsw.h"
#include "time_utils.h"

#if defined(HAL_RTC_MODULE_ENABLED)
/****************************************************************/


/*!\brief Sends new time to RTC peripheral
** \param[in] time_new - pointer to DateTime instance
** \return FctERR - error code
**/
FctERR RTC_SetTime(DateTime * time_new);

/*!\brief Get time from RTC peripheral
** \param[in,out] time_now - pointer to DateTime instance
** \return FctERR - error code
**/
FctERR RTC_GetTime(DateTime * time_now);


/****************************************************************/
#endif	/* defined(HAL_RTC_MODULE_ENABLED) */
#endif	/* __RTC_EX_H */
/****************************************************************/
