/*!\file WDG_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Extensions for WDG peripherals
** \note For proper sync to use state related functionalities (get and update), you shall disable
**		 call to \p MX_IWDG_Init and/or \p MX_WWDG_Init in HAL generated from ioc file,
**		 \ref WDG_ex_Init have to be called instead at the end of your init routine.
**/
/****************************************************************/
#include "sarmfsw.h"
#include "WDG_ex.h"

#if defined(HAL_IWDG_MODULE_ENABLED) || defined(HAL_WWDG_MODULE_ENABLED)
/****************************************************************/


static const uint32_t refCLK = LSI_VALUE / 1000U;	// Watchdog clock frequency in KHz (for 32b computations)
static const uint32_t prescaler_mult = 4U;			// STM32 IWDG STM32 architecture prescaler multiplier


#if defined(HAL_IWDG_MODULE_ENABLED)
static IWDG_InitTypeDef IWDG_cfg = { 0 };
static bool iwdg_en = false;
#endif

#if defined(HAL_WWDG_MODULE_ENABLED)
static WWDG_InitTypeDef WWDG_cfg = { 0 };
static bool wwdg_en = false;
#endif

bool WDG_ex_get_IWDG_state(void)
{
	#if defined(HAL_IWDG_MODULE_ENABLED)
	return iwdg_en;
	#else
	return false;
	#endif
}

bool WDG_ex_get_WWDG_state(void)
{
	#if defined(HAL_WWDG_MODULE_ENABLED)
	return wwdg_en;
	#else
	return false;
	#endif
}


void WDG_ex_refresh_all(void)
{
	#if defined(HAL_IWDG_MODULE_ENABLED)
	if (iwdg_en)
	{
		UNUSED_RET HAL_IWDG_Refresh(&hiwdg);
	}
	#endif

	#if defined(HAL_WWDG_MODULE_ENABLED)
	if (wwdg_en)
	{
		UNUSED_RET HAL_WWDG_Refresh(&hwwdg);
	}
	#endif
}

void WDG_ex_refresh_IWDG(void)
{
	#if defined(HAL_IWDG_MODULE_ENABLED)
	if (iwdg_en)
	{
		UNUSED_RET HAL_IWDG_Refresh(&hiwdg);
	}
	#endif
}

void WDG_ex_refresh_WWDG(void)
{
	#if defined(HAL_WWDG_MODULE_ENABLED)
	if (wwdg_en)
	{
		UNUSED_RET HAL_WWDG_Refresh(&hwwdg);
	}
	#endif
}


void WDG_ex_save_cfg(void)
{
	#if defined(HAL_IWDG_MODULE_ENABLED)
	UNUSED_RET memcpy(&IWDG_cfg, &hiwdg.Init, sizeof(IWDG_InitTypeDef));
	#endif

	#if defined(HAL_WWDG_MODULE_ENABLED)
	UNUSED_RET memcpy(&WWDG_cfg, &hwwdg.Init, sizeof(WWDG_InitTypeDef));
	#endif
}

HAL_StatusTypeDef WDG_ex_restore_cfg(void)
{
	HAL_StatusTypeDef status = HAL_OK;

	#if defined(HAL_IWDG_MODULE_ENABLED)
	UNUSED_RET memcpy(&hiwdg.Init, &IWDG_cfg, sizeof(IWDG_InitTypeDef));
	status |= HAL_IWDG_Init(&hiwdg);
	#endif

	#if defined(HAL_WWDG_MODULE_ENABLED)
	UNUSED_RET memcpy(&hwwdg.Init, &WWDG_cfg, sizeof(WWDG_InitTypeDef));
	status |= HAL_WWDG_Init(&hwwdg);
	#endif

	return status;
}


#if defined(HAL_IWDG_MODULE_ENABLED)
HAL_StatusTypeDef NONNULL__ WDG_ex_set_IWDG_Period_us(IWDG_HandleTypeDef * const pIwdg, const uint32_t per)
{
	const uint32_t max_prescaler = 256U;	// STM32 IWDG STM32 architecture maximum prescaler
	const uint32_t max_reload = 4095U;		// STM32 IWDG STM32 architecture maximum reload value

	uint32_t reload;
	uint32_t prescaler = prescaler_mult;

	HAL_StatusTypeDef status;

	while (prescaler <= max_prescaler)
	{
		reload = (refCLK * per) / (1000U * prescaler);	// Computation using KHz instead of Hz to stay in 32b range
		if (reload <= max_reload)	{ break; }			// If in range

		prescaler <<= 1U;
	}

	if (	(reload == 0U)					// Period is too short
		||	(prescaler > max_prescaler))	// If nothing has been found (after last iteration)
	{
		status = HAL_ERROR;
	}
	else
	{
		pIwdg->Init.Reload = reload;
		pIwdg->Init.Prescaler = (prescaler / prescaler_mult) - 1U;	// Convert to register value (0-7)

		status = HAL_IWDG_Init(pIwdg);
	}

	return status;
}

uint32_t NONNULL__ WDG_ex_get_IWDG_Period_us(const IWDG_HandleTypeDef * const pIwdg)
{
	const uint32_t reload = pIwdg->Init.Reload;
	const uint32_t prescaler = (pIwdg->Init.Prescaler + 1U) * prescaler_mult;

	return ((reload * prescaler * 1000U) / refCLK);
}
#endif


HAL_StatusTypeDef NONNULL__ WDG_ex_Init(void)
{
	HAL_StatusTypeDef status = HAL_OK;

	#if defined(HAL_IWDG_MODULE_ENABLED)
	MX_IWDG_Init();
	iwdg_en = true;
	#endif

	#if defined(HAL_WWDG_MODULE_ENABLED)
	MX_WWDG_Init();
	wwdg_en = true;
	#endif

	WDG_ex_save_cfg();

	return status;
}


/********************************************************************************/
#endif	/* defined(HAL_IWDG_MODULE_ENABLED) || defined(HAL_WWDG_MODULE_ENABLED) */
/********************************************************************************/
