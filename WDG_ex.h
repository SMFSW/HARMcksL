/*!\file WDG_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Extensions for WDG peripherals
** \note For proper sync to use state related functionalities (get and update), you shall disable
**		 call to \p MX_IWDG_Init and/or \p MX_WWDG_Init in HAL generated from ioc file,
**		 \ref WDG_ex_Init have to be called instead at the end of your init routine.
**/
/****************************************************************/
#ifndef WDG_EX_H__
	#define WDG_EX_H__

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
bool WDG_ex_get_IWDG_state(void);

/*!\brief Get IWDG status
** \return WWDG enabled state
**/
bool WDG_ex_get_WWDG_state(void);


/*!\brief Refresh IWDG & WWDG
**/
void WDG_ex_refresh_all(void);

/*!\brief Refresh IWDG
**/
void WDG_ex_refresh_IWDG(void);

/*!\brief Refresh WWDG
**/
void WDG_ex_refresh_WWDG(void);


/*!\brief Save IWDG & WWDG configuration
**/
void WDG_ex_save_cfg(void);

/*!\brief Restore saved IWDG & WWDG configuration
** \return HAL Status
**/
HAL_StatusTypeDef WDG_ex_restore_cfg(void);


#if defined(HAL_IWDG_MODULE_ENABLED)
/*!\brief Set IWDG period (in us)
** \warning 125us granularity for a 32KHz clock with minimum prescaler of 4 (typical STM32 IWDG architecture)
** \param[in,out] pIwdg - Pointer to IWDG instance
** \param[in] per - Period (in us)
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ WDG_ex_set_IWDG_Period_us(IWDG_HandleTypeDef * const pIwdg, const uint32_t per);

/*!\brief Set IWDG period (in ms)
** \param[in,out] pIwdg - Pointer to IWDG instance
** \param[in] per - Period (in ms)
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef NONNULL_INLINE__ WDG_ex_set_IWDG_Period_ms(IWDG_HandleTypeDef * const pIwdg, const uint32_t per) {
	return WDG_ex_set_IWDG_Period_us(pIwdg, per * 1000U); }

/*!\brief Get IWDG period (in us)
** \param[in] pIwdg - Pointer to IWDG instance
** \return Period (in us)
**/
uint32_t NONNULL__ WDG_ex_get_IWDG_Period_us(const IWDG_HandleTypeDef * const pIwdg);

/*!\brief Get IWDG period (in ms)
** \param[in] pIwdg - Pointer to IWDG instance
** \return Period (in ms)
**/
__INLINE uint32_t NONNULL__ WDG_ex_get_IWDG_Period_ms(const IWDG_HandleTypeDef * const pIwdg) {
	return (WDG_ex_get_IWDG_Period_us(pIwdg) / 1000U); }
#endif

/*!\brief Initialize WDG_ex module and start watchdog(s)
** \return HAL Status
**/
HAL_StatusTypeDef NONNULL__ WDG_ex_Init(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_IWDG_MODULE_ENABLED) || defined(HAL_WWDG_MODULE_ENABLED) */
#endif	/* WDG_EX_H__ */
/****************************************************************/
