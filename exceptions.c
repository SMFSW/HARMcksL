/*!\file exceptions.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief Debug tool helpers
**/
/****************************************************************/
#include <string.h>

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
	
	printf("stack addr = %lx\r\n", stack);
	printf("r0  = 0x%08lx\r\n", stack[r0]);
	printf("r1  = 0x%08lx\r\n", stack[r1]);
	printf("r2  = 0x%08lx\r\n", stack[r2]);
	printf("r3  = 0x%08lx\r\n", stack[r3]);
	printf("r12 = 0x%08lx\r\n", stack[r12]);
	printf("lr  = 0x%08lx\r\n", stack[lr]);
	printf("pc  = 0x%08lx\r\n", stack[pc]);
	printf("psr = 0x%08lx\r\n", stack[psr]);
}


void NONNULL__ HardFault_Handler_callback(const uint32_t stack[])
{
	printf("Hard Fault handler\t");

	#if !defined(STM32G0)
		printf("SCB->HFSR = 0x%08lx\r\n", SCB->HFSR);

		if ((SCB->HFSR & (1 << 30)) != 0)
		{
			uint32_t CFSRValue = SCB->CFSR;

			printf("Hard Fault\t");
			printf("SCB->CFSR = 0x%08lx\r\n", CFSRValue);
			if ((SCB->CFSR & 0xFFFF0000) != 0)
			{
				printf("Usage fault: ");
				CFSRValue >>= 16;	// right shift to lsb
				if((CFSRValue & (1 << 9)) != 0) { printf("Zero div\r\n"); }
			}

			if ((SCB->CFSR & 0xFF00) != 0)
			{
				CFSRValue = ((CFSRValue & 0x0000FF00) >> 8); // mask and shift
				printf("Bus fault: 0x%02lx\r\n", CFSRValue);
			}

			if ((SCB->CFSR & 0xFF) != 0) {
				CFSRValue &= 0x000000FF; // mask other faults
				printf("Memory Management fault: 0x%02lx\r\n", CFSRValue);
			}
		}
	#endif

	print_exception_stack(stack);

//	__BKPT(01);
	while(1);
}


void NONNULL__ Error_Handler_callback(const uint32_t stack[])
{
	// TODO: maybe pass by another asm code to retrieve HAL error code if not in stack
	printf("Error handler\r\n");
	print_exception_stack(stack);

//	__BKPT(01);
//	while(1);
}


eResetSource Get_Reset_Source(void)
{
	eResetSource rst = RST_UNKNOWN;

	#if defined(STM32G0)
		if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST))				{ rst = RST_POR; }
	#else
		if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))				{ rst = RST_POR; }
	#endif
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))				{ rst = RST_PIN; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))				{ rst = RST_SW; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))				{ rst = RST_IWDG; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))				{ rst = RST_WWDG; }
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))				{ rst = RST_LPWR; }
	#if defined(STM32F3)
		else if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST))			{ rst = RST_OBL; }
		#if defined(STM32F301x8) ||													\
			defined(STM32F302x8) || defined(STM32F302xC) || defined(STM32F302xE) ||	\
			defined(STM32F303x8) || defined(STM32F303xC) || defined(STM32F303xE) ||	\
			defined(STM32F334x8) ||	defined(STM32F358xx) || defined(STM32F373xC)
			else if (__HAL_RCC_GET_FLAG(RCC_FLAG_V18PWRRST))	{ rst = RST_V18PWR; }
		#endif
	#endif

	__HAL_RCC_CLEAR_RESET_FLAGS();	// Clear reset flags

	return rst;
}
