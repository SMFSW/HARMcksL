/*!\file tick_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief Core ticks related utilities
** \warning Of course, these blocking delays are not designed to be used in an RTOS environment where they wouldn't work properly.
** \warning M0/M0+ cores doesn't have core debug trace peripheral, delay shall be generated using a dedicated TIM peripheral.
** \note With cores other than M0/M0+, core debug trace timer is used by default.
** \note In case of M0/M0+ core or willing to use a TIM peripheral on purpose, DELAY_TIM_INST symbol has to be defined with proper TIM instance at project level.
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"
#include "tick_utils.h"
/****************************************************************/


#if defined(DWT) && !defined(DELAY_TIM_INST)

static float CYCCNT_ticks_us;	//!< Counter ticks for a microsecond

HAL_StatusTypeDef init_Delay_Generator(void)
{
	CYCCNT_ticks_us = (float) SystemCoreClock / 1000000.0f;

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	// CoreDebug trace seems enabled only in debug sessions, enable
	#if (__CORTEX_M == 7U)
	DWT->LAR = 0xC5ACCE55;							// Cortex M7, need to enable access to DWT
	#endif
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	return HAL_OK;
}

void Delay_us(const uint32_t us)
{
	DWT->CYCCNT = 0;

	const uint32_t delay = us * CYCCNT_ticks_us;
	while (DWT->CYCCNT < delay);
}

void Delay_ms(const uint32_t ms)
{
	DWT->CYCCNT = 0;

	const uint32_t delay = ms * CYCCNT_ticks_us * 1000;
	while (DWT->CYCCNT < delay);
}

#elif defined(DELAY_TIM_INST) && defined(HAL_TIM_MODULE_ENABLED)

HAL_StatusTypeDef init_Delay_Generator(void)
{
	HAL_StatusTypeDef err;

	err = set_TIM_Tick_Freq(DELAY_TIM_INST, 1000000);
	if (err)	{ return err; }
	return HAL_TIM_Base_Start(DELAY_TIM_INST);
}

void Delay_us(const uint32_t us)
{
	__HAL_TIM_SET_COUNTER(DELAY_TIM_INST, 0);					// set the counter value to 0
	while (__HAL_TIM_GET_COUNTER(DELAY_TIM_INST) < us);			// wait for the counter to reach the us input in the parameter
}

void Delay_ms(const uint32_t ms)
{
	for (unsigned int i = 0 ; i < ms ; i++)
	{
		__HAL_TIM_SET_COUNTER(DELAY_TIM_INST, 0);				// set the counter value to 0
		while (__HAL_TIM_GET_COUNTER(DELAY_TIM_INST) < 1000);	// wait for the counter to reach the us input in the parameter
	}
}

#endif
