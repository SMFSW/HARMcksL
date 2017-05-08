/*!\file PWM.h
** \author SMFSW
** \version v0.5
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Simple PWM handling
**/
/****************************************************************/
#ifndef __PWM_H
	#define __PWM_H

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_TIM_MODULE_ENABLED)
#include "tim.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Set TIM module PWM frequency for channel
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] freq - Desired PWM frequency
**/
HAL_StatusTypeDef set_PWM_Freq(TIM_HandleTypeDef * pTim, uint32_t freq);

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
