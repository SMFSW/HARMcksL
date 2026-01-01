/*!\file RTC_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Basic RTC handling
**/
/****************************************************************/
#include "sarmfsw.h"
#include "time_utils.h"
#include "RTC_ex.h"

#if defined(HAL_RTC_MODULE_ENABLED)
/****************************************************************/


FctERR NONNULL__ RTC_SetTime(const DateTime * const time_new)
{
	HAL_StatusTypeDef	st;
	RTC_TimeTypeDef		RTCTime;
	RTC_DateTypeDef		RTCDate;

	#if defined(STM32F2) || defined(STM32F3) || defined(STM32F4) || defined(STM32F7) || defined(STM32H7) || defined(STM32L0) || defined(STM32L1) || defined(STM32L4)
	RTCTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	RTCTime.StoreOperation = RTC_STOREOPERATION_RESET;
	#endif
	RTCTime.Hours = time_new->Hours;
	RTCTime.Minutes = time_new->Minutes;
	RTCTime.Seconds = time_new->Seconds;
	RTCDate.Year = time_new->Year;
	RTCDate.Month = time_new->Month;
	RTCDate.Date = time_new->Day;
	RTCDate.WeekDay = Get_Weekday(time_new->Year + 2000U, time_new->Month, time_new->Day);
	if (RTCDate.WeekDay == Sunday)	{ RTCDate.WeekDay = 7U; }	// To match with STM32 RTC weekdays definitions

	st = HAL_RTC_SetTime(&hrtc, &RTCTime, RTC_FORMAT_BIN);
	st |= HAL_RTC_SetDate(&hrtc, &RTCDate, RTC_FORMAT_BIN);
	if (st != HAL_OK)	{ return ERROR_FAULT; }

	return ERROR_OK;
}


FctERR NONNULL__ RTC_GetTime(DateTime * const time_now)
{
	HAL_StatusTypeDef	st;
	RTC_TimeTypeDef		RTCTime;
	RTC_DateTypeDef		RTCDate;

	st = HAL_RTC_GetTime(&hrtc, &RTCTime, RTC_FORMAT_BIN);
	st |= HAL_RTC_GetDate(&hrtc, &RTCDate, RTC_FORMAT_BIN);
	if (st != HAL_OK)	{ return ERROR_FAULT; }

	time_now->Hours = RTCTime.Hours;
	time_now->Minutes = RTCTime.Minutes;
	time_now->Seconds = RTCTime.Seconds;
	time_now->Year = RTCDate.Year;
	time_now->Month = RTCDate.Month;
	time_now->Day = RTCDate.Date;
	time_now->Weekday = RTCDate.WeekDay;
	if (time_now->Weekday == 7U)	{ time_now->Weekday = Sunday; }	// To match with STM32 RTC weekdays definitions

	return ERROR_OK;
}


/****************************************************************/
#endif	/* defined(HAL_RTC_MODULE_ENABLED) */
/****************************************************************/
