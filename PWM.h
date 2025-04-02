/*!\file PWM.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief Straightforward PWM handling
** \warning Init functions assume that TIM peripherals instance were already configured by HAL
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


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
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


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
#endif	/* __PWM_H */
/****************************************************************/
