/*!\file WDG_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief Extensions for WDG peripherals
** \details This module is mostly meant for debug target purposes,
**			giving the ability to call freeze/unfreeze watchdogs functions
**			no matter the build target, watchdogs being un-frozen only when
**			they were enabled in the first place.
** \warning Watchdogs cannot be frozen while code is running, freeze will on be relevant
** 			when execution is paused, thus no freezing possible for a time consuming operation.
** 			Workaround is to save the watchdogs configurations, modify temporarily configuration,
** 			then restore the configuration back, using \ref WDG_save_cfg & \ref WDG_restore_cfg.
** \warning For this module to work properly, \ref WDG_init_check have to be called once
**			at the end of your init routine prior to use freeze/unfreeze functions.
**/
/****************************************************************/
#include "sarmfsw.h"
#include "WDG_ex.h"

#if defined(HAL_IWDG_MODULE_ENABLED) || defined(HAL_WWDG_MODULE_ENABLED)
/****************************************************************/


#if defined(STM32F1)
#define REG_APB1	DBGMCU->CR
#define NAME_DBG	DBGMCU_
#define NAME_APB1	CR
#elif defined(STM32G0)
#define REG_APB1	DBG->APBFZ1
#define NAME_DBG	DBG_
#define NAME_APB1	APB_FZ1
#elif defined(STM32G4) || defined(STM32L4) || defined(STM32L5) || defined(STM32WB)
#define REG_APB1	DBGMCU->APB1FZR1
#define NAME_DBG	DBGMCU_
#define NAME_APB1	APB1FZR1
#elif defined(STM32H7)
#warning "STM32H7 Family wdgs have to be handled differently because of dual core & dual wdgs! Coming soon...Maybe."
#elif defined(STM32MP1)
#warning "STM32MP1 Family wdgs have to be handled differently because of dual core & dual wdgs! Coming soon...Maybe."
#elif defined(STM32WL)
#warning "STM32WL Family wdgs have to be handled differently because of dual core & dual wdgs! Coming soon...Maybe."
#elif (!(defined(STM32H7) || defined(STM32MP1) || defined(STM32WL)))
#define REG_APB1	DBGMCU->APB1FZ
#define NAME_DBG	DBGMCU_
#define NAME_APB1	APB1_FZ
#endif


static const uint32_t refCLK = LSI_VALUE / 1000U;	// Watchdog clock frequency in KHz (for 32b computations)
static const uint32_t prescaler_mult = 4U;			// STM32 IWDG STM32 architecture prescaler multiplier


#if defined(HAL_IWDG_MODULE_ENABLED)
static bool iwdg_en = false;
static IWDG_InitTypeDef IWDG_cfg = { 0 };
#endif

#if defined(HAL_WWDG_MODULE_ENABLED)
static bool wwdg_en = false;
static WWDG_InitTypeDef WWDG_cfg = { 0 };
#endif


bool WDG_get_state_IWDG(void)
{
	return (!(REG_APB1 & (XCAT(XCAT(NAME_DBG, NAME_APB1), _DBG_IWDG_STOP))));
}

bool WDG_get_state_WWDG(void)
{
	return (!(REG_APB1 & (XCAT(XCAT(NAME_DBG, NAME_APB1), _DBG_WWDG_STOP))));
}


void WDG_init_check(void)
{
#if defined(STM32H7)

#if defined(DUAL_CORE)
#if defined(CORE_CM4)
#elif defined(CORE_CM7)
#endif
#else
#endif

#elif defined(STM32MP1)

#if defined(CORE_CA7)
#elif defined(CORE_CM4)
#endif

#elif defined(STM32WL)

#if defined(CORE_CM0PLUS)
#elif defined(CORE_CM4)
#endif

#else

#if defined(HAL_IWDG_MODULE_ENABLED)
	if (WDG_get_state_IWDG())	{ iwdg_en = true; }
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	if (WDG_get_state_WWDG())	{ wwdg_en = true; }
#endif

	WDG_save_cfg();

#endif
}


void WDG_refresh(void)
{
#if defined(HAL_IWDG_MODULE_ENABLED)
	UNUSED_RET HAL_IWDG_Refresh(&hiwdg);
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	UNUSED_RET HAL_WWDG_Refresh(&hwwdg);
#endif
}


