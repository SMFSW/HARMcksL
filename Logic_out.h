/*!\file Logic_out.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Logic output handling
** \note Define \c LOGIC_OUT_IT symbol at project level to use \ref Logic_out from timer interrupts (for more timing precision if required)
** \note When using \ref Logic_out from interrupts, \c LOGIC_OUT_IT_PER period is defined by default with a period of 1000µs (can be customly defined)
** \warning \ref Logic_out & \ref GPIO_out shares interrupt behavior, thus needs to be implemented the same way (it or loop) if both are used at the same time
**/
/****************************************************************/
#ifndef LOGIC_OUT_H__
	#define LOGIC_OUT_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


#if defined(LOGIC_OUT_IT) || defined(GPIO_OUT_IT)
#if !defined(LOGIC_OUT_IT_PER) && !defined(GPIO_OUT_IT_PER)
#define LOGIC_OUT_IT_PER	1000UL	//!< LOGIC_OUT_IT_PER can be used to define TIM interrupt period in microseconds
//!\note Define LOGIC_OUT_IT_PER at project level to define custom TIM interrupt period (default set to 1000µs (most likely Systick))
#elif defined(GPIO_OUT_IT_PER)
#define LOGIC_OUT_IT_PER	GPIO_OUT_IT_PER
#endif
#endif


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum eLogic_out_mode
** \brief Activation state On, Off
**/
typedef enum {
	outStatic = 0U,	//!< Static mode
	outPulse,		//!< Pulse mode (mono-stable)
	outBlink,		//!< Blink mode (bi-stable)
} eLogic_out_mode;


/*!\struct Logic_out
** \brief Logic output structure
**/
typedef struct logic_out {
	__IOM GPIO_PinState	currentState;													//!< Current Logic output state (Reset/Set)
	GPIO_PinState		memState;														//!< Memorized Logic output state (Reset/Set), used after Pulse or Blink stop
	eLogic_out_mode		mode;															//!< Current Logic output mode
	uint32_t			cnt;															//!< Blinking counter
	uint32_t			delay;															//!< Delay before action
	uint32_t			timeActive;														//!< Active time (used for Pulse)
	uint32_t			timeInactive;													//!< Inactive time (used for Pulse and Blink)
	uint32_t			hOut;															//!< Start time of the current phase
	eGPIOState			action;															//!< Action to perform
	uintCPU_t			infinite	:1;													//!< Infinite blinking (only if count received is 0 when starting Blink action)
	uintCPU_t			start		:1;													//!< Action start step
	uintCPU_t			active		:1;													//!< Pulse/Blink active phase
	uintCPU_t			init		:1;													//!< Set to 1 when GPIO_out instance properly initialized
	__IOM uintCPU_t		idle		:1;													//!< Action state (idle / running)
	struct {
	void				(*set)(const struct logic_out * const, const GPIO_PinState);	//!< Setter function
	void *				LOGx;															//!< Logic output address
	uint16_t			LOG_Pos;														//!< Logic output bit position
	GPIO_PinState		polarity;														//!< Logic output polarity
	} cfg;
} Logic_out;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get Logic_out current output value
** \param[in] out - Logic_out instance
** \return Pin state
**/
__INLINE eGPIOState NONNULL_INLINE__ get_Logic_out_State(const Logic_out * const out) {
	return (eGPIOState) out->currentState; }


/*!\brief Get Logic_out current mode
** \param[in] out - Logic_out instance
** \return Pin mode
**/
__INLINE eLogic_out_mode NONNULL_INLINE__ get_Logic_out_Mode(const Logic_out * const out) {
	return out->mode; }


/*!\brief Get Logic_out current status
** \param[in] out - Logic_out instance
** \return Pin status
**/
__INLINE bool NONNULL_INLINE__ get_Logic_out_Idle(const Logic_out * const out) {
	return out->idle; }


