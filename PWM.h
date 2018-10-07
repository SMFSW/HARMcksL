/*!\file PWM.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Straightforward PWM handling
** \warning Shall work for all STM32 F families, L families not totally covered
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
/*** TIM Base ***/
/*!\brief Start TIM module interrupts
** \param[in,out] pTim - pointer to TIM instance
** \param[in] on - Time Interrupts 0: off, 1: on
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_TIM_Interrupts(TIM_HandleTypeDef * pTim, const bool on) {
	return on ? HAL_TIM_Base_Start_IT(pTim) : HAL_TIM_Base_Stop_IT(pTim); }

/*!\brief De-Init TIM module and start interruptions
** \param[in,out] pTim - pointer to TIM instance
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ deinit_TIM_Base(TIM_HandleTypeDef * pTim) {
	return set_TIM_Interrupts(pTim, Off); }	// Stop interrupts

/*!\brief Init TIM module and start interruptions
** \param[in,out] pTim - pointer to TIM instance
** \param[in] freq - Desired TIM frequency
** \return HAL Status
**/
HAL_StatusTypeDef init_TIM_Base(TIM_HandleTypeDef * pTim, const uint32_t freq);

/*!\brief Set TIM module frequency
** \param[in,out] pTim - pointer to TIM instance for Frequency computation
** \param[in] freq - Desired TIM frequency
** \return HAL Status
**/
HAL_StatusTypeDef set_TIM_Freq(TIM_HandleTypeDef * pTim, const uint32_t freq);


/*** PWM ***/
/*!\brief Init TIM PWM module channel with frequency and starts the channel
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] freq - Desired PWM frequency
** \return HAL Status
**/
HAL_StatusTypeDef init_PWM_Chan(TIM_HandleTypeDef * pTim, const uint32_t chan, const uint16_t freq);


/*!\brief Set PWM channel output on/off
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] on - Channel Output state 0: off, 1: on
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_PWM_Output(TIM_HandleTypeDef * pTim, const uint32_t chan, const bool on) {
	return on ? HAL_TIM_PWM_Start(pTim, chan) : HAL_TIM_PWM_Stop(pTim, chan); }


/*!\brief Set TIM module PWM duty cycle (scaled)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \param[in] scale - Full scale value
** \return HAL Status
**/
HAL_StatusTypeDef set_PWM_Duty_Scaled(const TIM_HandleTypeDef * pTim, const uint32_t chan, const uint16_t duty, const uint16_t scale);

/*!\brief Set TIM module PWM duty cycle (percents)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_PWM_Duty_Perc(const TIM_HandleTypeDef * pTim, const uint32_t chan, const uint16_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, 100); }

/*!\brief Set TIM module PWM duty cycle (u16-bit value)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_PWM_Duty_Word(const TIM_HandleTypeDef * pTim, const uint32_t chan, const uint16_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, (uint16_t) -1); }

/*!\brief Set TIM module PWM duty cycle (u8-bit value)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_PWM_Duty_Byte(const TIM_HandleTypeDef * pTim, const uint32_t chan, const uint8_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, (uint8_t) -1); }


/*** Emulated PWM ***/
/*!\brief Set channel pin & polarity for emulated PWM channel
** \param[in] GPIOx - port for emulated PWM
** \param[in] GPIO_Pin - pin for emulated PWM
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in] polarity - 0: low polarity, 1: high polarity
** \return Error code
**/
FctERR logPWM_setPin(logicPWM * pPWM, GPIO_TypeDef * GPIOx, const uint16_t GPIO_Pin, const bool polarity);

/*!\brief Set channel frequency for emulated PWM channel
** \warning For multiple PWMs on same timer with different frequencies,
** 			take care of init order (first configured channel will get TIM parameters precedence)
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in,out] pTim - pointer to TIM instance for Frequency computation
** \param[in] freq - PWM frequency to apply
** \param[in] granularity - PWM duty cycle granularity
** \return Error code
**/
FctERR logPWM_setFreq(logicPWM * pPWM, TIM_HandleTypeDef * pTim, const uint16_t freq, uint16_t granularity);

/*!\brief Set new duty cycle for emulated PWM channel
** \param[in,out] pPWM - pointer to emulated PWM channel
** \param[in] val - Duty cycle to apply
** \return Error code
**/
FctERR logPWM_setDuty(logicPWM * pPWM, const uint16_t val);

/*!\brief Get channel frequency for emulated PWM channel
** \param[in,out] freq - pointer to frequency result
** \param[in,out] pPWM - pointer to emulated PWM channel
** \return Error code
**/
FctERR logPWM_getFreq(uint16_t * freq, const logicPWM * pPWM);

/*!\brief Get channel Duty Cycle for emulated PWM channel
** \param[in,out] duty - pointer to duty cycle result
** \param[in,out] pPWM - pointer to emulated PWM channel
** \return Error code
**/
FctERR logPWM_getDutyCycle(float * duty, const logicPWM * pPWM);

/*!\brief Handler for an emulated PWM channel
** \warning Shall be called directly from timer interrupt (HAL_TIM_PeriodElapsedCallback)
** \param[in,out] pPWM - pointer to emulated PWM channel
**/
void logPWM_handler(logicPWM * pPWM);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
#endif	/* __PWM_H */
/****************************************************************/
