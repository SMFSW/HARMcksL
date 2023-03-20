/*!\file tick_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief Core ticks related utilities
** \warning Of course, these blocking delays are not designed to be used in an RTOS environment where they wouldn't work properly.
** \warning M0/M0+ cores doesn't have core debug trace peripheral, delay shall be generated using a dedicated TIM peripheral.
** \note With cores other than M0/M0+, core debug trace timer is used by default.
** \note In case of M0/M0+ core or willing to use a TIM peripheral on purpose, DELAY_TIM_INST symbol has to be defined with proper TIM instance at project level.
**/
/****************************************************************/
#ifndef __TICK_UTILS_H
	#define __TICK_UTILS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


#if defined(DWT) || defined(DELAY_TIM_INST)

/*!\brief Delay generator initialization function
** \note In case of system core clock frequency change during execution (calls to SystemCoreClockUpdate, HAL_RCC_ClockConfig), init_Delay_us shall be called to update tick ratio.
** \note Debug Trace peripheral timer only: On some Cortex cores, need to enable access to DWT prior to init and use of delays (automatically done only for M7 core).
** \return HAL Status
**/
HAL_StatusTypeDef init_Delay_Generator(void);

/*!\brief Microseconds delay generator (blocking)
** \note Please keep in mind Delay_us is blocking code execution for the given amount of time (except interrupts).
** \param[in] us - delay (in µs)
** \warning us is 32b type parameter for consistency, yet only DWT & 32b TIM instances can handle us parameter more than USHRT_MAX (65535)
**/
void Delay_us(const uint32_t us);

/*!\brief Milliseconds delay generator (blocking)
** \note Please keep in mind Delay_ms is blocking code execution for the given amount of time (except interrupts).
** \param[in] ms - delay (in ms)
** \warning ms is 32b type parameter for consistency, yet if using DWT as delay source, value must be kept below ULONG_MAX / (prescaler * 1000)
**/
void Delay_ms(const uint32_t ms);

#endif


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __TICK_UTILS_H */
/****************************************************************/
