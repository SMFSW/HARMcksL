/*!\file random_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
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

static uint32_t random32;


uint32_t random_Get(uint32_t start)
{
	static bool			first_call = true;
	static uint32_t *	pUID = (uint32_t *) UID_BASE;
	uint32_t			UID[3], time = HALTicks();

	if (first_call)
	{
		random32 = start;
		first_call = false;
	}

	memcpy(&UID, pUID, sizeof(UID));

	/*** Some kind of tricky random algorithm using STM32 UID & HAL ticks ***/
	// Enhancing randomness through time and calls
	random32 += UID[0];
	random32 ^= time;
	random32 ^= UID[1] >> (time & 0x3);
	random32 ^= UID[2] << (time & 0x1);

	// Enhancing randomness by changing seed & xor random result with previous one
	srand(random32);
	random32 ^= rand();

	return random32;
}