void WDG_freeze(void)
{
#if defined(STM32H7)

#if defined(DUAL_CORE)
#if defined(CORE_CM4)
#elif defined(CORE_CM7)
#endif
#else
#endif

#elif defined(STM32MP1)

#if defined(CORE_CA7)
#elif defined(CORE_CM4)
#endif

#elif defined(STM32WL)

#if defined(CORE_CM0PLUS)
#elif defined(CORE_CM4)
#endif

#else

#if defined(HAL_IWDG_MODULE_ENABLED)
	if (iwdg_en)	{ __HAL_DBGMCU_FREEZE_IWDG(); }
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	if (wwdg_en)	{ __HAL_DBGMCU_FREEZE_WWDG(); }
#endif

#endif
}


void WDG_unfreeze(void)
{
#if defined(STM32H7)

#if defined(DUAL_CORE)
#if defined(CORE_CM4)
#elif defined(CORE_CM7)
#endif
#else
#endif

#elif defined(STM32MP1)

#if defined(CORE_CA7)
#elif defined(CORE_CM4)
#endif

#elif defined(STM32WL)

#if defined(CORE_CM0PLUS)
#elif defined(CORE_CM4)
#endif

#else

#if defined(HAL_IWDG_MODULE_ENABLED)
	if (iwdg_en)
	{
		UNUSED_RET HAL_IWDG_Refresh(&hiwdg);
		__HAL_DBGMCU_UNFREEZE_IWDG();
	}
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	if (wwdg_en)
	{
		UNUSED_RET HAL_WWDG_Refresh(&hwwdg);
		__HAL_DBGMCU_UNFREEZE_WWDG();
	}
#endif

#endif
}


void WDG_save_cfg(void)
{
#if defined(HAL_IWDG_MODULE_ENABLED)
	UNUSED_RET memcpy(&IWDG_cfg, &hiwdg.Init, sizeof(IWDG_InitTypeDef));
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	UNUSED_RET memcpy(&WWDG_cfg, &hwwdg.Init, sizeof(WWDG_InitTypeDef));
#endif
}

HAL_StatusTypeDef WDG_restore_cfg(void)
{
	HAL_StatusTypeDef status = HAL_OK;

#if defined(HAL_IWDG_MODULE_ENABLED)
	UNUSED_RET memcpy(&hiwdg.Init, &IWDG_cfg, sizeof(IWDG_InitTypeDef));
	status |= HAL_IWDG_Init(&hiwdg);
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	memcpy(&hwwdg.Init, &WWDG_cfg, sizeof(WWDG_InitTypeDef));
	status |= HAL_WWDG_Init(&hwwdg);
#endif

	return status;
}


#if defined(HAL_IWDG_MODULE_ENABLED)
HAL_StatusTypeDef NONNULL__ set_IWDG_Period_us(IWDG_HandleTypeDef * const pIwdg, const uint32_t per)
{
	const uint32_t max_prescaler = 256U;		// STM32 IWDG STM32 architecture maximum prescaler
	const uint32_t max_reload = 4095U;			// STM32 IWDG STM32 architecture maximum reload value
	uint32_t reload;
	uint32_t prescaler;

	for (prescaler = prescaler_mult ; prescaler <= max_prescaler ; prescaler <<= 1U)
	{
		reload = (refCLK * per) / (1000U * prescaler);		// Computation using KHz instead of Hz to stay in 32b range
		if (reload <= max_reload)	{ break; }				// If in range
	}

	if (reload == 0U)				{ return HAL_ERROR; }	// Period is too short
	if (prescaler > max_prescaler)	{ return HAL_ERROR; }	// If nothing has been found (after last iteration)

	pIwdg->Init.Reload = reload;
	pIwdg->Init.Prescaler = (prescaler / prescaler_mult) - 1U;	// Convert to register value (0-7)

	return HAL_IWDG_Init(pIwdg);
}

uint32_t NONNULL__ get_IWDG_Period_us(const IWDG_HandleTypeDef * const pIwdg)
{
	const uint32_t reload = pIwdg->Init.Reload;
	const uint32_t prescaler = (pIwdg->Init.Prescaler + 1U) * prescaler_mult;

	return ((reload * prescaler * 1000U) / refCLK);
}
#endif


/********************************************************************************/
#endif	/* defined(HAL_IWDG_MODULE_ENABLED) || defined(HAL_WWDG_MODULE_ENABLED) */
/********************************************************************************/