/*!\brief Initialize Logic_out instance
** \param[in,out] out - Logic_out instance to initialize
** \param[in] setter - Logic_out setter function pointer (may be NULL: default behavior for handling RAM variable at address \b addr)
** \param[in] addr - Variable address to write to (pointer to unsigned 32b, may be NULL if setter handles everything)
** \param[in] pos - Bit position in variable (may be unused if setter function is used)
** \param[in] polarity - set to \c GPIO_PIN_RESET if active state is low, \c GPIO_PIN_SET if high
** \return FctERR - Error code
**/
FctERR NONNULLX__(1) Logic_out_init(Logic_out * const out, void (*setter)(const Logic_out * const, const GPIO_PinState),
									uint32_t * const addr, const uint16_t pos, const GPIO_PinState polarity);


/*!\brief Stop Logic_out pulse/blink if running, or abort set if delay not reached
** \param[in,out] out - Logic_out instance
** \return FctERR - Error code
**/
FctERR NONNULL__ Logic_out_Abort(Logic_out * const out);


/*!\brief Set Logic_out state
** \param[in,out] out - Logic_out instance
** \param[in] action - action to perform on output port pin (Reset/Set/Toggle)
** \param[in] delay - delay before writing to port (in ms)
** \return FctERR - Error code
**/
FctERR NONNULL__ Logic_out_SetStatic(Logic_out * const out, const eGPIOState action, const uint32_t delay);

/*!\brief Set Logic_out
** \param[in,out] out - Logic_out instance
** \return FctERR - Error Code
**/
__INLINE FctERR NONNULL_INLINE__ Logic_out_Set(Logic_out * const out) {
	return Logic_out_SetStatic(out, Set, 0); }

/*!\brief Set Logic_out
** \param[in,out] out - Logic_out instance
** \param[in] delay - delay before writing to port (in ms)
** \return FctERR - Error Code
**/
__INLINE FctERR NONNULL_INLINE__ Logic_out_Set_Delayed(Logic_out * const out, const uint32_t delay) {
	return Logic_out_SetStatic(out, Set, delay); }

/*!\brief Reset Logic_out
** \param[in,out] out - Logic_out instance
** \return FctERR - Error Code
**/
__INLINE FctERR NONNULL_INLINE__ Logic_out_Reset(Logic_out * const out) {
	return Logic_out_SetStatic(out, Reset, 0); }

/*!\brief Reset Logic_out
** \param[in,out] out - Logic_out instance
** \param[in] delay - delay before writing to port (in ms)
** \return FctERR - Error Code
**/
__INLINE FctERR NONNULL_INLINE__ Logic_out_Reset_Delayed(Logic_out * const out, const uint32_t delay) {
	return Logic_out_SetStatic(out, Reset, delay); }

/*!\brief Start Logic_out Pulse
** \param[in,out] out - Logic_out instance
** \param[in] action - action to perform on output port pin for active state (Reset/Set/Toggle)
** \param[in] delay - delay before pulse (in ms)
** \param[in] active - pulse time (in ms)
** \return FctERR - Error code
**/
FctERR NONNULL__ Logic_out_StartPulse(Logic_out * const out, const eGPIOState action, const uint32_t delay, const uint32_t active);

/*!\brief Start Logic_out Blink
** \param[in,out] out - Logic_out instance
** \param[in] action - action to perform on output port pin for active state (Reset/Set/Toggle)
** \param[in] delay - delay before pulse (in ms)
** \param[in] active - blink active time (in ms)
** \param[in] inactive - blink inactive time (in ms)
** \param[in] count - blink count (0 for infinite)
** \return FctERR - Error code
**/
FctERR NONNULL__ Logic_out_StartBlink(	Logic_out * const out, const eGPIOState action, const uint32_t delay,
										const uint32_t active, const uint32_t inactive, const uint32_t count);


/*!\brief Handles Logic_out treatment
** \param[in,out] out - Logic_out instance to handle
**/
void NONNULL__ Logic_out_handler(Logic_out * const out);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* LOGIC_OUT_H__ */
/****************************************************************/
