/*!\file WDG_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief Extensions for WDG peripherals
** \details This module is mostly meant for debug target purposes,
**			giving the ability to call freeze/unfreeze watchdogs functions
**			no matter the build target, watchdogs being unfrozen only when
**			they were enabled in the first place.
** \warning For this module to work properly, WDG_init_check have to be called once
**			at the end of your init routine prior to use freeze/unfreeze functions.
**/
/****************************************************************/
#include "sarmfsw.h"
/****************************************************************/


#if defined(HAL_IWDG_MODULE_ENABLED)
static bool iwdg_en = false;
#endif

#if defined(HAL_WWDG_MODULE_ENABLED)
static bool wwdg_en = false;
#endif


void WDG_init_check(void)
{
#if defined(HAL_IWDG_MODULE_ENABLED)
	if (!(DBGMCU->APB1FZ & (DBGMCU_APB1_FZ_DBG_IWDG_STOP)))	{ iwdg_en = true; }
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	if (!(DBGMCU->APB1FZ & (DBGMCU_APB1_FZ_DBG_WWDG_STOP)))	{ wwdg_en = true; }
#endif
}

void WDG_freeze(void)
{
#if defined(HAL_IWDG_MODULE_ENABLED)
	if (iwdg_en)	{ __HAL_DBGMCU_FREEZE_IWDG(); }
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	if (wwdg_en)	{ __HAL_DBGMCU_FREEZE_WWDG(); }
#endif
}

void WDG_unfreeze(void)
{
#if defined(HAL_IWDG_MODULE_ENABLED)
	if (iwdg_en)
	{
		HAL_IWDG_Refresh(&hiwdg);
		__HAL_DBGMCU_UNFREEZE_IWDG();
	}
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	if (wwdg_en)
	{
		HAL_WWDG_Refresh(&hwwdg);
		__HAL_DBGMCU_UNFREEZE_WWDG();
	}
#endif
}
