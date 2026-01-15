/*!\file random_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief (A little less pseudo) random numbers generation utilities
** \note Randomness is enhanced between MCUs (using UID), through calls and time
** \warning Unfortunately, after reset, assuming call will happen at same clock tick,
** 			random_Get will give same result (unless start saved/restore from some storage
** 			is given as parameter at first call)
**/
/****************************************************************/
#include <stdlib.h>
#include <string.h>

#include "random_utils.h"
#include "exceptions.h"
/****************************************************************/


uint32_t random_Get(const uint32_t start)
{
	static uint32_t			random32;
	static bool				first_call = true;

	const uint32_t * const	pUID = (uint32_t *) UID_BASE;
	const uint32_t			time = HALTicks();
	uint32_t				UID[3];

	if (first_call)
	{
		random32 = start;
		first_call = false;
	}

	UNUSED_RET memcpy(&UID, pUID, sizeof(UID));

	/*** Some kind of tricky random algorithm using STM32 UID & HAL ticks ***/
	// Enhancing randomness through time and calls
	random32 += UID[0];
	random32 ^= time;
	random32 ^= UID[1] >> (time & 0x3UL);
	random32 ^= UID[2] << (time & 0x1UL);

	// Enhancing randomness by changing seed & xor random result with previous one
	// Deepsource would raise limited randomness generation, keeping it quiet with 'skipcq'
	srand(random32);
	random32 ^= rand();		// skipcq: CXX-W2016

	return random32;
}
