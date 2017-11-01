/*!\file PWM.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Straightforward PWM handling
**/
/****************************************************************/
#include "sarmfsw.h"
#include "PWM.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


/*!\brief Get TIM module clock
** \warning Shall work for all STM32 F families, L families not totally covered
** \param[in,out] pTim - pointer to TIM instance
** \return TIM Clock frequency
**/
static uint32_t get_TIM_clock(TIM_HandleTypeDef * pTim)
{
	uint32_t	refCLK;

	#if defined(STM32F0)
		refCLK = HAL_RCC_GetPCLK1Freq();
	#else
		if (	(pTim->Instance == TIM1)
		#if defined(TIM8)
			||	(pTim->Instance == TIM8)
		#endif
		#if defined(TIM9)
			||	(pTim->Instance == TIM9)
		#endif
		#if defined(TIM10)
			||	(pTim->Instance == TIM10)
		#endif
		#if defined(TIM11)
			||	(pTim->Instance == TIM11)
		#endif
		#if defined(TIM15)
			||	(pTim->Instance == TIM15)
		#endif
		#if defined(TIM16)
			||	(pTim->Instance == TIM16)
		#endif
		#if defined(TIM17)
			||	(pTim->Instance == TIM17)
		#endif
			)
		{
			#if defined (STM32F3)
				if (	((pTim->Instance == TIM1) && (RCC->CFGR3 & RCC_CFGR3_TIM1SW_PLL))
				#if defined(RCC_CFGR3_TIM15SW_PLL)
					||	((pTim->Instance == TIM15) && (RCC->CFGR3 & RCC_CFGR3_TIM15SW_PLL))
				#endif
				#if defined(RCC_CFGR3_TIM16SW_PLL)
					||	((pTim->Instance == TIM16) && (RCC->CFGR3 & RCC_CFGR3_TIM16SW_PLL))
				#endif
				#if defined(RCC_CFGR3_TIM17SW_PLL)
					||	((pTim->Instance == TIM17) && (RCC->CFGR3 & RCC_CFGR3_TIM17SW_PLL))
				#endif
					)
				{	// Get SYCLK (HCLK) frequency
					refCLK = HAL_RCC_GetHCLKFreq() * 2;
				}
				else
			#endif
			{	// Get APB2 (PCLK2) frequency
				refCLK = HAL_RCC_GetPCLK2Freq();
				if ((RCC->CFGR & RCC_CFGR_PPRE2) != 0)	{ refCLK *= 2; }
			}
		}
		else
		{	// Get APB1 (PCLK1) frequency
			refCLK = HAL_RCC_GetPCLK1Freq();
			if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0)	{ refCLK *= 2; }
		}
	#endif

	return refCLK;
}


/*******************/
/*** TIM RELATED ***/
/*******************/
HAL_StatusTypeDef init_TIM_Base(TIM_HandleTypeDef * pTim, uint32_t freq)
{
	HAL_StatusTypeDef err;

	err = set_TIM_Interrupts(pTim, Off);	// Stop interrupts if they were already started
	err = set_TIM_Freq(pTim, freq);			// Configure TIM frequency
	if (err)	{ return err; }
	return set_TIM_Interrupts(pTim, On);	// Start interrupts

}


HAL_StatusTypeDef set_TIM_Freq(TIM_HandleTypeDef * pTim, uint32_t freq)
{
	uint32_t	refCLK, per, i;

	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(pTim->Instance));

	refCLK = get_TIM_clock(pTim);
	if (freq > refCLK / 100)		{ return HAL_ERROR; }

	// TODO: find prescaler & period with i++ instead of shifts for more accuracy (despite of time passed)
	for (i = 1 ; i < (uint16_t) -1 ; i <<= 1)
	{
		per = (refCLK / (freq * (i + 1))) - 1;
		if (per <= (uint16_t) -1)	{ break; }				// If in 16b range
		if (i == 1 << 15)			{ return HAL_ERROR; }	// If nothing has been found (last iteration)
	}

	pTim->Init.Period = per;
	pTim->Init.Prescaler = i;

	return HAL_TIM_Base_Init(pTim);
}


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

		for (unsigned int i = 0 ; i < SZ_OBJ(chans, uint32_t) ; i++)
		{
			st = set_PWM_Output(pTim, chans[i], true);
			if (st)	{ break; }
		}
		return st;
	}
	else return set_PWM_Output(pTim, chan, true);
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

