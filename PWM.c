/*!\file PWM.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Straightforward PWM handling
**/
/****************************************************************/
#include "PWM.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


HAL_StatusTypeDef init_PWM_Chan(TIM_HandleTypeDef * pTim, uint32_t chan, uint16_t freq)
{
	HAL_StatusTypeDef	st;

	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(pTim->Instance));
	assert_param(IS_TIM_CHANNELS(chan));

	st = set_TIM_Freq(pTim, freq);
	if (st)	{ return st; }

	if (chan == TIM_CHANNEL_ALL)
	{
		#if defined(TIM_CHANNEL_6)
			uint32_t chans[6] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_5, TIM_CHANNEL_6 };
		#else
			uint32_t chans[4] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };
		#endif

		for (int i = 0 ; i < SZ_OBJ(chans, uint32_t) ; i++)
		{
			st = set_PWM_Output(pTim, chans[i], true);
			if (st)	{ break; }
		}
		return st;
	}
	else return set_PWM_Output(pTim, chan, true);
}


HAL_StatusTypeDef set_TIM_Freq(TIM_HandleTypeDef * pTim, uint32_t freq)
{
	uint32_t	coreCLK, per, i;

	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(pTim->Instance));

	if (	((TIM_TypeDef *) pTim == TIM1)
#if defined(TIM8)
		||	((TIM_TypeDef *) pTim == TIM8)
#endif
#if defined(TIM9)
		||	((TIM_TypeDef *) pTim == TIM9)
#endif
#if defined(TIM10)
		||	((TIM_TypeDef *) pTim == TIM10)
#endif
#if defined(TIM11)
		||	((TIM_TypeDef *) pTim == TIM11)
#endif
#if defined(TIM15)
		||	((TIM_TypeDef *) pTim == TIM15)
#endif
#if defined(TIM16)
		||	((TIM_TypeDef *) pTim == TIM16)
#endif
#if defined(TIM17)
		||	((TIM_TypeDef *) pTim == TIM17)
#endif
	)		{ coreCLK = HAL_RCC_GetPCLK1Freq(); }	// Get APB2 (PCLK1) frequency
	else	{ coreCLK = HAL_RCC_GetPCLK2Freq();	}	// Get APB1 (PCLK2) frequency

	if (freq > coreCLK / 3)		{ return HAL_ERROR; }

	// TODO: find prescaler & period with i++ instead of shifts for more accuracy (despite of time passed)
	for (i = 1 ; i < (uint16_t) -1 ; i <<= 1)
	{
		per = (coreCLK / (freq * (i + 1))) - 1;
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


/*******************/
/*** PWM DRIVING ***/
/*******************/
/*!\brief Low level TIM module PWM duty cycle write
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] CCR_val - Scaled duty cycle for CCR register
** \return HAL Status
**/
__STATIC_INLINE HAL_StatusTypeDef INLINE__ write_CCR(TIM_HandleTypeDef * pTim, uint32_t chan, uint16_t CCR_val)
{
	__IO uint32_t * pCCR;	// __IO means volatile for R/W

	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(pTim->Instance));
	assert_param(IS_TIM_CCX_INSTANCE(pTim->Instance, chan));

	if (chan <= TIM_CHANNEL_4)			{ pCCR = &pTim->Instance->CCR1 + (chan / 4); }
	#if defined(STM32F3)
		else if (chan <= TIM_CHANNEL_6)	{ pCCR = &pTim->Instance->CCR5 + (chan / 4) - 4; }
	#endif
	else 								{ return HAL_ERROR; }

	*pCCR = CCR_val;

	return HAL_OK;
}


HAL_StatusTypeDef set_PWM_Duty_Scaled(TIM_HandleTypeDef * pTim, uint32_t chan, uint16_t duty, uint16_t scale)
{
	float tmp = ((float) min(scale, duty) / (float) scale) * pTim->Instance->ARR;
	return write_CCR(pTim, chan, (uint16_t) tmp);
}


/****************************************************************/
#endif
/****************************************************************/
