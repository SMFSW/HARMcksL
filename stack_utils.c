/*!\file stack_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Stack utilities
**/
/****************************************************************/
#include <stdio.h>

#include "stdream_rdir.h"
#include "stack_utils.h"
/****************************************************************/


void print_stack_address(void) {
	UNUSED_RET printf("stack addr = %p\r\n", get_pMSP()); }


void print_global_regs(void)
{
	const uint32_t r0_reg = get_R0();
	const uint32_t r1_reg = get_R1();
	const uint32_t r2_reg = get_R2();
	const uint32_t r3_reg = get_R3();
	const uint32_t r4_reg = get_R4();
	const uint32_t r5_reg = get_R5();
	const uint32_t r6_reg = get_R6();
	const uint32_t r7_reg = get_R7();
	const uint32_t r8_reg = get_R8();
	const uint32_t r9_reg = get_R9();
	const uint32_t r10_reg = get_R10();
	const uint32_t r11_reg = get_R11();
	const uint32_t r12_reg = get_R12();
	//const uint32_t lr_reg = get_LR();
	//const uint32_t psr_reg = get_PSR();

	UNUSED_RET printf("r0  = %#08lx\r\n", r0_reg);
	UNUSED_RET printf("r1  = %#08lx\r\n", r1_reg);
	UNUSED_RET printf("r2  = %#08lx\r\n", r2_reg);
	UNUSED_RET printf("r3  = %#08lx\r\n", r3_reg);
	UNUSED_RET printf("r4  = %#08lx\r\n", r4_reg);
	UNUSED_RET printf("r5  = %#08lx\r\n", r5_reg);
	UNUSED_RET printf("r6  = %#08lx\r\n", r6_reg);
	UNUSED_RET printf("r7  = %#08lx\r\n", r7_reg);
	UNUSED_RET printf("r8  = %#08lx\r\n", r8_reg);
	UNUSED_RET printf("r9  = %#08lx\r\n", r9_reg);
	UNUSED_RET printf("r10 = %#08lx\r\n", r10_reg);
	UNUSED_RET printf("r11 = %#08lx\r\n", r11_reg);
	UNUSED_RET printf("r12 = %#08lx\r\n", r12_reg);
	//UNUSED_RET printf("lr  = %#08lx\r\n", lr_reg);
	//UNUSED_RET printf("psr = %#08lx\r\n", psr_reg);
}

