/*!\file tick_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
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


/*!\brief Microseconds delay generator (blocking)
** \note Please keep in mind Delay_us is blocking code execution for the given amount of time (except interrupts).
** \note On Cortex M7 (and maybe some other cores), need to enable access to DWT manually prior to call Delay_us.
** \warning Delay initialization instructions being executed on first call, this delay will last a little longer.
**			If delay really needs to be accurate on first go, call Delay_us(1) at startup before using it for the purpose.
** \warning As for functions like HAL_Delay, don't expect accurate delay for just a few us (through tests, overtaking is observed weighted by System Clock)
** \param[in] delay - delay (in µs)
**/
void Delay_us(const uint32_t us);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __TICK_UTILS_H */
/****************************************************************/
