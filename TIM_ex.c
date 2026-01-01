/*!\file TIM_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Extensions for TIM peripherals
** \warning Init functions assume that TIM peripherals instance were already configured by HAL
** \warning Shall work for all STM32 C/F/G/H/L/U families only (yet)
** \note \c TIM_MIN_GRANULARITY can defined at project level to tweak to needed minimum granularity
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


#ifndef TIM_MIN_GRANULARITY
#define	TIM_MIN_GRANULARITY	255UL	//!< Minimum granularity for TIM channel
//!\note \c TIM_MIN_GRANULARITY can defined at project level to tweak to needed minimum granularity
#endif


uint32_t NONNULL__ RCC_TIMCLKFreq(const TIM_HandleTypeDef * const pTim)
{
	uint32_t refCLK;

	#if defined(STM32C0) || defined(STM32G0) || defined(STM32U0)
		refCLK = HAL_RCC_GetPCLK1Freq();
		if ((RCC->CFGR & RCC_CFGR_PPRE) != 0U)	{ refCLK *= 2UL; }
	#elif defined(STM32F0)
		refCLK = HAL_RCC_GetPCLK1Freq();
	#else
		bool notAPB1;

		switch ((uint32_t) pTim->Instance)
		{
			default:
			{
				notAPB1 = false;
			}
			break;

			#if defined(TIM1)
			case TIM1_BASE:
			#endif
			#if defined(TIM8)
			case TIM8_BASE:
			#endif
			#if defined(TIM9)
			case TIM9_BASE:
			#endif
			#if defined(TIM10)
			case TIM10_BASE:
			#endif
			#if defined(TIM11)
			case TIM11_BASE:
			#endif
			#if defined(TIM15)
			case TIM15_BASE:
			#endif
			#if defined(TIM16)
			case TIM16_BASE:
			#endif
			#if defined(TIM17)
			case TIM17_BASE:
			#endif
			#if defined(TIM21)
			case TIM21_BASE:		//!< L0 family (TIM on APB2)
			#endif
			#if defined(TIM22)
			case TIM22_BASE:		//!< L0 family (TIM on APB2)
			#endif
			{
				notAPB1 = true;		// cppcheck-suppress unreachableCode ; successive #if defined() leads to a case where none are defined for cppcheck
			}
			break;
		}

		if (notAPB1)
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
					refCLK = HAL_RCC_GetHCLKFreq() * 2UL;
				}
				else
			#endif
			{
				// Get APB2 (PCLK2) frequency
				refCLK = HAL_RCC_GetPCLK2Freq();

				#if defined(STM32H7)
				if ((RCC->D2CFGR & RCC_D2CFGR_D2PPRE2) != 0U)
				#elif defined(STM32U3) || defined(STM32U5)
				if ((RCC->CFGR2 & RCC_CFGR2_PPRE2) != 0U)
				#else
				if ((RCC->CFGR & RCC_CFGR_PPRE2) != 0U)
				#endif
				{
					refCLK *= 2UL;
				}
			}
		}
		else
		{
			// Get APB1 (PCLK1) frequency
			refCLK = HAL_RCC_GetPCLK1Freq();

			#if defined(STM32H7)
			if ((RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) != 0U)
			#elif defined(STM32U3) || defined(STM32U5)
			if ((RCC->CFGR2 & RCC_CFGR2_PPRE1) != 0U)
			#else
			if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0U)
			#endif
			{
				refCLK *= 2UL;
			}
		}
	#endif

	return refCLK;
}


HAL_StatusTypeDef NONNULL__ write_TIM_Preload(TIM_HandleTypeDef * const pTim, const uint32_t chan)
{
	HAL_StatusTypeDef st = HAL_OK;

	switch (chan)	/* Set the Preload enable bit for channel */
	{
		default:
			st = HAL_ERROR;
			break;

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

		#if defined(TIM_CHANNEL_5)
		case TIM_CHANNEL_5:
			pTim->Instance->CCMR3 |= TIM_CCMR3_OC5PE;
			break;
		#endif

		#if defined(TIM_CHANNEL_6)
		case TIM_CHANNEL_6:
			pTim->Instance->CCMR3 |= TIM_CCMR3_OC6PE;
			break;
		#endif
	}

	return st;
}


