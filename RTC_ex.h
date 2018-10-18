/*!\file RTC_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Basic RTC handling
**/
/****************************************************************/
#ifndef __RTC_EX_H
	#define __RTC_EX_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "time_utils.h"

#if defined(HAL_RTC_MODULE_ENABLED)
/****************************************************************/


/*!\brief Sends new time to RTC peripheral
** \param[in] time_new - pointer to DateTime instance
** \return FctERR - error code
**/
FctERR NONNULL__ RTC_SetTime(const DateTime * time_new);

/*!\brief Get time from RTC peripheral
** \param[in,out] time_now - pointer to DateTime instance
** \return FctERR - error code
**/
FctERR NONNULL__ RTC_GetTime(DateTime * time_now);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_RTC_MODULE_ENABLED) */
#endif	/* __RTC_EX_H */
/****************************************************************/
