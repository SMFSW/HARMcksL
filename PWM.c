/*!\file PWM.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief Straightforward PWM handling
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"
#include "PWM.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


/*******************/
/*** PWM DRIVING ***/
/*******************/
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
	uint16_t tim_freq;

	assert_param(IS_TIM_INSTANCE(pTim->Instance));

	granularity = max(10, granularity);

	// TODO: set some limits for frequency (regarding granularity?)

	if ((pTim->Instance->DIER & TIM_IT_UPDATE) && (pTim->Instance->CR1 & TIM_CR1_CEN))
	{	// Timer already started (try to set period according to already configured timer module)
		tim_freq = get_TIM_clock(pTim) / ((pTim->Init.Period + 1) * (pTim->Init.Prescaler + 1));
		if ((tim_freq / granularity) < freq)	{ return ERROR_VALUE; }
		granularity = tim_freq / freq;
	}
	else
	{
		tim_freq = freq * granularity;
		HAL_StatusTypeDef st = init_TIM_Base(pTim, tim_freq);
		if (st)	{ return HALERRtoFCTERR(st); }
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
