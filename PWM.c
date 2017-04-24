/*!\file PWM.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Simple PWM handling
**/

#include "PWM.h"

#if defined(HAL_TIM_MODULE_ENABLED)


HAL_StatusTypeDef set_PWM_Freq(TIM_HandleTypeDef * pTim, uint32_t Freq)
{
	const uint32_t	coreCLK = HAL_RCC_GetHCLKFreq();
	uint32_t		per, i;

	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(pTim->Instance));

	if (Freq > coreCLK / 100)	{ return HAL_ERROR; }

	// TODO: find prescaler & period with i++ instead of shifts
	for (i = 1 ; i < (uint16_t) -1 ; i <<= 1)
	{
		per = (coreCLK / (Freq * (i + 1))) - 1;
		if (per <= (uint16_t) -1)	{ break; }				// If in 16b range
		if (i == 1 << 15)			{ return HAL_ERROR; }	// If nothing has been found (last iteration)
	}

	pTim->Init.Period = per;
	pTim->Init.Prescaler = i;

	//timCLK = 1000000;
	//pTim->Init.Prescaler = (coreCLK / timCLK) - 1;
	//pTim->Init.Period = (timCLK / Freq) - 1;

	return HAL_TIM_Base_Init(pTim);
}

/*** PWM DRIVING ***/
__STATIC_INLINE HAL_StatusTypeDef set_PWM_Duty(TIM_HandleTypeDef * pTim, uint32_t Chan, uint16_t CCR_val)
{
	__IO uint32_t * pCCR;	// __IO means volatile for R/W

	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(pTim->Instance));
	assert_param(IS_TIM_CCX_INSTANCE(pTim->Instance, Chan));

	if (Chan <= TIM_CHANNEL_4)		{ pCCR = &pTim->Instance->CCR1 + (Chan / 4); }
	else if (Chan <= TIM_CHANNEL_6)	{ pCCR = &pTim->Instance->CCR5 + (Chan / 4) - 4; }
	else 							{ return HAL_ERROR; }

	*pCCR = CCR_val;
	return HAL_OK;
}

HAL_StatusTypeDef set_PWM_Duty_Scaled(TIM_HandleTypeDef * pTim, uint32_t Chan, uint16_t Duty, uint16_t Scale)
{
	float tmp = ((float) min(Scale, Duty) / (float) Scale) * pTim->Instance->ARR;
	return set_PWM_Duty(pTim, Chan, (uint16_t) tmp);
}


#endif
