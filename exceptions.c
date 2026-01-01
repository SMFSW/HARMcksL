/*!\file exceptions.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Debug tool helpers
**/
/****************************************************************/
#include <stdio.h>

#include "sarmfsw.h"
#include "stdream_rdir.h"
#include "exceptions.h"
/****************************************************************/


/*!\brief prints contents of exception stack
** \param[in] stack - pointer to stack address
** \note When Exception (Hard fault, ...) occurs, contents of global register are stacked
** \note print_exception_stack should not be called directly, unless a particular stack is
** 		 needed use stack_dump() (from stack_utils.h) to get execution dump
** \return Nothing
**/
static void NONNULL__ print_exception_stack(const uint32_t stack[])
{
	enum { r0, r1, r2, r3, r12, lr, pc, psr };

	UNUSED_RET printf("stack addr = %p\r\n", stack);
	UNUSED_RET printf("r0  = %#08lx\r\n", stack[r0]);
	UNUSED_RET printf("r1  = %#08lx\r\n", stack[r1]);
	UNUSED_RET printf("r2  = %#08lx\r\n", stack[r2]);
	UNUSED_RET printf("r3  = %#08lx\r\n", stack[r3]);
	UNUSED_RET printf("r12 = %#08lx\r\n", stack[r12]);
	UNUSED_RET printf("lr  = %#08lx\r\n", stack[lr]);
	UNUSED_RET printf("pc  = %#08lx\r\n", stack[pc]);
	UNUSED_RET printf("psr = %#08lx\r\n", stack[psr]);
}


void NONNULL__ HardFault_Handler_callback(const uint32_t stack[])
{
	UNUSED_RET printf("Hard Fault handler\t");

	#if defined(__CORTEX_M)
		#if (__CORTEX_M == 4U) || (__CORTEX_M == 7U)
			UNUSED_RET printf("SCB->HFSR = %#08lx\r\n", SCB->HFSR);

			if ((SCB->HFSR & (1UL << 30U)) != 0U)
			{
				uint32_t CFSRValue = SCB->CFSR;

				UNUSED_RET printf("Hard Fault\t");
				UNUSED_RET printf("SCB->CFSR = %#08lx\r\n", CFSRValue);
				if ((SCB->CFSR & 0xFFFF0000UL) != 0U)
				{
					UNUSED_RET printf("Usage fault: ");
					CFSRValue >>= 16U;	// right shift to lsb
					if((CFSRValue & (1UL << 9U)) != 0U)
					{
						UNUSED_RET printf("Zero div\r\n");
					}
				}

				if ((SCB->CFSR & 0xFF00UL) != 0U)
				{
					CFSRValue = ((CFSRValue & 0x0000FF00) >> 8); // mask and shift
					UNUSED_RET printf("Bus fault: %#02lx\r\n", CFSRValue);
				}

				if ((SCB->CFSR & 0xFFUL) != 0U) {
					CFSRValue &= 0x000000FFUL; // mask other faults
					UNUSED_RET printf("Memory Management fault: %#02lx\r\n", CFSRValue);
				}
			}
		#endif
	#endif

	print_exception_stack(stack);

//	__BKPT(01);
	while(1);
}


void NONNULL__ Error_Handler_callback(const uint32_t stack[])
{
	// TODO: maybe pass by another asm code to retrieve HAL error code if not in stack
	UNUSED_RET printf("Error handler\r\n");
	print_exception_stack(stack);

//	__BKPT(01);
//	while(1);
}


eResetSource Get_Reset_Source(void)
{
	eResetSource rst = RST_UNKNOWN;

	if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))					{ rst = RST_PIN; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))				{ rst = RST_SW; }

	#if defined(STM32L4)
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_FWRST))				{ rst = RST_FW; }
	#elif defined(STM32C0) || defined(STM32G0)
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST))				{ rst = RST_POR; }
	#elif defined(STM32L5) || defined(STM32U0) || defined(STM32U3) || defined(STM32U5)
	// No such flag on STM32Ux families
	#else
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))				{ rst = RST_POR; }
	#endif

	#if defined(STM32L0) ||	defined(STM32L1) ||	defined(STM32L4) ||	defined(STM32L5) ||	\
		defined(STM32H7) || defined(STM32U3) || defined(STM32U5)
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))				{ rst = RST_BOR; }
	#endif

	#if defined(STM32H7)
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST))				{ rst = RST_IWDG; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDG1RST))				{ rst = RST_WWDG; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWR1RST))				{ rst = RST_LPWR; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWR2RST))				{ rst = RST_LPWR; }
	#else
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))				{ rst = RST_IWDG; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))				{ rst = RST_WWDG; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))				{ rst = RST_LPWR; }
	#endif

	#if defined(STM32C0) || defined(STM32F3) || defined(STM32G0) ||						\
		defined(STM32L0) ||	defined(STM32L1) ||	defined(STM32L4) ||	defined(STM32L5) ||	\
		defined(STM32U0) || defined(STM32U3) || defined(STM32U5)
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST))				{ rst = RST_OBL; }
	#endif

	#if defined(STM32F301x8) ||													\
		defined(STM32F302x8) || defined(STM32F302xC) || defined(STM32F302xE) ||	\
		defined(STM32F303x8) || defined(STM32F303xC) || defined(STM32F303xE) ||	\
		defined(STM32F334x8) ||	defined(STM32F358xx) || defined(STM32F373xC)
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_V18PWRRST))			{ rst = RST_V18PWR; }
	#endif

	__HAL_RCC_CLEAR_RESET_FLAGS();	// Clear reset flags

	return rst;
}
