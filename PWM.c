/*!\file PWM.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Straightforward PWM handling
** \warning Shall work for all STM32 F families, L families not totally covered
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
static uint32_t NONNULL__ get_TIM_clock(const TIM_HandleTypeDef * const pTim)
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
			#if defined(STM32F3)
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


/*!\brief Set Preload bit in TIM module for desired channel
** \param[in,out] pTim - pointer to TIM instance
** \param[in] chan - Channel to write
** \return HAL Status
**/
static HAL_StatusTypeDef NONNULL__ set_PWM_Preload_bit(TIM_HandleTypeDef * const pTim, const uint32_t chan)
{
	switch (chan)	/* Set the Preload enable bit for channel */
	{
		default:
			return HAL_ERROR;

		case TIM_CHANNEL_1:
			pTim->Instance->CCMR1 |= TIM_CCMR1_OC1PE;
			break;

		case TIM_CHANNEL_2:
			pTim->Instance->CCMR1 |= TIM_CCMR1_OC2PE;
			break;

		case TIM_CHANNEL_3:
			pTim->Instance->CCMR2 |= TIM_CCMR2_OC3PE;
			break;

		case TIM_CHANNEL_4:
			pTim->Instance->CCMR2 |= TIM_CCMR2_OC4PE;
			break;

		#if defined(TIM_CHANNEL_6)
		case TIM_CHANNEL_5:
			pTim->Instance->CCMR3 |= TIM_CCMR3_OC5PE;
			break;

		case TIM_CHANNEL_6:
			pTim->Instance->CCMR3 |= TIM_CCMR3_OC6PE;
			break;
		#endif
	}

	return HAL_OK;
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
__STATIC_INLINE HAL_StatusTypeDef NONNULL_INLINE__ set_PWM_CCR(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint16_t CCR_val)
{
	__IO uint32_t * pCCR;

	assert_param(IS_TIM_INSTANCE(pTim->Instance));
	assert_param(IS_TIM_CCX_INSTANCE(pTim->Instance, chan));

	if (chan <= TIM_CHANNEL_4)			{ pCCR = &pTim->Instance->CCR1 + (chan / 4); }
	#if defined(TIM_CHANNEL_6)
		else if (chan <= TIM_CHANNEL_6)	{ pCCR = &pTim->Instance->CCR5 + (chan / 4) - 4; }
	#endif
	else 								{ return HAL_ERROR; }

	*pCCR = CCR_val;

	return HAL_OK;
}


HAL_StatusTypeDef NONNULL__ set_PWM_Duty_Scaled(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint16_t duty, const uint16_t scale)
{
	uint16_t tmp;

	if (!scale)			{ return HAL_ERROR; }	// Division by 0

	if (duty >= scale)	{ tmp = pTim->Instance->ARR + 1; }	// +1 To achieve real 100% duty cycle
	else if (duty == 0)	{ tmp = 0; }
	else				{ tmp = (duty * pTim->Instance->ARR) / scale; }

	return set_PWM_CCR(pTim, chan, tmp);
}


/*******************/
/*** TIM RELATED ***/
/*******************/
HAL_StatusTypeDef NONNULL__ init_TIM_Base(TIM_HandleTypeDef * const pTim, const uint32_t freq)
{
	HAL_StatusTypeDef err;

	err = set_TIM_Interrupts(pTim, Off);	// Stop interrupts if they were already started
	err = set_TIM_Freq(pTim, freq);			// Configure TIM frequency
	if (err)	{ return err; }
	return set_TIM_Interrupts(pTim, On);	// Start interrupts
}


HAL_StatusTypeDef NONNULL__ set_TIM_Freq(TIM_HandleTypeDef * const pTim, const uint32_t freq)
{
	// TODO: now forming a uint16_t max scale (better in 32b?)
	uint32_t	refCLK, per, i;

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


HAL_StatusTypeDef NONNULL__ init_PWM_Chan(TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint16_t freq)
{
	HAL_StatusTypeDef	st;

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
			st = set_PWM_Preload_bit(pTim, chans[i]);
			//st |= set_PWM_CCR(pTim, chans[i], 0);
			st |= set_PWM_Output(pTim, chans[i], true);
			if (st)	{ break; }
		}
		return st;
	}
	else
	{
		st = set_PWM_Preload_bit(pTim, chan);
		//st |= set_PWM_CCR(pTim, chan, 0);
		st |= set_PWM_Output(pTim, chan, true);
		return st;
	}
}


/****************************************************************/

/*****************/
/*** LOGIC PWM ***/
/*****************/
FctERR NONNULL__ logPWM_setPin(logicPWM * const pPWM, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const bool polarity)
{
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	diInterrupts();
	pPWM->cfg.GPIOx = GPIOx;
	pPWM->cfg.GPIO_Pin = GPIO_Pin;
	pPWM->cfg.polarity = polarity;
	enInterrupts();
	return ERROR_OK;
}


FctERR NONNULL__ logPWM_setFreq(logicPWM * const pPWM, TIM_HandleTypeDef * const pTim, const uint16_t freq, uint16_t granularity)
{
	uint16_t	tim_freq;
	FctERR		err = ERROR_OK;

	assert_param(IS_TIM_INSTANCE(pTim->Instance));

	granularity = max(10, granularity);

	// TODO: set some limits for frequency (regarding granularity?)

	if (	(pTim->Instance->DIER & TIM_IT_UPDATE)
		&&	(pTim->Instance->CR1 & TIM_CR1_CEN))
	{	// Timer already started (try to set period according to already configured timer module)
		tim_freq = get_TIM_clock(pTim) / ((pTim->Init.Period + 1) * (pTim->Init.Prescaler + 1));
		if ((tim_freq / granularity) < freq)	{ return ERROR_VALUE; }
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

	return ERROR_OK;
}


FctERR NONNULL__ logPWM_setDuty(logicPWM * const pPWM, const uint16_t val)
{
	uint16_t duty;

	if (val == (uint16_t) -1)	{ duty = pPWM->cfg.per; }
	else if (val == 0)			{ duty = 0; }
	else						{ duty = (uint16_t) max(0, ((((val + 1) * (pPWM->cfg.per)) / 65536) - 1)); }

	diInterrupts();
	pPWM->cfg.duty = duty;
	enInterrupts();

	return ERROR_OK;
}


FctERR NONNULL__ logPWM_getFreq(uint16_t * const freq, const logicPWM * const pPWM)
{
	*freq = pPWM->cfg.tim_freq / pPWM->cfg.per;
	return ERROR_OK;
}


FctERR NONNULL__ logPWM_getDutyCycle(float * const duty, const logicPWM * const pPWM)
{
	*duty = (pPWM->cfg.duty * 100) / (float) pPWM->cfg.per;
	return ERROR_OK;
}


void NONNULL__ logPWM_handler(logicPWM * const pPWM)
{
	assert_param(IS_GPIO_PIN(pPWM->cfg.GPIO_Pin));

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
