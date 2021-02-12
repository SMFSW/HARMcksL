/*!\file GPIO_out.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief GPIO output handling
** \note Define GPIO_OUT_IT symbol at project level to use GPIO_out from timer interrupts (for more timing precision if required)
** \note When using GPIO_out from interrupts, GPIO_OUT_IT_PER period is defined by default with a period of 1000µs (can be customly defined)
**/
/****************************************************************/
#ifndef __GPIO_OUT_H
	#define __GPIO_OUT_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum eGPIO_out_mode
** \brief Activation state On, Off
**/
typedef enum eGPIO_out_mode {
	GPIO_out_Static = 0,	//!< Static mode
	GPIO_out_Pulse,			//!< Pulse mode (mono-stable)
	GPIO_out_Blink,			//!< Blink mode (bi-stable)
} eGPIO_out_mode;

/*!\struct GPIO_out
** \brief GPIO output structure
**/
typedef struct GPIO_out {
	__IO eGPIOState	currentState;	//!< Current GPIO state (Reset/Set)
	eGPIOState		memState;		//!< Memorized GPIO state (Reset/Set), used after Pulse or Blink stop
	eGPIO_out_mode	mode;			//!< Current GPIO mode
	uint32_t		cnt;			//!< Blinking counter
	uint32_t		delay;			//!< Delay before action
	uint32_t		timeActive;		//!< Active time (used for Pulse)
	uint32_t		timeInactive;	//!< Inactive time (used for Pulse and Blink)
	uint32_t		hOut;			//!< Start time of the current phase
	eGPIOState		action;			//!< Action to perform
	bool			infinite	:1;	//!< Infinite blinking (only if count received is 0 when starting Blink action)
	bool			start		:1;	//!< Action start step
	bool			active		:1;	//!< Pulse/Blink active phase
	bool			init		:1;	//!< Set to 1 when GPIO_out instance properly initialized
	__IO bool		idle		:1;	//!< Action state (idle / running)
	struct {
	GPIO_TypeDef *	GPIOx;			//!< HAL GPIO instance
	uint16_t		GPIO_Pin;		//!< HAL GPIO pin
	GPIO_PinState	polarity;		//!< Output polarity
	} cfg;
} GPIO_out;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get GPIO_out current output value
** \param[in] out - GPIO_out instance
** \return Pin state
**/
__INLINE eGPIOState NONNULL_INLINE__ get_GPIO_out_State(const GPIO_out * const out) {
	return out->currentState; }


/*!\brief Get GPIO_out current mode
** \param[in] out - GPIO_out instance
** \return Pin mode
**/
__INLINE eGPIO_out_mode NONNULL_INLINE__ get_GPIO_out_Mode(const GPIO_out * const out) {
	return out->mode; }


/*!\brief Get GPIO_out current status
** \param[in] out - GPIO_out instance
** \return Pin status
**/
__INLINE bool NONNULL_INLINE__ get_GPIO_out_Idle(const GPIO_out * const out) {
	return out->idle; }


/*!\brief Initialize GPIO_out instance
** \param[in,out] out - GPIO_out instance to initialize
** \param[in] GPIOx - port to write to
** \param[in] GPIO_Pin - pin to write to
** \param[in] polarity - set to \ref GPIO_PIN_RESET if active state is GND, \ref GPIO_PIN_SET if Vdd
**/
void NONNULLX__(1, 2) GPIO_out_init(GPIO_out * const out, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState polarity);


/*!\brief Stop GPIO_out pulse/blink if running, or abort set if delay not reached
** \param[in,out] out - GPIO_out instance
** \return Error code
**/
FctERR NONNULL__ GPIO_out_Abort(GPIO_out * const out);


/*!\brief Set GPIO_out state
** \param[in,out] out - GPIO_out instance
** \param[in] action - action to perform on output port pin (Reset/Set/Toggle)
** \param[in] delay - delay before writing to port (in ms)
** \return Error code
**/
FctERR NONNULL__ GPIO_out_SetStatic(GPIO_out * const out, const eGPIOState action, const uint32_t delay);

/*!\brief Set GPIO_out
** \param[in,out] out - GPIO_out instance
** \return Error Code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Set(GPIO_out * const out) {
	return GPIO_out_SetStatic(out, Set, 0); }

/*!\brief Set GPIO_out
** \param[in,out] out - GPIO_out instance
** \param[in] delay - delay before writing to port (in ms)
** \return Error Code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Set_Delayed(GPIO_out * const out, const uint32_t delay) {
	return GPIO_out_SetStatic(out, Set, delay); }

/*!\brief Reset GPIO_out
** \param[in,out] out - GPIO_out instance
** \return Error Code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Reset(GPIO_out * const out) {
	return GPIO_out_SetStatic(out, Reset, 0); }

/*!\brief Reset GPIO_out
** \param[in,out] out - GPIO_out instance
** \param[in] delay - delay before writing to port (in ms)
** \return Error Code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Reset_Delayed(GPIO_out * const out, const uint32_t delay) {
	return GPIO_out_SetStatic(out, Reset, delay); }

/*!\brief Start GPIO_out Pulse
** \param[in,out] out - GPIO_out instance
** \param[in] action - action to perform on output port pin for active state (Reset/Set/Toggle)
** \param[in] delay - delay before pulse (in ms)
** \param[in] active - pulse time (in ms)
** \return Error code
**/
FctERR NONNULL__ GPIO_out_StartPulse(GPIO_out * const out, const eGPIOState action, const uint32_t delay, const uint32_t active);

/*!\brief Start GPIO_out Blink
** \param[in,out] out - GPIO_out instance
** \param[in] action - action to perform on output port pin for active state (Reset/Set/Toggle)
** \param[in] delay - delay before pulse (in ms)
** \param[in] active - blink active time (in ms)
** \param[in] inactive - blink inactive time (in ms)
** \param[in] count - blink count (0 for infinite)
** \return Error code
**/
FctERR NONNULL__ GPIO_out_StartBlink(	GPIO_out * const out, const eGPIOState action, const uint32_t delay,
										const uint32_t active, const uint32_t inactive, const uint32_t count);


/*!\brief Handles GPIO_out treatment
** \param[in,out] out - GPIO_out instance to handle
**/
void NONNULL__ GPIO_out_handler(GPIO_out * const out);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __GPIO_OUT_H */
/****************************************************************/
