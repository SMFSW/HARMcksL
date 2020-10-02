/*!\file TIM_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief TIM extended functions
** \warning Shall work for all STM32 F/G families, L/H families not totally covered
**/
/****************************************************************/
#ifndef __TIM_EX_H
	#define __TIM_EX_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get TIM module clock
** \warning Shall work for all STM32 F/G families, L families not totally covered
** \param[in,out] pTim - pointer to TIM instance
** \return TIM Clock frequency
**/
uint32_t NONNULL__ get_TIM_clock(const TIM_HandleTypeDef * const pTim);


/*!\brief Write preload bit in TIM module for desired channel
** \param[in,out] pTim - pointer to TIM instance
** \param[in] chan - Channel to write
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ write_TIM_Preload(TIM_HandleTypeDef * const pTim, const uint32_t chan);


/*!\brief Low level TIM module CCR write
** \param[in,out] pTim - pointer to TIM instance for PWM generation
** \param[in] chan - Channel to write
** \param[in] CCR_val - CCR value
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ write_TIM_CCR(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint16_t CCR_val);


/*!\brief Start TIM module interrupts
** \param[in,out] pTim - pointer to TIM instance
** \param[in] on - Time Interrupts 0: off, 1: on
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef NONNULL_INLINE__ set_TIM_Interrupts(TIM_HandleTypeDef * const pTim, const bool on) {
	return on ? HAL_TIM_Base_Start_IT(pTim) : HAL_TIM_Base_Stop_IT(pTim); }

/*!\brief De-Init TIM base module (stop interruptions)
** \param[in,out] pTim - pointer to TIM instance
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef NONNULL_INLINE__ deinit_TIM_Base(TIM_HandleTypeDef * const pTim) {
	return set_TIM_Interrupts(pTim, Off); }	// Stop interrupts

/*!\brief Init TIM module and start interruptions
** \param[in,out] pTim - pointer to TIM instance
** \param[in] freq - Desired TIM frequency
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ init_TIM_Base(TIM_HandleTypeDef * const pTim, const uint32_t freq);

/*!\brief Set TIM module frequency
** \param[in,out] pTim - pointer to TIM instance for Frequency computation
** \param[in] freq - Desired TIM frequency
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ set_TIM_Freq(TIM_HandleTypeDef * const pTim, const uint32_t freq);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
#endif	/* __TIM_EX_H */
/****************************************************************/
