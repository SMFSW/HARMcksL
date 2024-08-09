/*!\file PWM_IC.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
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
#ifndef __PWM_IC_H
	#define __PWM_IC_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


#ifndef NB_PWM_IC
#define	NB_PWM_IC	0		//!< Number of PWM input capture instances
//! \note NB_PWM_IC can defined at project level to set number of PWM input capture instances
#endif


/*!\struct PWM_IC
** \brief PWM input capture structure
**/
typedef struct PWM_IC {
	uint32_t				Frequency;		//!< Frequency Value
	uint32_t				DutyCycle;		//!< Duty Cycle Value
	uint32_t				Timeout;		//!< No sample timeout
	__IOM uint32_t			CallbackTick;	//!< Last callback Tick (useful to ensure Freq & DutyCycle are still valid)
	__IOM uint32_t			Direct_cnt;		//!< Direct channel sampled counter value
	__IOM uint32_t			Indirect_cnt;	//!< Indirect channel sample counter value
	__IOM eEdge				Last_Edge;		//!< Last captured edge (caught through ITs, or deduced from callback)
	__IOM bool				New_Sample;		//!< New samples available
	struct {
		TIM_HandleTypeDef * htim;			//!< PWM capture TIM handler
		uint32_t			Direct_Pin;		//!< PWM capture Direct mode pin
		uint32_t			Indirect_Pin;	//!< PWM capture Indirect mode pin
		uint32_t			ActiveChan;		//!< PWM capture active channel to trigger on
		uint32_t			refCLK;			//!< PWM capture TIM reference clock
		uint32_t			Scale;			//!< PWM capture scale range
	} cfg;
} PWM_IC;

extern PWM_IC PWMin[NB_PWM_IC];


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief PWM Input Capture get Pin state (in case of timeout, or at startup)
** \weak Function declared as weak, needs to be customly implemented in user code, otherwise returns RESET state
** \param[in] idx - PWM_IC instance index
** \return Pin state
**/
GPIO_PinState PWM_IC_get_Pin_State_Callback(const uint8_t idx);



/*!\brief Init PWM Input Capture channel
** \param[in,out] pPWM_IC - pointer to PWM input capture instance
** \param[in,out] pTim - pointer to TIM instance
** \param[in] Direct_Channel - Direct Channel used for PWM input capture
** \param[in] Indirect_Channel - Indirect Channel used for PWM input capture
** \param[in] Scale - PWM input capture duty cycle output scale
** \return FctERR - Error code
**/
FctERR NONNULL__ init_PWM_IC(PWM_IC * const pPWM_IC, TIM_HandleTypeDef * const pTim, const uint32_t Direct_Channel, const uint32_t Indirect_Channel, const uint32_t Scale);


/*!\brief Get current PWM Input Capture frequency
** \param[in] pPWM_IC - pointer to PWM_IC instance
** \return Measured frequency
**/
uint32_t NONNULL__ get_PWM_IC_Freq(PWM_IC * const pPWM_IC);


/*!\brief Get current PWM Input Capture duty cycle
** \param[in] pPWM_IC - pointer to PWM_IC instance
** \return Measured duty cycle
**/
uint32_t NONNULL__ get_PWM_IC_DutyCycle(PWM_IC * const pPWM_IC);


/*!\brief PWM Input Capture handler
** \note Shall to be called periodically if PWM_IC_NO_IT is defined
** \note When interrupts are used, handler can be called periodically (but not required), results getters will perform the conversion when required
**/
void PWM_IC_handler(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
#endif	/* __PWM_IC_H */
/****************************************************************/
