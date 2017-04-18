/*!\file PWM.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Simple PWM handling
**/
/****************************************************************/
#ifndef __PWM_H
	#define __PWM_H
/****************************************************************/

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG

#if defined(HAL_TIM_MODULE_ENABLED)
#include "tim.h"


HAL_StatusTypeDef set_PWM_Freq(TIM_HandleTypeDef * pTim, uint32_t Freq);

HAL_StatusTypeDef set_PWM_Duty_Scaled(TIM_HandleTypeDef * pTim, uint32_t Chan, uint16_t Duty, uint16_t Scale);							// Scaled

__INLINE HAL_StatusTypeDef __attribute__((always_inline)) set_PWM_Duty_Perc(TIM_HandleTypeDef * pTim, uint32_t Chan, uint16_t Duty) {	// Percent
	return set_PWM_Duty_Scaled(pTim, Chan, Duty, 100); }

__INLINE HAL_StatusTypeDef __attribute__((always_inline)) set_PWM_Duty_Word(TIM_HandleTypeDef * pTim, uint32_t Chan, uint16_t Duty) {	// 16b
	return set_PWM_Duty_Scaled(pTim, Chan, Duty, (uint16_t) -1); }

__INLINE HAL_StatusTypeDef __attribute__((always_inline)) set_PWM_Duty_Byte(TIM_HandleTypeDef * pTim, uint32_t Chan, uint8_t Duty) {	// 8b
	return set_PWM_Duty_Scaled(pTim, Chan, Duty, (uint8_t) -1); }

#endif

/****************************************************************/
#endif /* __PWM_H */
/****************************************************************/
