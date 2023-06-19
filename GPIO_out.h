/*!\file GPIO_out.h
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief GPIO output handling
** \note Define LOGIC_OUT_IT shared symbol at project level to use GPIO_out from timer interrupts (for more timing precision if required)
** \note When using GPIO_out from interrupts, shared LOGIC_OUT_IT_PER period is defined by default with a period of 1000µs (can be customly defined)
** \warning Logic_out & GPIO_out shares interrupt behavior, thus needs to be implemented the same way (it or loop) if both are used at the same time
**/
/****************************************************************/
#ifndef __GPIO_OUT_H
	#define __GPIO_OUT_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "Logic_out.h"

#if defined(HAL_GPIO_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
typedef Logic_out		GPIO_out;			//!< GPIO_out typedef alias of \ref Logic_out
typedef eLogic_out_mode	eGPIO_out_mode;		//!< eGPIO_out_mode typedef alias of \ref eLogic_out_mode


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get GPIO_out current output value
** \param[in] out - GPIO_out instance
** \return Pin state
**/
__INLINE eGPIOState NONNULL_INLINE__ get_GPIO_out_State(const GPIO_out * const out) {
	return get_Logic_out_State(out); }


/*!\brief Get GPIO_out current mode
** \param[in] out - GPIO_out instance
** \return Pin mode
**/
__INLINE eGPIO_out_mode NONNULL_INLINE__ get_GPIO_out_Mode(const GPIO_out * const out) {
	return get_Logic_out_Mode(out); }


/*!\brief Get GPIO_out current status
** \param[in] out - GPIO_out instance
** \return Pin status
**/
__INLINE bool NONNULL_INLINE__ get_GPIO_out_Idle(const GPIO_out * const out) {
	return get_Logic_out_Idle(out); }


/*!\brief Initialize GPIO_out instance
** \param[in,out] out - GPIO_out instance to initialize
** \param[in] GPIOx - port to write to
** \param[in] GPIO_Pin - pin to write to
** \param[in] polarity - set to \ref GPIO_PIN_RESET if active state is GND, \ref GPIO_PIN_SET if Vdd
** \return Error code
**/
FctERR NONNULL__ GPIO_out_init(GPIO_out * const out, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState polarity);


/*!\brief Stop GPIO_out pulse/blink if running, or abort set if delay not reached
** \param[in,out] out - GPIO_out instance
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Abort(GPIO_out * const out) {
	return Logic_out_Abort(out); }


/*!\brief Set GPIO_out state
** \param[in,out] out - GPIO_out instance
** \param[in] action - action to perform on output port pin (Reset/Set/Toggle)
** \param[in] delay - delay before writing to port (in ms)
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_SetStatic(GPIO_out * const out, const eGPIOState action, const uint32_t delay) {
	return Logic_out_SetStatic(out, action, delay); }

/*!\brief Set GPIO_out
** \param[in,out] out - GPIO_out instance
** \return Error Code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Set(GPIO_out * const out) {
	return Logic_out_SetStatic(out, Set, 0); }

/*!\brief Set GPIO_out
** \param[in,out] out - GPIO_out instance
** \param[in] delay - delay before writing to port (in ms)
** \return Error Code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Set_Delayed(GPIO_out * const out, const uint32_t delay) {
	return Logic_out_SetStatic(out, Set, delay); }

/*!\brief Reset GPIO_out
** \param[in,out] out - GPIO_out instance
** \return Error Code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Reset(GPIO_out * const out) {
	return Logic_out_SetStatic(out, Reset, 0); }

/*!\brief Reset GPIO_out
** \param[in,out] out - GPIO_out instance
** \param[in] delay - delay before writing to port (in ms)
** \return Error Code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_Reset_Delayed(GPIO_out * const out, const uint32_t delay) {
	return Logic_out_SetStatic(out, Reset, delay); }

/*!\brief Start GPIO_out Pulse
** \param[in,out] out - GPIO_out instance
** \param[in] action - action to perform on output port pin for active state (Reset/Set/Toggle)
** \param[in] delay - delay before pulse (in ms)
** \param[in] active - pulse time (in ms)
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_StartPulse(GPIO_out * const out, const eGPIOState action, const uint32_t delay, const uint32_t active) {
	return Logic_out_StartPulse(out, action, delay, active); }

/*!\brief Start GPIO_out Blink
** \param[in,out] out - GPIO_out instance
** \param[in] action - action to perform on output port pin for active state (Reset/Set/Toggle)
** \param[in] delay - delay before pulse (in ms)
** \param[in] active - blink active time (in ms)
** \param[in] inactive - blink inactive time (in ms)
** \param[in] count - blink count (0 for infinite)
** \return Error code
**/
__INLINE FctERR NONNULL_INLINE__ GPIO_out_StartBlink(	GPIO_out * const out, const eGPIOState action, const uint32_t delay,
														const uint32_t active, const uint32_t inactive, const uint32_t count) {
	return Logic_out_StartBlink(out, action, delay, active, inactive, count); }


/*!\brief Handles GPIO_out treatment
** \param[in,out] out - GPIO_out instance to handle
**/
__INLINE void NONNULL_INLINE__ GPIO_out_handler(GPIO_out * const out) {
	Logic_out_handler(out); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_GPIO_MODULE_ENABLED) */
#endif	/* __GPIO_OUT_H */
/****************************************************************/
