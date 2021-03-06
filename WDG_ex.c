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
#include "WDG_ex.h"
/****************************************************************/


#if defined(STM32F1)
#define REG_APB1	DBGMCU->CR
#define NAME_APB1	CR
#elif defined(STM32G4) || defined(STM32L4) || defined(STM32L5) || defined(STM32WB)
#define REG_APB1	DBGMCU->APB1FZR1
#define NAME_APB1	APB1FZR1
#elif defined(STM32H7)
#warning "STM32H7 Family wdgs have to be handled differently because of dual core & dual wdgs! Coming soon..."
#elif defined(STM32MP1)
#warning "STM32MP1 Family wdgs have to be handled differently because of dual core & dual wdgs! Coming soon..."
#elif defined(STM32WL)
#warning "STM32WL Family wdgs have to be handled differently because of dual core & dual wdgs! Coming soon..."
#elif (!(defined(STM32H7) || defined(STM32MP1) || defined(STM32WL)))
#define REG_APB1	DBGMCU->APB1FZ
#define NAME_APB1	APB1_FZ
#endif


#if defined(HAL_IWDG_MODULE_ENABLED)
static bool iwdg_en = false;
#endif

#if defined(HAL_WWDG_MODULE_ENABLED)
static bool wwdg_en = false;
#endif


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
	if (!(REG_APB1 & (XCAT(XCAT(DBGMCU_, NAME_APB1), _DBG_IWDG_STOP))))	{ iwdg_en = true; }
#endif
#if defined(HAL_WWDG_MODULE_ENABLED)
	if (!(REG_APB1 & (XCAT(XCAT(DBGMCU_, NAME_APB1), _DBG_WDG_STOP))))	{ wwdg_en = true; }
#endif

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

#endif
}
