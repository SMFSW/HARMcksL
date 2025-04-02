/*!\file PWM_IC.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief PWM Input Capture handling
** \details PWM_In configuration:
** TIM with multiple channels with slave mode capability shall be used
**
** Slave Mode: Reset Mode
** Trigger: TI1FP1/TI2FP2 (depending physical MCU pin used tied to channel 1 or 2)
** Channel tied to pin: Input Capture direct mode
** Channel (2nd one): Input Capture indirect mode
**
** Prescaler & CLK div: 0, No CLK div
** Counter Period: max possible value
** Counter Mode: Up
** Auto-Reload Preload: Disabled
** Trigger Output parameters: Disabled
** Input Capture Channel tied to pin: Rising Edge, Direct, No CLK div
** Input Capture Channel (2nd one): Falling Edge, Indirect, No CLK div
**
** Instead, CubeMX Combined channels capture may be configured to PWM input capture, avoiding manual timer configuration
**
** GPIO configuration: Alternate Function Push Pull ; Pull Up (if needed) ; High Speed may be recommended
**
** NVIC configuration: Enable interrupt(s) if PWM_IC_NO_IT is not defined at project level
**
** \note Define NB_PWM_IC symbol with number of inputs at project level to use PWM_IC functionalities
** \note Define PWM_IC_NO_IT symbol at project level to disable TIM interrupts driven capture
** \warning Input Capture limitation:
** 			- Lower/Higher frequency/duty cycle measurable totally depends on TIM configuration and clocks used.
** 			- Continuous signal (pin held low or high) is detected automatically after timeout:
** 				- automatically when using interrupts driven PWM input capture
** 				- by calling \ref PWM_IC_get_Pin_State_Callback without interrupts
** 			- !!Please note there can be up to 1 second with startup timeout during which low to high continuous signal may be misinterpreted before retrieving values!!
** 			- If PWM signal to capture can never become continuous, it may save some time of servicing interrupts to disable them (with PWM_IC_NO_IT symbol)
**/
/****************************************************************/
#include "sarmfsw.h"
#include "TIM_ex.h"
#include "PWM_IC.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/

#if defined(PWM_IC_NO_IT)
#define HAL_TIM_IC_Starter	HAL_TIM_IC_Start
#else
#define HAL_TIM_IC_Starter	HAL_TIM_IC_Start_IT
#endif


PWM_IC PWMin[NB_PWM_IC];


__WEAK GPIO_PinState PWM_IC_get_Pin_State_Callback(const uint8_t idx)
{
	/**\code
	GPIO_TypeDef *	GPIOx;
	uint16_t		GPIO_Pin;

	... Set proper GPIOx and GPIO_Pin according to PWMin idx

	return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	\endcode**/

	UNUSED(idx);
	return GPIO_PIN_RESET;
}


/*!\brief Get PWM Input Capture Edge from Pin (in case of timeout)
** \param[in] pPWM_IC - pointer to PWM_IC instance
** \return Edge from pin state
**/
__STATIC_INLINE eEdge NONNULL_INLINE__ PWM_IC_get_Edge_From_Pin(const PWM_IC * const pPWM_IC) {
	return (PWM_IC_get_Pin_State_Callback(pPWM_IC - PWMin) == GPIO_PIN_SET) ? Rising : Falling; }

/*!\brief Get PWM Input Capture last update (in ms)
** \note Can be useful to ensure IC is still ongoing and results aren't outdated
** \param[in] pPWM_IC - pointer to PWM_IC instance
** \return Last PWM_IC Update (in ms)
**/
__STATIC_INLINE uint32_t NONNULL_INLINE__ get_PWM_IC_LastUpdate(const PWM_IC * const pPWM_IC) {
	return OVF_DIFF(HALTicks(), pPWM_IC->CallbackTick); }


FctERR NONNULL__ init_PWM_IC(PWM_IC * const pPWM_IC, TIM_HandleTypeDef * const pTim, const uint32_t Direct_Channel, const uint32_t Indirect_Channel, const uint32_t Scale)
{
	FctERR err = ERROR_OK;

	assert_param(IS_TIM_INSTANCE(pTim->Instance));

	memset(pPWM_IC, 0, sizeof(PWM_IC));

	#if !defined(PWM_IC_NO_IT)
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
	}
	#endif

	pPWM_IC->cfg.htim = pTim;
	pPWM_IC->cfg.Direct_Pin = Direct_Channel;
	pPWM_IC->cfg.Indirect_Pin = Indirect_Channel;

	pPWM_IC->cfg.refCLK = RCC_TIMCLKFreq(pTim);	// Get TIM reference clock
	pPWM_IC->cfg.Scale = Scale;

	pPWM_IC->Timeout = 1000;
	pPWM_IC->Last_Edge = PWM_IC_get_Edge_From_Pin(pPWM_IC);

	// Start PWM pin input capture
	err = HALERRtoFCTERR(HAL_TIM_IC_Starter(pTim, Indirect_Channel));
	if (err) { return err; }
	err = HALERRtoFCTERR(HAL_TIM_IC_Starter(pTim, Direct_Channel));

	return err;
}


