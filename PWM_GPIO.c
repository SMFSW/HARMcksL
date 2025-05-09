/*!\file PWM_GPIO.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief GPIO PWM emulation handling
** \details \ref PWM_GPIO configuration:
** TIM with enabled period callback interrupt shall be used (as time base for PWM emulation)
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"
#include "PWM_GPIO.h"

#if defined(HAL_GPIO_MODULE_ENABLED) && defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


FctERR NONNULL__ PWM_GPIO_setPin(PWM_GPIO * const pPWM, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const bool polarity)
{
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	diInterrupts();
	pPWM->cfg.GPIOx = GPIOx;
	pPWM->cfg.GPIO_Pin = GPIO_Pin;
	pPWM->cfg.polarity = polarity;
	enInterrupts();
	return ERROR_OK;
}


FctERR NONNULL__ PWM_GPIO_setFreq(PWM_GPIO * const pPWM, TIM_HandleTypeDef * const pTim, const uint16_t freq, uint16_t granularity)
{
	uint16_t tim_freq;

	granularity = max(10, granularity);

	// TODO: set some limits for frequency (regarding granularity?)

	if ((pTim->Instance->DIER & TIM_IT_UPDATE) && (pTim->Instance->CR1 & TIM_CR1_CEN))
	{	// Timer already started (try to set period according to already configured timer module)
		tim_freq = RCC_TIMCLKFreq(pTim) / ((pTim->Init.Period + 1UL) * (pTim->Init.Prescaler + 1UL));
		if ((tim_freq / granularity) < freq)	{ return ERROR_VALUE; }
		granularity = tim_freq / freq;
	}
	else
	{
		tim_freq = freq * granularity;
		const HAL_StatusTypeDef st = init_TIM_Base(pTim, tim_freq);
		if (st != HAL_OK)	{ return HALERRtoFCTERR(st); }
	}

	diInterrupts();
	pPWM->cfg.tim_freq = tim_freq;
	pPWM->cfg.per = granularity;
	pPWM->cfg.pTim = pTim;
	enInterrupts();

	return ERROR_OK;
}


FctERR NONNULL__ PWM_GPIO_setDuty(PWM_GPIO * const pPWM, const uint16_t val)
{
	uint16_t duty;

	if (val == (uint16_t) -1)	{ duty = pPWM->cfg.per; }
	else if (val == 0U)			{ duty = 0U; }
	else						{ duty = (uint16_t) max(0U, ((((val + 1U) * (pPWM->cfg.per)) / 65536U) - 1U)); }

	diInterrupts();
	pPWM->cfg.duty = duty;
	enInterrupts();

	return ERROR_OK;
}


void NONNULL__ PWM_GPIO_handler(PWM_GPIO * const pPWM)
{
	if (pPWM->init && (pPWM->cfg.GPIOx != NULL))
	{
		if (!pPWM->cntr)					// period over
		{
			pPWM->cntr = pPWM->cfg.per;		// Reload new period information
			pPWM->duty = pPWM->cfg.duty;	// Load new duty cycle

			// if duty has been reached (real 0-100% duty cycles)
			if (pPWM->cntr == pPWM->duty)
			{
				HAL_GPIO_WritePin(pPWM->cfg.GPIOx, pPWM->cfg.GPIO_Pin, pPWM->cfg.polarity ? GPIO_PIN_SET : GPIO_PIN_RESET);	// switch channel on
			}
			else
			{
				HAL_GPIO_WritePin(pPWM->cfg.GPIOx, pPWM->cfg.GPIO_Pin, pPWM->cfg.polarity ? GPIO_PIN_RESET : GPIO_PIN_SET);	// switch channel off
			}
		}
		else
		{
			// if duty has been reached (duty cycle)
			if (pPWM->cntr == pPWM->duty)
			{
				HAL_GPIO_WritePin(pPWM->cfg.GPIOx, pPWM->cfg.GPIO_Pin, pPWM->cfg.polarity ? GPIO_PIN_SET : GPIO_PIN_RESET);	// switch channel on
			}
		}

		pPWM->cntr--;	// decrease channel counter
	}
}


/*******************************************************************************/
#endif	/* defined(HAL_TIM_MODULE_ENABLED) && defined(HAL_GPIO_MODULE_ENABLED) */
/*******************************************************************************/
