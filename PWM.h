/*!\file PWM.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Straightforward PWM handling
** \warning Shall work for all STM32 F families, L families not totally covered
**/
/****************************************************************/
#ifndef __PWM_H
	#define __PWM_H

#include "sarmfsw.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*** TIM Base ***/
/*!\brief Init TIM module and start interruptions
** \param[in,out] pTim - pointer to TIM instance
** \param[in] freq - Desired TIM frequency
**/
HAL_StatusTypeDef init_TIM_Base(TIM_HandleTypeDef * pTim, uint32_t freq);


/*!\brief Set TIM module frequency
** \param[in,out] pTim - pointer to TIM instance for Frequency computation
** \param[in] freq - Desired TIM frequency
**/
HAL_StatusTypeDef set_TIM_Freq(TIM_HandleTypeDef * pTim, uint32_t freq);


/*!\brief Start TIM module interrupts
** \param[in,out] pTim - pointer to TIM instance
** \param[in] on - Time Interrupts 0: off, 1: on
**/
__INLINE HAL_StatusTypeDef INLINE__ set_TIM_Interrupts(TIM_HandleTypeDef * pTim, bool on) {
	return on ? HAL_TIM_Base_Start_IT(pTim) : HAL_TIM_Base_Stop_IT(pTim); }


/*** PWM ***/
/*!\brief Init TIM PWM module channel with frequency and starts the channel
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] freq - Desired PWM frequency
**/
HAL_StatusTypeDef init_PWM_Chan(TIM_HandleTypeDef * pTim, uint32_t chan, uint16_t freq);


/*!\brief Set PWM channel output on/off
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] on - Channel Output state 0: off, 1: on
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_PWM_Output(TIM_HandleTypeDef * pTim, uint32_t chan, bool on) {
	return on ? HAL_TIM_PWM_Start(pTim, chan) : HAL_TIM_PWM_Stop(pTim, chan); }

/*!\brief Set TIM module PWM duty cycle (scaled)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \param[in] scale - Full scale value
** \return HAL Status
**/
HAL_StatusTypeDef set_PWM_Duty_Scaled(TIM_HandleTypeDef * pTim, uint32_t chan, uint16_t duty, uint16_t scale);


/*!\brief Set TIM module PWM duty cycle (percents)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_PWM_Duty_Perc(TIM_HandleTypeDef * pTim, uint32_t chan, uint16_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, 100); }


/*!\brief Set TIM module PWM duty cycle (u16-bit value)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_PWM_Duty_Word(TIM_HandleTypeDef * pTim, uint32_t chan, uint16_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, (uint16_t) -1); }


/*!\brief Set TIM module PWM duty cycle (u8-bit value)
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] duty - Scaled duty cycle value to write
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ set_PWM_Duty_Byte(TIM_HandleTypeDef * pTim, uint32_t chan, uint8_t duty) {
	return set_PWM_Duty_Scaled(pTim, chan, duty, (uint8_t) -1); }


/****************************************************************/
#endif
#endif /* __PWM_H */
/****************************************************************/
