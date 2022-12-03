/*!\file WDG_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2022, SMFSW
** \brief Extensions for WDG peripherals
** \details This module is mostly meant for debug target purposes,
**			giving the ability to call freeze/unfreeze watchdogs functions
**			no matter the build target, watchdogs being unfrozen only when
**			they were enabled in the first place.
** \warning For this module to work properly, WDG_init_check have to be called once
**			at the end of your init routine prior to use freeze/unfreeze functions.
**/
/****************************************************************/
#ifndef __WDG_EX_H
	#define __WDG_EX_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Check IWDG & WWDG status
** \warning This function have to be called once at the end of your init routine prior to use \ref WDG_freeze & \ref WDG_unfreeze functions.
**/
void WDG_init_check(void);


/*!\brief Freeze IWDG & WWDG
** \note Watchdogs will be frozen only if enabled by init (checked by \ref WDG_init_check).
**/
void WDG_freeze(void);

/*!\brief Unfreeze IWDG & WWDG
** \note Watchdogs will be unfrozen only if enabled by init (checked by \ref WDG_init_check).
**/
void WDG_unfreeze(void);


#if defined(HAL_IWDG_MODULE_ENABLED)
/*!\brief Set IWDG period (in us)
** \warning 125us granularity for a 32KHz clock with minimum prescaler of 4 (typical STM32 IWDG architecture)
** \param[in,out] pIwdg - Pointer to IWDG instance
** \param[in] per - Period (in us)
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ set_IWDG_Period_us(IWDG_HandleTypeDef * const pIwdg, const uint32_t per);

/*!\brief Set IWDG period (in ms)
** \param[in,out] pIwdg - Pointer to IWDG instance
** \param[in] per - Period (in ms)
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef NONNULL_INLINE__ set_IWDG_Period_ms(IWDG_HandleTypeDef * const pIwdg, const uint32_t per) {
	return set_IWDG_Period_us(pIwdg, per * 1000); }
#endif


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __WDG_EX_H */
/****************************************************************/
