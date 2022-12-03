/*!\file TIM_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2022, SMFSW
** \brief Extensions for TIM peripherals
** \warning Init functions assume that TIM peripherals instance were already configured by HAL
** \warning Shall work for all STM32 F/G/L families, H families not totally covered
** \warning LPTIM peripheral not handled yet
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


#ifndef TIM_MIN_GRANULARITY
#define	TIM_MIN_GRANULARITY	255		//!< Minimum granularity for TIM channel
//! \note TIM_MIN_GRANULARITY can defined at project level to tweak to needed minimum granularity
#endif


uint32_t NONNULL__ RCC_TIMCLKFreq(const TIM_HandleTypeDef * const pTim)
{
	// TODO: handle LPTIMx?

	uint32_t refCLK;

	#if defined(STM32G0)
		refCLK = HAL_RCC_GetPCLK1Freq();
		if ((RCC->CFGR & RCC_CFGR_PPRE) != 0)	{ refCLK *= 2; }
	#elif defined(STM32F0)
		refCLK = HAL_RCC_GetPCLK1Freq();
	#else
		bool notAPB1;

		switch ((uint32_t) pTim->Instance)
		{
			default:
				notAPB1 = false;
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
				notAPB1 = true;
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


HAL_StatusTypeDef NONNULL__ write_TIM_Preload(TIM_HandleTypeDef * const pTim, const uint32_t chan)
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

	return HAL_OK;
}


HAL_StatusTypeDef NONNULL__ write_TIM_CCR(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint32_t CCR_val)
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
	const uint32_t max_prescaler = 0xFFFF;	// Prescaler is 16b no redeeming timer instance
	uint32_t max_period = 0xFFFF;			// Max period for 16b timers (most common)
	uint64_t period;						// For 32b timers, period needs to be 64b large for calculations
	uint32_t prescaler;

	// TODO: init freq tests for 32b timers (init time may take some time)
	if (	(pTim->Instance == TIM2)
	#if defined(TIM5)
		||	(pTim->Instance == TIM5)
	#endif
		)	{ max_period = 0xFFFFFFFF; }								// Max period for 32b timers

	const uint32_t refCLK = RCC_TIMCLKFreq(pTim);
	if (freq > refCLK / TIM_MIN_GRANULARITY)	{ return HAL_ERROR; }	// To guarantee minimum step range

	for (prescaler = 0 ; prescaler <= max_prescaler ; prescaler++)
	{
		period = (refCLK / (freq * (prescaler + 1))) - 1;
		if (period <= max_period)				{ break; }				// If in range
	}

	if (prescaler == max_prescaler + 1)			{ return HAL_ERROR; }	// If nothing has been found (after last iteration)

	pTim->Init.Period = period;
	pTim->Init.Prescaler = prescaler;

	return HAL_TIM_Base_Init(pTim);
}


HAL_StatusTypeDef NONNULL__ set_TIM_Tick_Freq(TIM_HandleTypeDef * const pTim, const uint32_t freq)
{
	if (	(pTim->Instance == TIM2)
	#if defined(TIM5)
		||	(pTim->Instance == TIM5)
	#endif
		)	{ pTim->Init.Period = 0xFFFFFFFF; }							// Set to max period for 32b timers
	else	{ pTim->Init.Period = 0xFFFF; }								// Set to max period for 16b timers

	pTim->Init.Prescaler = (RCC_TIMCLKFreq(pTim) / freq) - 1;			// Get prescaler value adjusted to desired frequency

	return HAL_TIM_Base_Init(pTim);
}


/****************************************************************/
#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
/****************************************************************/
