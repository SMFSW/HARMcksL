/*!\file GPIO_out.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief GPIO output handling
** \note Define GPIO_OUT_IT symbol at project level to use GPIO_out from timer interrupts (for more timing precision if required)
** \note When using GPIO_out from interrupts, GPIO_OUT_IT_PER period is defined by default with a period of 1000µs (can be customly defined)
**/
/****************************************************************/
#include "GPIO_out.h"
/****************************************************************/

#ifdef GPIO_OUT_IT
#ifndef GPIO_OUT_IT_PER
#define GPIO_OUT_IT_PER		1000	//!< GPIO_OUT_IT_PER can be used to define TIM interrupt period in microseconds
//!\note Define GPIO_OUT_IT_PER at project level to define custom TIM interrupt period (default set to 1000µs (most likely Systick))
#endif
#endif


/*!\brief Set GPIO port value
** \param[in,out] out - GPIO_out instance
**/
static void NONNULL__ GPIO_setter(GPIO_out * const out)
{
	if (out->action == Reset)		{ out->currentState = Reset; }
	else if (out->action == Set)	{ out->currentState = Set; }
	else if (out->action == Toggle)	{ out->currentState ^= 1; }
	else							{ return; }

	const GPIO_PinState val = out->currentState ^ (out->cfg.polarity ? 0 : 1);

	HAL_GPIO_WritePin(out->cfg.GPIOx, out->cfg.GPIO_Pin, val);
}


void NONNULLX__(1, 2) GPIO_out_init(GPIO_out * const out, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState polarity)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	out->cfg.GPIOx = GPIOx;
	out->cfg.GPIO_Pin = GPIO_Pin;
	out->cfg.polarity = polarity;

	out->currentState = Reset;
	out->memState = Reset;
	out->action = Reset;
	out->idle = true;
	out->init = true;

	GPIO_setter(out);		// Immediately set to reset value
}


FctERR NONNULL__ GPIO_out_Abort(GPIO_out * const out)
{
	FctERR err = ERROR_OK;

	if ((out->init) && (!out->idle))
	{
		err = GPIO_out_SetStatic(out, out->memState, 0);
	}

	return err;
}


/*!\brief Start GPIO_out change
** \param[in,out] out - GPIO_out instance
** \param[in] mode - GPIO_out mode
** \param[in] action - action to perform on output port pin for active state (Reset/Set/Toggle)
** \param[in] delay - delay before pulse (in ms)
** \param[in] active - active time (in ms) for pulse/blink mode only
** \param[in] inactive - inactive time (in ms) for blink mode only
** \param[in] count - blink count (0 for infinite) for blink mode only
** \return Error code
**/
static FctERR NONNULL__ GPIO_out_Start(	GPIO_out * const out, const eGPIO_out_mode mode, const eGPIOState action,
										uint32_t delay, uint32_t active, uint32_t inactive, const uint32_t count)
{
	if (!out->init)			{ return ERROR_INSTANCE; }
	if (action > Toggle)	{ return ERROR_VALUE; }

	#ifdef GPIO_OUT_IT
	delay = (delay * GPIO_OUT_IT_PER) / 1000;
	active = (active * GPIO_OUT_IT_PER) / 1000;
	inactive = (inactive * GPIO_OUT_IT_PER) / 1000;

	out->hOut = 0;

	diInterrupts();
	#else
	out->hOut = HALTicks();
	#endif

	out->mode = mode;
	out->action = action;
	out->delay = delay;
	out->timeActive = active;
	out->timeInactive = inactive;
	out->infinite = count ? false : true;
	out->cnt = count;

	out->start = true;
	out->active = true;
	out->idle = false;

	#ifdef GPIO_OUT_IT
	enInterrupts();
	#endif

	return ERROR_OK;
}

FctERR NONNULL__ GPIO_out_SetStatic(GPIO_out * const out, const eGPIOState action, const uint32_t delay) {
	return GPIO_out_Start(out, GPIO_out_Static, action, delay, 0, 0, 0); }

FctERR NONNULL__ GPIO_out_StartPulse(GPIO_out * const out, const eGPIOState action, const uint32_t delay, const uint32_t active) {
	return GPIO_out_Start(out, GPIO_out_Pulse, action, delay, active, 0, 0); }

FctERR NONNULL__ GPIO_out_StartBlink(	GPIO_out * const out, const eGPIOState action,
										const uint32_t delay, const uint32_t active, const uint32_t inactive, const uint32_t count) {
	return GPIO_out_Start(out, GPIO_out_Blink, action, delay, active, inactive, count); }


void NONNULL__ GPIO_out_handler(GPIO_out * const out)
{
	if ((out->init) && (!out->idle))
	{
		#ifdef GPIO_OUT_IT
		out->hOut++;	// Increment timer count

		if (	(!out->delay)
			||	(out->delay && (out->hOut > out->delay)))
		#else
		if (	(!out->delay)
			||	(out->delay && (TPSSUP_MS(out->hOut, out->delay))))
		#endif
		{
			out->delay = 0;

			switch (out->mode)
			{
				case GPIO_out_Pulse:
				{
					if (out->start)
					{
						GPIO_setter(out);
						out->start = false;
						#ifdef GPIO_OUT_IT
						out->hOut = 0;
						#else
						out->hOut = HALTicks();
						#endif
					}

					#ifdef GPIO_OUT_IT
					if (out->hOut > out->timeActive)
					#else
					if (TPSSUP_MS(out->hOut, out->timeActive))
					#endif
					{
						GPIO_out_Abort(out);
						goto stop;	// Go back to memorized state in current cycle
					}
				}
				break;

				case GPIO_out_Blink:
				{
					bool set = false;

					if (out->start)
					{
						GPIO_setter(out);
						out->action = Toggle;	// Force toggle if action set was Reset or Set
						out->start = false;
						#ifdef GPIO_OUT_IT
						out->hOut = 0;
						#else
						out->hOut = HALTicks();
						#endif
					}

					#ifdef GPIO_OUT_IT
					if ((out->active) && (out->hOut > out->timeActive))
					#else
					if ((out->active) && (TPSSUP_MS(out->hOut, out->timeActive)))
					#endif
					{
						set = true;
					}
					#ifdef GPIO_OUT_IT
					else if ((!out->active) && (out->hOut > out->timeInactive))
					#else
					else if ((!out->active) && (TPSSUP_MS(out->hOut, out->timeInactive)))
					#endif
					{
						if (--out->cnt == 0)
						{
							GPIO_out_Abort(out);
							goto stop;	// Go back to memorized state in current cycle
						}

						set = true;
					}

					if (set)
					{
						GPIO_setter(out);
						out->active ^= true;
						#ifdef GPIO_OUT_IT
						out->hOut = 0;
						#else
						out->hOut = HALTicks();
						#endif
					}
				}
				break;

				case GPIO_out_Static:
				{
					stop:		// label used only by Pulse and Blink cases when finished to revert to memorized state in current cycle
					GPIO_setter(out);
					out->memState = out->currentState;
					out->idle = true;
				}
				break;

				default:
					break;
			}
		}
	}
}
