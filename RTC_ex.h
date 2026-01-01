/*!\file RTC_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Basic RTC handling
**/
/****************************************************************/
#ifndef RTC_EX_H__
	#define RTC_EX_H__

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
FctERR NONNULL__ RTC_SetTime(const DateTime * const time_new);

/*!\brief Get time from RTC peripheral
** \param[in,out] time_now - pointer to DateTime instance
** \return FctERR - error code
**/
FctERR NONNULL__ RTC_GetTime(DateTime * const time_now);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_RTC_MODULE_ENABLED) */
#endif	/* RTC_EX_H__ */
/****************************************************************/
