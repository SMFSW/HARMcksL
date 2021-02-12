/*!\file tick_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief Core ticks related utilities
**/
/****************************************************************/
#include "sarmfsw.h"
#include "tick_utils.h"
/****************************************************************/


#if (__CORTEX_M != 0U)
void Delay_us(const uint32_t us)
{
	static bool init = false;

	if (!init)
	{
		init = true;

		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	// CoreDebug trace seems enabled only in debug sessions, enable

		#if (__CORTEX_M == 0x07U)
		DWT->LAR = 0xC5ACCE55;							// Cortex M7, need to enable access to DWT
		#endif
		DWT->CYCCNT = 0;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}

	const uint32_t	hNow = DWT->CYCCNT;
	const float		CYCCNT_ratio = (float) SystemCoreClock / 1000000.0f;
	const uint32_t	delay = us * CYCCNT_ratio;

	while (DWT->CYCCNT - hNow < delay);
}
#else
// TODO: use counters based on instruction timing to generate delay (beware execution from RAM/FLASH timings are different)
#endif
