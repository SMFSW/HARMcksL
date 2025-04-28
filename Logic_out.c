/*!\file Logic_out.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief Logic output handling
** \note Define \c LOGIC_OUT_IT symbol at project level to use \ref Logic_out from timer interrupts (for more timing precision if required)
** \note When using \ref Logic_out from interrupts, \c LOGIC_OUT_IT_PER period is defined by default with a period of 1000µs (can be customly defined)
** \warning \ref Logic_out & \ref GPIO_out shares interrupt behavior, thus needs to be implemented the same way (it or loop) if both are used at the same time
**/
/****************************************************************/
#include "Logic_out.h"
/****************************************************************/


/*!\brief Logic output variable setter
** \param[in] out - Logic_out instance
** \param[in] state - Logic_out state
**/
__STATIC_INLINE void NONNULL_INLINE__ Logic_out_setter(const Logic_out * const out, const GPIO_PinState state)
{
	const uint32_t bit_mask = LSHIFT(1UL, out->cfg.LOG_Pos);
	if (out->cfg.LOGx != NULL)	{ SET_BITS_VAL(*((uint32_t *) out->cfg.LOGx), bit_mask, state ? bit_mask : 0UL); }
}


/*!\brief Set GPIO port value
** \param[in,out] out - Logic_out instance
** \return Error code
**/
FctERR NONNULL__ Logic_out_update(Logic_out * const out)
{
	if (out->action == Reset)		{ out->currentState = GPIO_PIN_RESET; }
	else if (out->action == Set)	{ out->currentState = GPIO_PIN_SET; }
	else if (out->action == Toggle)	{ out->currentState ^= GPIO_PIN_SET; }
	else							{ return ERROR_VALUE; }

	const GPIO_PinState val = out->currentState ^ (out->cfg.polarity ? GPIO_PIN_RESET : GPIO_PIN_SET);

	if (out->cfg.set != NULL)	{ out->cfg.set(out, val); }
	else						{ Logic_out_setter(out, val); }

	return ERROR_OK;
}


FctERR NONNULLX__(1) Logic_out_init(Logic_out * const out, void (*setter)(const Logic_out * const, const GPIO_PinState),
									uint32_t * const addr, const uint16_t pos, const GPIO_PinState polarity)
{
	/* Check the parameters */
	//assert_param(pos < 32);	// Would raise an assert error in case of GPIO pin

	out->cfg.set = setter;
	out->cfg.LOGx = addr;
	out->cfg.LOG_Pos = pos;
	out->cfg.polarity = polarity;

	out->currentState = GPIO_PIN_RESET;
	out->memState = GPIO_PIN_RESET;
	out->action = Reset;
	out->idle = true;
	out->init = true;

	return Logic_out_update(out);		// Immediately set to reset value
}


FctERR NONNULL__ Logic_out_Abort(Logic_out * const out)
{
	FctERR err = ERROR_OK;

	if ((out->init) && (!out->idle))
	{
		err = Logic_out_SetStatic(out, (eGPIOState) out->memState, 0);
	}

	return err;
}


/*!\brief Start Logic_out change
** \param[in,out] out - Logic_out instance
** \param[in] mode - Logic_out mode
** \param[in] action - action to perform on output port pin for active state (Reset/Set/Toggle)
** \param[in] delay - delay before pulse (in ms)
** \param[in] active - active time (in ms) for pulse/blink mode only
** \param[in] inactive - inactive time (in ms) for blink mode only
** \param[in] count - blink count (0 for infinite) for blink mode only
** \return Error code
**/
static FctERR NONNULL__ Logic_out_Start(Logic_out * const out, const eLogic_out_mode mode, const eGPIOState action,
										uint32_t delay, uint32_t active, uint32_t inactive, const uint32_t count)
{
	if (!out->init)			{ return ERROR_INSTANCE; }
	if (action > Toggle)	{ return ERROR_VALUE; }

	#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
	delay = (delay * LOGIC_OUT_IT_PER) / 1000UL;
	active = (active * LOGIC_OUT_IT_PER) / 1000UL;
	inactive = (inactive * LOGIC_OUT_IT_PER) / 1000UL;

	out->hOut = 0U;

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

	#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
	enInterrupts();
	#endif

	return ERROR_OK;
}

FctERR NONNULL__ Logic_out_SetStatic(Logic_out * const out, const eGPIOState action, const uint32_t delay) {
	return Logic_out_Start(out, outStatic, action, delay, 0, 0, 0); }

FctERR NONNULL__ Logic_out_StartPulse(Logic_out * const out, const eGPIOState action, const uint32_t delay, const uint32_t active) {
	return Logic_out_Start(out, outPulse, action, delay, active, 0, 0); }

FctERR NONNULL__ Logic_out_StartBlink(	Logic_out * const out, const eGPIOState action,
										const uint32_t delay, const uint32_t active, const uint32_t inactive, const uint32_t count) {
	return Logic_out_Start(out, outBlink, action, delay, active, inactive, count); }


void NONNULL__ Logic_out_handler(Logic_out * const out)
{
	if ((out->init) && (!out->idle))
	{
		#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
		out->hOut++;	// Increment timer count

		if (	(!out->delay)
			||	(out->delay && (out->hOut > out->delay)))
		#else
		if (	(!out->delay)
			||	(out->delay && (TPSSUP_MS(out->hOut, out->delay))))
		#endif
		{
			out->delay = 0U;

			switch (out->mode)
			{
				case outPulse:
				{
					if (out->start)
					{
						UNUSED_RET Logic_out_update(out);
						out->start = false;
						#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
						out->hOut = 0U;
						#else
						out->hOut = HALTicks();
						#endif
					}

					#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
					if (out->hOut > out->timeActive)
					#else
					if (TPSSUP_MS(out->hOut, out->timeActive))
					#endif
					{
						UNUSED_RET Logic_out_Abort(out);
						goto stop;	// Go back to memorized state in current cycle
					}
				}
				break;

				case outBlink:
				{
					bool set = false;

					if (out->start)
					{
						UNUSED_RET Logic_out_update(out);
						out->action = Toggle;	// Force toggle if action set was Reset or Set
						out->start = false;
						#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
						out->hOut = 0U;
						#else
						out->hOut = HALTicks();
						#endif
					}

					#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
					if ((out->active) && (out->hOut > out->timeActive))
					#else
					if ((out->active) && (TPSSUP_MS(out->hOut, out->timeActive)))
					#endif
					{
						set = true;
					}
					#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
					else if ((!out->active) && (out->hOut > out->timeInactive))
					#else
					else if ((!out->active) && (TPSSUP_MS(out->hOut, out->timeInactive)))
					#endif
					{
						if (--out->cnt == 0U)
						{
							UNUSED_RET Logic_out_Abort(out);
							goto stop;	// Go back to memorized state in current cycle
						}

						set = true;
					}

					if (set)
					{
						UNUSED_RET Logic_out_update(out);
						out->active ^= true;
						#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
						out->hOut = 0U;
						#else
						out->hOut = HALTicks();
						#endif
					}
				}
				break;

				case outStatic:
				{
					stop:		// label used only by Pulse and Blink cases when finished to revert to memorized state in current cycle
					UNUSED_RET Logic_out_update(out);
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

