/*!\file PWM.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief Straightforward PWM handling
**/
/****************************************************************/
#ifndef __PWM_H
	#define __PWM_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


/*!\struct logicPWM
** \brief Software PWM on GPIO struct
**/
typedef __IO struct logicPWM {
	uint16_t				cntr;		//!< Counter
	uint16_t				duty;		//!< Current Duty cycle
	struct {
		TIM_HandleTypeDef *	pTim;		//!< Timer instance (for reference)
		GPIO_TypeDef *		GPIOx;		//!< Port of emulated PWM pin
		uint16_t			GPIO_Pin;	//!< Pin mask on port
		uint16_t			tim_freq;	//!< Timer frequency (for reference)
		uint16_t			duty;		//!< Duty Cycle (effective when new period starts)
		uint16_t			per;		//!< Overflow threshold (emulated PWM period)
		bool				polarity;	//!< Output polarity
	} cfg;
} logicPWM;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/***********/
/*** PWM ***/
/***********/
/*!\brief Init TIM PWM module channel with frequency and starts the channel
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] freq - Desired PWM frequency
** \param[in] start_polarity - Set PWM output to 100% at init if set to On (0% otherwise)
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ init_PWM_Chan(TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint16_t freq, const eState start_polarity);


/*!\brief Set PWM channel output on/off
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] on - Channel Output state 0: off, 1: on
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef NONNULL_INLINE__ set_PWM_Output(TIM_HandleTypeDef * const pTim, const uint32_t chan, const bool on) {
	return on ? HAL_TIM_PWM_Start(pTim, chan) : HAL_TIM_PWM_Stop(pTim, chan); }


/*!\brief Set TIM module PWM duty cycle (scaled)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \param[in] scale - Full scale value
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ set_PWM_Duty_Scaled(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint32_t duty, const uint32_t scale);

/*!\brief Set TIM module PWM duty cycle (u16 value)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - 16b duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef NONNULL_INLINE__ set_PWM_Duty_Word(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint16_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, (uint16_t) -1); }

/*!\brief Set TIM module PWM duty cycle (u8 value)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - 8b duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef NONNULL_INLINE__ set_PWM_Duty_Byte(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint8_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, (uint8_t) -1); }

/*!\brief Set TIM module PWM duty cycle (percents)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Percent duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef NONNULL_INLINE__ set_PWM_Duty_Perc(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint8_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, 100); }


/********************/
/*** Emulated PWM ***/
/********************/
/*!\brief Set channel pin & polarity for emulated PWM channel
** \param[in] GPIOx - port for emulated PWM
** \param[in] GPIO_Pin - pin for emulated PWM
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in] polarity - 0: low polarity, 1: high polarity
** \return Error code
**/
FctERR NONNULL__ logPWM_setPin(logicPWM * const pPWM, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const bool polarity);

/*!\brief Set channel frequency for emulated PWM channel
** \warning For multiple PWMs on same timer with different frequencies,
** 			take care of init order (first configured channel will get TIM parameters precedence)
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in,out] pTim - pointer to TIM instance for Frequency computation
** \param[in] freq - PWM frequency to apply
** \param[in] granularity - PWM duty cycle granularity
** \return Error code
**/
FctERR NONNULL__ logPWM_setFreq(logicPWM * const pPWM, TIM_HandleTypeDef * const pTim, const uint16_t freq, uint16_t granularity);

/*!\brief Set new duty cycle for emulated PWM channel
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in] val - Duty cycle to apply
** \return Error code
**/
FctERR NONNULL__ logPWM_setDuty(logicPWM * const pPWM, const uint16_t val);

/*!\brief Get channel frequency for emulated PWM channel
** \param[in,out] pPWM - pointer to emulated PWM channel
** \return PWM frequency
**/
__INLINE uint16_t NONNULL__ logPWM_getFreq(const logicPWM * const pPWM) {
	return pPWM->cfg.tim_freq / pPWM->cfg.per; }

/*!\brief Get channel Duty Cycle for emulated PWM channel
** \param[in,out] pPWM - pointer to emulated PWM channel
** \return PWM Duty cycle
**/
__INLINE uint16_t NONNULL__ logPWM_getDutyCycle(const logicPWM * const pPWM) {
	return ((pPWM->cfg.duty * 100) / pPWM->cfg.per); }

/*!\brief Handler for an emulated PWM channel
** \warning Shall be called directly from timer interrupt (HAL_TIM_PeriodElapsedCallback)
** \param[in,out] pPWM - pointer to emulated PWM channel
**/
void NONNULL__ logPWM_handler(logicPWM * const pPWM);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
#endif	/* __PWM_H */
/****************************************************************/
