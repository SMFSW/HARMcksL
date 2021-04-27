/*!\file PWM.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
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
	uint32_t tmp;

	if (!scale)			{ return HAL_ERROR; }	// Division by 0

	if (duty >= scale)	{ tmp = pTim->Instance->ARR + 1; }	// +1 To achieve real 100% duty cycle
	else if (duty == 0)	{ tmp = 0; }
	else				{ tmp = ((uint64_t) duty * pTim->Instance->ARR) / scale; }

	return write_TIM_CCR(pTim, chan, tmp);
}


HAL_StatusTypeDef NONNULL__ init_PWM_Chan(TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint16_t freq, const eState start_polarity)
{
	HAL_StatusTypeDef st = set_TIM_Freq(pTim, freq);

	if (st == HAL_OK)
	{
		unsigned int i, end;

		if (chan == TIM_CHANNEL_ALL)
		{
			i = 0;
			end = 6;	// 6 channels max on bigger MCUs
		}
		else
		{
			i = chan >> 2;
			end = i + 1;
		}

		for ( ; i < end ; i++)
		{
			const uint32_t channel = i << 2;
			if ((st) || (IS_TIM_CCX_INSTANCE(pTim->Instance, channel) == 0))	// Channel does not exist on TIM
			{
				if (chan != TIM_CHANNEL_ALL)	{ st = HAL_ERROR; }				// set HAL_ERROR only for specified channel
				break;
			}

			st = write_TIM_Preload(pTim, channel);
			st |= write_TIM_CCR(pTim, channel, (start_polarity == On) ? pTim->Instance->ARR + 1 : 0);
			st |= set_PWM_Output(pTim, channel, true);
		}
	}

	return st;
}


/****************************************************************/
#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
/****************************************************************/
