/*!\file WDG_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief Extensions for WDG peripherals
** \details This module is mostly meant for debug target purposes,
**			giving the ability to call freeze/unfreeze watchdogs functions
**			no matter the build target, watchdogs being unfrozen only when
**			they were enabled in the first place.
** \warning Watchdogs cannot be frozen while code is running, freeze will on be relevant
** 			when execution is paused, thus no freezing possible for a time consuming operation.
** 			Workaround is to save the watchdogs configurations, modify temporarily confiruation,
** 			then restore the configuration back, using \ref WDG_save_cfg & \ref WDG_restore_cfg.
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

#if defined(HAL_IWDG_MODULE_ENABLED) || defined(HAL_WWDG_MODULE_ENABLED)
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get IWDG status
** \return IWDG enabled state
**/
bool WDG_get_state_IWDG(void);

/*!\brief Get IWDG status
** \return WWDG enabled state
**/
bool WDG_get_state_WWDG(void);


/*!\brief Check IWDG & WWDG status
** \warning This function have to be called once at the end of your init routine prior to use \ref WDG_freeze & \ref WDG_unfreeze functions.
**/
void WDG_init_check(void);


/*!\brief Refresh IWDG & WWDG
**/
void WDG_refresh(void);

/*!\brief Freeze IWDG & WWDG
** \note Watchdogs will be frozen only if enabled by init (checked by \ref WDG_init_check).
**/
void WDG_freeze(void);

/*!\brief Unfreeze IWDG & WWDG
** \note Watchdogs will be unfrozen only if enabled by init (checked by \ref WDG_init_check).
**/
void WDG_unfreeze(void);


/*!\brief Save IWDG & WWDG configuration
**/
void WDG_save_cfg(void);

/*!\brief Restore saved IWDG & WWDG configuration
** \return HAL Status
**/
HAL_StatusTypeDef WDG_restore_cfg(void);


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

#endif	/* defined(HAL_IWDG_MODULE_ENABLED) || defined(HAL_WWDG_MODULE_ENABLED) */
#endif	/* __WDG_EX_H */
/****************************************************************/
