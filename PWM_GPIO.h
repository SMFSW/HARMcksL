/*!\file PWM_GPIO.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief GPIO PWM emulation handling
** \details PWM_GPIO configuration:
** TIM with enabled period callback interrupt shall be used (as time base for PWM emulation)
**/
/****************************************************************/
#ifndef __PWM_GPIO_H
	#define __PWM_GPIO_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct PWM_GPIO
** \brief Software PWM on GPIO struct
**/
typedef __IO struct PWM_GPIO {
	uint16_t				cntr;		//!< Counter
	uint16_t				duty;		//!< Current Duty cycle
	bool					init;		//!< PWM initialized (ready for use)
	struct {
		TIM_HandleTypeDef *	pTim;		//!< Timer instance (for reference)
		GPIO_TypeDef *		GPIOx;		//!< Port of emulated PWM pin
		uint16_t			GPIO_Pin;	//!< Pin mask on port
		uint16_t			tim_freq;	//!< Timer frequency (for reference)
		uint16_t			duty;		//!< Duty Cycle (effective when new period starts)
		uint16_t			per;		//!< Overflow threshold (emulated PWM period)
		bool				polarity;	//!< Output polarity
	} cfg;
} PWM_GPIO;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Set channel pin & polarity for emulated PWM channel
** \param[in] GPIOx - port for emulated PWM
** \param[in] GPIO_Pin - pin for emulated PWM
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in] polarity - 0: low polarity, 1: high polarity
** \return Error code
**/
FctERR NONNULL__ PWM_GPIO_setPin(PWM_GPIO * const pPWM, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const bool polarity);

/*!\brief Set channel frequency for emulated PWM channel
** \warning For multiple PWMs on same timer with different frequencies,
** 			take care of init order (first configured channel will get TIM parameters precedence)
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in,out] pTim - pointer to TIM instance for Frequency computation
** \param[in] freq - PWM frequency to apply
** \param[in] granularity - PWM duty cycle granularity
** \return Error code
**/
FctERR NONNULL__ PWM_GPIO_setFreq(PWM_GPIO * const pPWM, TIM_HandleTypeDef * const pTim, const uint16_t freq, uint16_t granularity);

/*!\brief Set new duty cycle for emulated PWM channel
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in] val - Duty cycle to apply
** \return Error code
**/
FctERR NONNULL__ PWM_GPIO_setDuty(PWM_GPIO * const pPWM, const uint16_t val);

/*!\brief Get channel frequency for emulated PWM channel
** \param[in,out] pPWM - pointer to emulated PWM channel
** \return PWM frequency
**/
__INLINE uint16_t NONNULL__ PWM_GPIO_getFreq(const PWM_GPIO * const pPWM) {
	return pPWM->cfg.tim_freq / pPWM->cfg.per; }

/*!\brief Get channel Duty Cycle for emulated PWM channel
** \param[in,out] pPWM - pointer to emulated PWM channel
** \return PWM Duty cycle
**/
__INLINE uint16_t NONNULL__ PWM_GPIO_getDutyCycle(const PWM_GPIO * const pPWM) {
	return ((pPWM->cfg.duty * 100) / pPWM->cfg.per); }

/*!\brief Handler for an emulated PWM channel
** \warning Shall be called directly from timer interrupt (HAL_TIM_PeriodElapsedCallback)
** \param[in,out] pPWM - pointer to emulated PWM channel
**/
void NONNULL__ PWM_GPIO_handler(PWM_GPIO * const pPWM);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
#endif	/* __PWM_GPIO_H */
/****************************************************************/
