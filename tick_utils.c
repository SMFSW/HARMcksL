/*!\file tick_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Core ticks related utilities
** \warning Of course, these blocking delays are not designed to be used in an RTOS environment where they wouldn't work properly.
** \warning M0/M0+ cores doesn't have core debug trace peripheral, delay shall be generated using a dedicated TIM peripheral.
** \note With cores other than M0/M0+, core debug trace timer is used by default.
** \note In case of M0/M0+ core or willing to use a TIM peripheral on purpose, \c DELAY_TIM_INST symbol has to be defined with proper TIM instance at project level.
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"
#include "tick_utils.h"
/****************************************************************/


static float ticks_per_us = 0.0f;	//!< Counter ticks for a microsecond

float get_TicksPerMicrosecond(void) {
	return ticks_per_us; }


#if defined(DWT) && !defined(DELAY_TIM_INST)

FctERR init_Delay_Generator(void)
{
	ticks_per_us = (float) SystemCoreClock / 1000000.0f;

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	// CoreDebug trace seems enabled only in debug sessions, enable
	#if (__CORTEX_M == 7U)
	DWT->LAR = 0xC5ACCE55UL;						// Cortex M7, need to enable access to DWT
	#endif
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	return ERROR_OK;
}


void Delay_us(const uint32_t us)
{
	DWT->CYCCNT = 0;

	const uint32_t delay = us * ticks_per_us;
	while (DWT->CYCCNT < delay);
}

void Delay_ms(const uint32_t ms)
{
	DWT->CYCCNT = 0;

	const uint32_t delay = ms * ticks_per_us * 1000UL;
	while (DWT->CYCCNT < delay);
}


#elif defined(DELAY_TIM_INST) && defined(HAL_TIM_MODULE_ENABLED)

FctERR init_Delay_Generator(void)
{
	HAL_StatusTypeDef err;

	err = set_TIM_Tick_Freq(DELAY_TIM_INST, 1000000UL);
	if (err)	{ goto ret; }
	err = HAL_TIM_Base_Start(DELAY_TIM_INST);

	ticks_per_us = 1.0f;

	ret:
	return HALERRtoFCTERR(err);
}


void Delay_us(const uint32_t us)
{
	__HAL_TIM_SET_COUNTER(DELAY_TIM_INST, 0);					// set the counter value to 0
	while (__HAL_TIM_GET_COUNTER(DELAY_TIM_INST) < us);			// wait for the counter to reach the us input in the parameter
}

void Delay_ms(const uint32_t ms)
{
	for (uintCPU_t i = 0 ; i < ms ; i++)
	{
		__HAL_TIM_SET_COUNTER(DELAY_TIM_INST, 0);				// set the counter value to 0
		while (__HAL_TIM_GET_COUNTER(DELAY_TIM_INST) < 1000UL);	// wait for the counter to reach the us input in the parameter
	}
}


#endif
