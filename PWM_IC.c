/*!\file PWM_IC.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief PWM Input Capture handling
** \details PWM_In configuration:
** TIM with multiple channels with slave mode capability shall be used
**
** Slave Mode: Reset Mode
** Trigger: TI2FP2 (?? depending channels ??)
** Channel1: Input Capture indirect mode
** Channel2: Input Capture direct mode
**
** Prescaler & CLK div: 0
** Counter Period: max possible value
** Counter Mode: Up
** Auto-Reload Preload: Disabled
** Trigger Output parameters: Disabled
** Input Capture Channel1: Falling Edge, Indirect, No CLK div
** Input Capture Channel2: Rising Edge, Direct, No CLK div
**
** GPIO configuration: Alternate Function Push Pull ; Pull Up ; High Speed
**
** NVIC configuration: Enable interrupt(s)
**
** \warning Input Capture limitation:
** 			- Lower/Higher frequency/duty cycle measurable totally depends on TIM configuration and clocks used.
** 			- As capture is based on interrupts, if signal becomes continuous (pin held low or high), last PWM value with edges will remain as current measure
** 				- It may not be a big issue if PWM duty cycle is always changing gradually, but will be in case of erratic changes (when becoming continous signal)
** 				- Thus, to overcome this issue, PWM signal shall always have edges to keep measures working and consistent
** \note Define NB_PWM_IC symbol with number of inputs at project level to use PWM_IC functionalities
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"
#include "PWM_IC.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/

#if NB_PWM_IC
PWM_IC PWMin[NB_PWM_IC] = { 0 };
#else
PWM_IC PWMin[NB_PWM_IC];
#endif


FctERR init_PWM_IC(PWM_IC * const pPWM_IC, TIM_HandleTypeDef * const pTim, const uint32_t Direct_Channel, const uint32_t Indirect_Channel, const uint32_t Scale)
{
	FctERR err = ERROR_OK;

	assert_param(IS_TIM_INSTANCE(pTim->Instance));
	assert_param(IS_TIM_CCX_INSTANCE(pTim->Instance, Direct_Channel));
	assert_param(IS_TIM_CCX_INSTANCE(pTim->Instance, Indirect_Channel));

	switch (Direct_Channel)
	{
		default:
			return ERROR_VALUE;
			break;

		case TIM_CHANNEL_1:
			pPWM_IC->cfg.ActiveChan = HAL_TIM_ACTIVE_CHANNEL_1;
			break;

		case TIM_CHANNEL_2:
			pPWM_IC->cfg.ActiveChan = HAL_TIM_ACTIVE_CHANNEL_2;
			break;

		case TIM_CHANNEL_3:
			pPWM_IC->cfg.ActiveChan = HAL_TIM_ACTIVE_CHANNEL_3;
			break;

		case TIM_CHANNEL_4:
			pPWM_IC->cfg.ActiveChan = HAL_TIM_ACTIVE_CHANNEL_4;
			break;

		#if defined(TIM_CHANNEL_5)
		case TIM_CHANNEL_5:
			pPWM_IC->cfg.ActiveChan = HAL_TIM_ACTIVE_CHANNEL_5;
			break;
		#endif

		#if defined(TIM_CHANNEL_6)
		case TIM_CHANNEL_6:
			pPWM_IC->cfg.ActiveChan = HAL_TIM_ACTIVE_CHANNEL_6;
			break;
		#endif
	}

	pPWM_IC->cfg.htim = pTim;
	pPWM_IC->cfg.Direct_Pin = Direct_Channel;
	pPWM_IC->cfg.Indirect_Pin = Indirect_Channel;

	pPWM_IC->cfg.refCLK = get_TIM_clock(pTim);	// Get TIM reference clock
	pPWM_IC->cfg.Scale = Scale;

	// Start PWM pin input capture interrupts
	err = HALERRtoFCTERR(HAL_TIM_IC_Start_IT(pTim, Indirect_Channel));
	if (err) { return err; }
	err = HALERRtoFCTERR(HAL_TIM_IC_Start_IT(pTim, Direct_Channel));

	return err;
}


/*!\brief Input Capture callback in non blocking mode
** \strong Strong implementation of HAL_TIM_IC_CaptureCallback in the project
** \param[in] htim - TIM IC handle
**/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim)
{
	for (PWM_IC * pIC = PWMin ; pIC < &PWMin[NB_PWM_IC] ; pIC++)
	{
		if ((pIC->cfg.htim) && (htim == pIC->cfg.htim))
		{
			if (htim->Channel == pIC->cfg.ActiveChan)
			{
				const uint32_t PWMin_DirectValue = HAL_TIM_ReadCapturedValue(pIC->cfg.htim, pIC->cfg.Direct_Pin);

				if (PWMin_DirectValue)
				{
					const uint32_t PWMin_IndirectValue = HAL_TIM_ReadCapturedValue(pIC->cfg.htim, pIC->cfg.Indirect_Pin);
					pIC->DutyCycle = (PWMin_IndirectValue * pIC->cfg.Scale) / PWMin_DirectValue;	// Duty cycle computation
					pIC->Frequency = pIC->cfg.refCLK / PWMin_DirectValue;							// Frequency computation
				}
				else
				{
					pIC->DutyCycle = pIC->Frequency = 0;
				}
			}
		}
	}
}


/****************************************************************/
#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
/****************************************************************/
