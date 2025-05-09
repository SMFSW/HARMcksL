/*!\file TIM_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief Extensions for TIM peripherals
** \warning Init functions assume that TIM peripherals instance were already configured by HAL
** \warning Shall work for all STM32 F/G/H/L families only (yet)
** \note \c TIM_MIN_GRANULARITY can defined at project level to tweak to needed minimum granularity
**/
/****************************************************************/
#ifndef TIM_EX_H__
	#define TIM_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_TIM_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get TIM peripheral clock frequency
** \warning Shall work for all STM32 F/G families, L families not totally covered
** \param[in,out] pTim - pointer to TIM instance
** \return TIM peripheral clock frequency (Hz)
**/
uint32_t NONNULL__ RCC_TIMCLKFreq(const TIM_HandleTypeDef * const pTim);


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
HAL_StatusTypeDef NONNULL__ write_TIM_CCR(const TIM_HandleTypeDef * const pTim, const uint32_t chan, const uint32_t CCR_val);


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

/*!\brief Set TIM module tick frequency
** \note This function sets TIM period to maximum value (which is most likely what is needed in such a configuration case)
** \param[in,out] pTim - pointer to TIM instance for Frequency computation
** \param[in] freq - Desired TIM tick frequency
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ set_TIM_Tick_Freq(TIM_HandleTypeDef * const pTim, const uint32_t freq);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_TIM_MODULE_ENABLED) */
#endif	/* TIM_EX_H__ */
/****************************************************************/
