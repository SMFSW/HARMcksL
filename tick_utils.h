/*!\file tick_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief Core ticks related utilities
**/
/****************************************************************/
#ifndef __TICK_UTILS_H
	#define __TICK_UTILS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


#if (__CORTEX_M != 0U)
/*!\brief Microseconds delay generator (blocking)
** \note Please keep in mind Delay_us is blocking code execution for the given amount of time (except interrupts).
** \note On some Cortex cores, need to enable access to DWT prior to init Delay_us (automatically done only for M7 core).
** \note Cortex cores M0/M0+ doesn't have DWT peripheral (the function is disabled). TODO: find another way to implement Delay_us for M0/M0+ cores
** \warning Delay initialization instructions being executed on first call, this delay will last a little longer.
**			If delay really needs to be accurate on first go, call Delay_us(1) at startup before really using it.
** \warning As for functions like HAL_Delay, don't expect accurate delay for just a few us (through tests, overtaking is observed weighted by System Clock)
** \param[in] delay - delay (in µs)
**/
void Delay_us(const uint32_t us);
#endif


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __TICK_UTILS_H */
/****************************************************************/