/*!\brief Convert PWM Input Capturer values
** \param[in,out] pPWM_IC - pointer to PWM_IC instance
** \return Error code
**/
static FctERR NONNULL__ PWM_IC_convert(PWM_IC * const pPWM_IC)
{
	if ((pPWM_IC - PWMin) >= NB_PWM_IC)		{ return ERROR_INSTANCE; }

	const bool timeout = binEval(get_PWM_IC_LastUpdate(pPWM_IC) >= pPWM_IC->Timeout);
	if (timeout)	{ pPWM_IC->New_Sample = false; }	// Timeout reached, treat as logic signal (last samples are irrelevant)

	if (pPWM_IC->New_Sample)
	{
		pPWM_IC->New_Sample = false;

		if (	/*(pPWM_IC->Direct_cnt)								// Avoid div by 0
			&&*/(pPWM_IC->Direct_cnt >= pPWM_IC->Indirect_cnt))		// Valid values (PWM frequency is too low otherwise)
		{
			pPWM_IC->DutyCycle = ((pPWM_IC->Indirect_cnt * pPWM_IC->cfg.Scale) / pPWM_IC->Direct_cnt) + 1;	// Duty cycle computation
			pPWM_IC->Frequency = pPWM_IC->cfg.refCLK / pPWM_IC->Direct_cnt;									// Frequency computation

			pPWM_IC->Timeout = max(1, (1000 * 100) / pPWM_IC->Frequency);	// Timeout set to a hundred times current period
		}
		else
		{
			// Unmeasurable PWM
			pPWM_IC->Frequency = 0;
			pPWM_IC->DutyCycle = 0;
		}
	}
	else
	{
		// Handle as logic signal (when possible)
		#if defined(PWM_IC_NO_IT)
		pPWM_IC->Last_Edge = PWM_IC_get_Edge_From_Pin(pPWM_IC);
		#endif
		pPWM_IC->Frequency = 0;
		pPWM_IC->DutyCycle = (pPWM_IC->Last_Edge == Rising) ? pPWM_IC->cfg.Scale : 0;
	}

	return ERROR_OK;
}


uint32_t NONNULL__ get_PWM_IC_Freq(PWM_IC * const pPWM_IC)
{
	FctERR err = PWM_IC_convert(pPWM_IC);
	return err ? 0 : pPWM_IC->Frequency;
}


uint32_t NONNULL__ get_PWM_IC_DutyCycle(PWM_IC * const pPWM_IC)
{
	FctERR err = PWM_IC_convert(pPWM_IC);
	return err ? 0 : pPWM_IC->DutyCycle;
}


void PWM_IC_handler(void)
{
	for (PWM_IC * pIC = PWMin ; pIC < &PWMin[NB_PWM_IC] ; pIC++)
	{
		#if defined(PWM_IC_NO_IT)
		if (pIC->cfg.htim->Instance->SR & (TIM_SR_CC1IF | TIM_SR_CC2IF))
		{
			pIC->Direct_cnt = HAL_TIM_ReadCapturedValue(pIC->cfg.htim, pIC->cfg.Direct_Pin);
			pIC->Indirect_cnt = HAL_TIM_ReadCapturedValue(pIC->cfg.htim, pIC->cfg.Indirect_Pin);
			pIC->CallbackTick = HALTicks();	// Update tick
			pIC->New_Sample = true;
		}
		#endif

		PWM_IC_convert(pIC);
	}
}


#if !defined(PWM_IC_NO_IT)
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
				pIC->Last_Edge = Rising;
				pIC->Direct_cnt = HAL_TIM_ReadCapturedValue(pIC->cfg.htim, pIC->cfg.Direct_Pin);
				pIC->Indirect_cnt = HAL_TIM_ReadCapturedValue(pIC->cfg.htim, pIC->cfg.Indirect_Pin);
				pIC->CallbackTick = HALTicks();	// Update tick
				pIC->New_Sample = true;
			}
			else	// TODO: see if Channel shouldn't be checked too, otherwise another IC channel could trigger a falling edge
			{
				pIC->Last_Edge = Falling;
			}

			break;	// As long as TIM instance matches, no need to continue iterations
		}
	}
}
#endif


/****************************************************************/
#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
/****************************************************************/
