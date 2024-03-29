/*!\file PWM_IC.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief PWM Input Capture handling
** \details PWM_In configuration:
** TIM with multiple channels with slave mode capability shall be used
**
** Slave Mode: Reset Mode
** Trigger: TI2FP2 (?? depending channels ??)
** Channel1: Input Capture indirect mode
** Channel2: Input Capture direct mode
**
** Prescaler & CLK div: 0, No CLK div
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
** 				- It may not be an issue if PWM duty cycle is always changing gradually, but will be in case of erratic changes (when becoming continuous signal)
** 				- Thus, to overcome this issue, PWM signal shall always have edges to keep measures working and consistent
** \note Define NB_PWM_IC symbol with number of inputs at project level to use PWM_IC functionalities
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
	__IOM uint32_t			Frequency;		//!< Frequency Value
	__IOM uint32_t			DutyCycle;		//!< Duty Cycle Value
	__IOM uint32_t			CallbackTick;	//!< Last callback Tick (useful to ensure Freq & DutyCycle are still valid)
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
/*!\brief Init PWM Input Capture channel
** \param[in,out] pPWM_IC - pointer to PWM input capture instance
** \param[in,out] pTim - pointer to TIM instance
** \param[in] Direct_Channel - Direct Channel used for PWM input capture
** \param[in] Indirect_Channel - Indirect Channel used for PWM input capture
** \param[in] Scale - PWM input capture duty cycle output scale
** \return FctERR - Error code
**/
FctERR init_PWM_IC(PWM_IC * const pPWM_IC, TIM_HandleTypeDef * const pTim, const uint32_t Direct_Channel, const uint32_t Indirect_Channel, const uint32_t Scale);


/*!\brief Get PWM Input Capture last update (in ms)
** \note Can be useful to ensure IC is still ongoing and results aren't outdated
** \param[in] pPWM_IC - pointer to PWM_IC instance
** \return Last PWM_IC Update (in ms)
**/
__INLINE uint32_t NONNULL_INLINE__ get_PWM_IC_LastUpdate(const PWM_IC * const pPWM_IC) {
	return OVF_DIFF(HALTicks(), pPWM_IC->CallbackTick); }


/*!\brief Get current PWM Input Capture frequency
** \param[in] pPWM_IC - pointer to PWM_IC instance
** \return Measured frequency
**/
__INLINE uint32_t NONNULL_INLINE__ get_PWM_IC_Freq(const PWM_IC * const pPWM_IC) {
	return pPWM_IC->Frequency; }


/*!\brief Get current PWM Input Capture duty cycle
** \param[in] pPWM_IC - pointer to PWM_IC instance
** \return Measured duty cycle
**/
__INLINE uint32_t NONNULL_INLINE__ get_PWM_IC_DutyCycle(const PWM_IC * const pPWM_IC) {
	return pPWM_IC->DutyCycle; }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
#endif	/* __PWM_IC_H */
/****************************************************************/
