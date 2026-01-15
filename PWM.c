/*!\file PWM.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Straightforward PWM handling
** \warning Init functions assume that TIM peripherals instance were already configured by HAL
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"
#include "PWM.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


HAL_StatusTypeDef NONNULL__ set_PWM_Duty_Scaled(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint32_t duty, const uint32_t scale)
{
	HAL_StatusTypeDef st = HAL_ERROR;

	if (scale != 0U)	// Avoid div by 0
	{
		uint32_t tmp;

		if (duty >= scale)		{ tmp = pTim->Instance->ARR + 1U; }	// +1 To achieve real 100% duty cycle
		else if (duty == 0U)	{ tmp = 0U; }
		else					{ tmp = ((uint64_t) duty * pTim->Instance->ARR) / scale; }

		st = write_TIM_CCR(pTim, chan, tmp);
	}

	return st;
}


HAL_StatusTypeDef NONNULL__ init_PWM_Chan(TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint32_t freq, const eState start_polarity)
{
	HAL_StatusTypeDef st = set_TIM_Freq(pTim, freq);

	if (st == HAL_OK)
	{
		uintCPU_t i;
		uintCPU_t end;

		if (chan == TIM_CHANNEL_ALL)
		{
			i = 0U;
			end = 6UL;	// 6 channels max on bigger MCUs
		}
		else
		{
			i = chan >> 2UL;
			end = i + 1UL;
		}

		while (i < end)
		{
			const uint32_t channel = (i++) << 2UL;
			if ((st) || (IS_TIM_CCX_INSTANCE(pTim->Instance, channel) == 0U))	// Channel does not exist on TIM
			{
				if (chan != TIM_CHANNEL_ALL)	{ st = HAL_ERROR; }				// set HAL_ERROR only for specified channel
				break;
			}

			st = write_TIM_Preload(pTim, channel);
			st |= write_TIM_CCR(pTim, channel, (start_polarity == On) ? (pTim->Instance->ARR + 1UL) : 0UL);
			st |= set_PWM_Output(pTim, channel, true);
		}
	}

	return st;
}


/****************************************************************/
#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
/****************************************************************/