/*****************/
/*** LOGIC PWM ***/
/*****************/
FctERR logPWM_setPin(logicPWM * pPWM, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, bool polarity)
{
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	if (!pPWM)	{ return ERR_INSTANCE; }

	diInterrupts();
	pPWM->cfg.GPIOx = GPIOx;
	pPWM->cfg.GPIO_Pin = GPIO_Pin;
	pPWM->cfg.polarity = polarity;
	enInterrupts();
	return ERR_OK;
}


FctERR logPWM_setFreq(logicPWM * pPWM, TIM_HandleTypeDef * pTim, uint16_t freq, uint16_t granularity)
{
	uint16_t	tim_freq;
	FctERR		err = ERR_OK;

	assert_param(IS_TIM_INSTANCE(pTim->Instance));
	if (!pPWM)	{ return ERR_INSTANCE; }

	granularity = max(10, granularity);

	// TODO: set some limits for frequency (regarding granularity?)

	if (	(pTim->Instance->DIER & TIM_IT_UPDATE)
		&&	(pTim->Instance->CR1 & TIM_CR1_CEN))
	{	// Timer already started (try to set period according to already configured timer module)
		tim_freq = get_TIM_clock(pTim) / ((pTim->Init.Period + 1) * (pTim->Init.Prescaler + 1));
		if ((tim_freq / granularity) < freq)	{ return ERR_VALUE; }
		granularity = tim_freq / freq;
	}
	else
	{
		tim_freq = freq * granularity;
		err = init_TIM_Base(pTim, tim_freq);
		if (err)	{ return err; }
	}

	diInterrupts();
	pPWM->cfg.tim_freq = tim_freq;
	pPWM->cfg.per = granularity;
	pPWM->cfg.pTim = pTim;
	enInterrupts();

	return ERR_OK;
}


FctERR logPWM_setDuty(logicPWM * pPWM, uint16_t val)
{
	uint16_t duty;

	if (!pPWM)	{ return ERR_INSTANCE; }

	if (val == 65535)	{ duty = pPWM->cfg.per; }
	else if (val == 0)	{ duty = 0; }
	else				{ duty = (uint16_t) max(0, ((((val + 1) * (pPWM->cfg.per)) / 65536) - 1)); }

	diInterrupts();
	pPWM->cfg.duty = duty;
	enInterrupts();

	return ERR_OK;
}


FctERR logPWM_getFreq(uint16_t * freq, logicPWM * pPWM)
{
	if (!pPWM)	{ return ERR_INSTANCE; }
	*freq = pPWM->cfg.tim_freq / pPWM->cfg.per;
	return ERR_OK;
}


FctERR logPWM_getDutyCycle(float * duty, logicPWM * pPWM)
{
	if ((!pPWM) || (!duty))	{ return ERR_INSTANCE; }
	*duty = ((float) pPWM->cfg.duty * 100) / (float) pPWM->cfg.per;
	return ERR_OK;
}


void logPWM_handler(logicPWM * pPWM)
{
	assert_param(IS_GPIO_PIN(pPWM->cfg.GPIO_Pin));
	if (!pPWM)	{ return; }

	if (pPWM->cfg.GPIOx)
	{
		if (!pPWM->cntr)					// period over
		{
			pPWM->cntr = pPWM->cfg.per;		// Reload new period information
			pPWM->duty = pPWM->cfg.duty;	// Load new duty cycle

			// if duty has been reached (real 0-100% duty cycles)
			if (pPWM->cntr == pPWM->duty)	HAL_GPIO_WritePin(pPWM->cfg.GPIOx, pPWM->cfg.GPIO_Pin, pPWM->cfg.polarity ? GPIO_PIN_SET : GPIO_PIN_RESET);	// switch channel on
			else							HAL_GPIO_WritePin(pPWM->cfg.GPIOx, pPWM->cfg.GPIO_Pin, pPWM->cfg.polarity ? GPIO_PIN_RESET : GPIO_PIN_SET);	// switch channel off
		}
		else
		{
			// if duty has been reached (duty cycle)
			if (pPWM->cntr == pPWM->duty)	HAL_GPIO_WritePin(pPWM->cfg.GPIOx, pPWM->cfg.GPIO_Pin, pPWM->cfg.polarity ? GPIO_PIN_SET : GPIO_PIN_RESET);	// switch channel on
		}

		pPWM->cntr--;	// decrease channel counter
	}
}


/****************************************************************/
#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
/****************************************************************/