HAL_StatusTypeDef NONNULL__ write_TIM_CCR(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint32_t CCR_val)
{
	__IO uint32_t * pCCR;

	assert_param(IS_TIM_INSTANCE(pTim->Instance));
	assert_param(IS_TIM_CCX_INSTANCE(pTim->Instance, chan));

	if (chan <= TIM_CHANNEL_4)			{ pCCR = &pTim->Instance->CCR1 + (chan / 4UL); }
	#if defined(TIM_CHANNEL_6)
		else if (chan <= TIM_CHANNEL_6)	{ pCCR = &pTim->Instance->CCR5 + (chan / 4UL) - 4UL; }
	#endif
	else 								{ return HAL_ERROR; }

	*pCCR = CCR_val;

	return HAL_OK;
}


HAL_StatusTypeDef NONNULL__ init_TIM_Base(TIM_HandleTypeDef * const pTim, const uint32_t freq)
{
	HAL_StatusTypeDef err;

	err = set_TIM_Interrupts(pTim, Off);	// Stop interrupts if they were already started
	err |= set_TIM_Freq(pTim, freq);		// Configure TIM frequency
	if (err)	{ return err; }
	return set_TIM_Interrupts(pTim, On);	// Start interrupts
}


HAL_StatusTypeDef NONNULL__ set_TIM_Freq(TIM_HandleTypeDef * const pTim, const uint32_t freq)
{
	HAL_StatusTypeDef st = HAL_ERROR;

	if (freq != 0UL)	// Avoid div by 0
	{
		const uint32_t max_prescaler = 0xFFFFU;		// Prescaler is 16b no redeeming timer instance
		uint32_t max_period = 0xFFFFUL;				// Max period for 16b timers (most common)
		uint64_t period;							// For 32b timers, period needs to be 64b large for calculations
		uint32_t prescaler;

		// TODO: init freq tests for 32b timers (init time may take some time)
		if (	(pTim->Instance == TIM2)
		#if defined(TIM5)
			||	(pTim->Instance == TIM5)
		#endif
			)	{ max_period = 0xFFFFFFFFUL; }								// Max period for 32b timers

		const uint32_t refCLK = RCC_TIMCLKFreq(pTim);
		if (freq > (refCLK / TIM_MIN_GRANULARITY))	{ return HAL_ERROR; }	// To guarantee minimum step range

		for (prescaler = 0UL ; prescaler <= max_prescaler ; prescaler++)
		{
			period = refCLK;
			period /= (uint64_t) freq * (prescaler + 1UL);
			period -= 1UL;
			if (period <= max_period)				{ break; }				// If in range
		}

		if (prescaler == (max_prescaler + 1UL))		{ return HAL_ERROR; }	// If nothing has been found (after last iteration)

		pTim->Init.Period = period;
		pTim->Init.Prescaler = prescaler;

		st = HAL_TIM_Base_Init(pTim);
	}

	return st;
}


HAL_StatusTypeDef NONNULL__ set_TIM_Tick_Freq(TIM_HandleTypeDef * const pTim, const uint32_t freq)
{
	if (!freq)	{ return HAL_ERROR; }		// Avoid div by 0

	if (	(pTim->Instance == TIM2)
	#if defined(TIM5)
		||	(pTim->Instance == TIM5)
	#endif
		)	{ pTim->Init.Period = 0xFFFFFFFFUL; }						// Set to max period for 32b timers
	else	{ pTim->Init.Period = 0xFFFFUL; }							// Set to max period for 16b timers

	pTim->Init.Prescaler = (RCC_TIMCLKFreq(pTim) / freq) - 1UL;			// Get prescaler value adjusted to desired frequency

	return HAL_TIM_Base_Init(pTim);
}


/****************************************************************/
#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
/****************************************************************/
