/*!\file exceptions.c
** \author SMFSW
** \version v0.6
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Debug tool helpers functions
**/
/****************************************************************/
/****************************************************************/

#include <string.h>

#include "exceptions.h"

#include "stdream_rdir.h"


void stackDump(uint32_t stack[])
{
	enum { r0, r1, r2, r3, r12, lr, pc, psr};

	printf("r0  = 0x%08lx\r\n", stack[r0]);
	printf("r1  = 0x%08lx\r\n", stack[r1]);
	printf("r2  = 0x%08lx\r\n", stack[r2]);
	printf("r3  = 0x%08lx\r\n", stack[r3]);
	printf("r12 = 0x%08lx\r\n", stack[r12]);
	printf("lr  = 0x%08lx\r\n", stack[lr]);
	printf("pc  = 0x%08lx\r\n", stack[pc]);
	printf("psr = 0x%08lx\r\n", stack[psr]);
}


void HardFault_Handler_callback(uint32_t stack[])
{
	printf("Hard Fault handler\t");
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

	stackDump(stack);

	__BKPT(01);
	while(1);
}


void Error_Handler_callback(uint32_t stack[])
{
	// TODO: maybe pass by another asm code to retrieve HAL error code if not in stack
	printf("Error handler\r\n");
	stackDump(stack);

//	__BKPT(01);
//	while(1);
}

