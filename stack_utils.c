/*!\file stack_utils.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Stack utilities
**/
/****************************************************************/
#include <stdlib.h>
#include <string.h>

#include "stdream_rdir.h"
#include "stack_utils.h"
/****************************************************************/


void print_stack_address(void) {
	printf("stack addr = %lx\r\n", get_pMSP()); }


void print_global_regs(void)
{
	uint32_t r0_reg = get_R0();
	uint32_t r1_reg = get_R1();
	uint32_t r2_reg = get_R2();
	uint32_t r3_reg = get_R3();
	uint32_t r4_reg = get_R4();
	uint32_t r5_reg = get_R5();
	uint32_t r6_reg = get_R6();
	uint32_t r7_reg = get_R7();
	uint32_t r8_reg = get_R8();
	uint32_t r9_reg = get_R9();
	uint32_t r10_reg = get_R10();
	uint32_t r11_reg = get_R11();
	uint32_t r12_reg = get_R12();
	//uint32_t lr_reg = get_LR();
	//uint32_t psr_reg = get_PSR();

	printf("r0  = 0x%08lx\r\n", r0_reg);
	printf("r1  = 0x%08lx\r\n", r1_reg);
	printf("r2  = 0x%08lx\r\n", r2_reg);
	printf("r3  = 0x%08lx\r\n", r3_reg);
	printf("r4  = 0x%08lx\r\n", r4_reg);
	printf("r5  = 0x%08lx\r\n", r5_reg);
	printf("r6  = 0x%08lx\r\n", r6_reg);
	printf("r7  = 0x%08lx\r\n", r7_reg);
	printf("r8  = 0x%08lx\r\n", r8_reg);
	printf("r9  = 0x%08lx\r\n", r9_reg);
	printf("r10  = 0x%08lx\r\n", r10_reg);
	printf("r11  = 0x%08lx\r\n", r11_reg);
	printf("r12 = 0x%08lx\r\n", r12_reg);
	//printf("lr  = 0x%08lx\r\n", lr_reg);
	//printf("psr  = 0x%08lx\r\n", psr_reg);
}

