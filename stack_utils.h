/*!\file stack_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief Stack utilities
**/
/****************************************************************/
#ifndef __STACK_UTILS_H
	#define __STACK_UTILS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/
// TODO: see about volatile qualifier


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get Main Stack Pointer value
** \return Main Stack Pointer value
**/
__INLINE uint32_t INLINE__ get_MSP(void)
{
	uint32_t msp;
	__ASM volatile ("mrs %0, MSP \r\n" : "=r" (msp));
	return(msp);
}


/*!\brief Get Current Stack Pointer value
** \return Current Stack Pointer value
**/
__INLINE uint32_t INLINE__ get_SP(void)
{
	uint32_t sp;
	__ASM volatile ("tst lr, #4 \r\n"	\
					"ite EQ \r\n");
	__ASM volatile ("mrseq %0, MSP \r\n" : "=r" (sp));
	__ASM volatile ("mrsne %0, PSP \r\n" : "=r" (sp));
	return(sp);
}


/*!\brief Get pointer to the current stack
** \return Current stack pointer
**/
__INLINE uint32_t * INLINE__ get_pSP(void) {
	return (uint32_t *) get_SP(); }

/*!\brief Get pointer to the main stack
** \return Main stack pointer
**/
__INLINE uint32_t * INLINE__ get_pMSP(void) {
	return (uint32_t *) get_MSP(); }


/*!\brief Get content of R0 register
** \return R0 register value
**/
__INLINE uint32_t INLINE__ get_R0(void) {
	register uint32_t R0 __ASM("r0");
	return R0; }

/*!\brief Get content of R1 register
** \return R1 register value
**/
__INLINE uint32_t INLINE__ get_R1(void) {
	register uint32_t R1 __ASM("r1");
	return R1; }

/*!\brief Get content of R2 register
** \return R2 register value
**/
__INLINE uint32_t INLINE__ get_R2(void) {
	register uint32_t R2 __ASM("r2");
	return R2; }

/*!\brief Get content of R3 register
** \return R3 register value
**/
__INLINE uint32_t INLINE__ get_R3(void) {
	register uint32_t R3 __ASM("r3");
	return R3; }

/*!\brief Get content of R4 register
** \return R4 register value
**/
__INLINE uint32_t INLINE__ get_R4(void) {
	register uint32_t R4 __ASM("r4");
	return R4; }

/*!\brief Get content of R5 register
** \return R5 register value
**/
__INLINE uint32_t INLINE__ get_R5(void) {
	register uint32_t R5 __ASM("r5");
	return R5; }

/*!\brief Get content of R6 register
** \return R6 register value
**/
__INLINE uint32_t INLINE__ get_R6(void) {
	register uint32_t R6 __ASM("r6");
	return R6; }

/*!\brief Get content of R7 register
** \return R7 register value
**/
__INLINE uint32_t INLINE__ get_R7(void) {
	register uint32_t R7 __ASM("r7");
	return R7; }

/*!\brief Get content of R8 register
** \return R8 register value
**/
__INLINE uint32_t INLINE__ get_R8(void) {
	register uint32_t R8 __ASM("r8");
	return R8; }

/*!\brief Get content of R9 register
** \return R9 register value
**/
__INLINE uint32_t INLINE__ get_R9(void) {
	register uint32_t R9 __ASM("r9");
	return R9; }

/*!\brief Get content of R10 register
** \return R10 register value
**/
__INLINE uint32_t INLINE__ get_R10(void) {
	register uint32_t R10 __ASM("r10");
	return R10; }

/*!\brief Get content of R11 register
** \return R11 register value
**/
__INLINE uint32_t INLINE__ get_R11(void) {
	register uint32_t R11 __ASM("r11");
	return R11; }

/*!\brief Get content of R12 register
** \return R12 register value
**/
__INLINE uint32_t INLINE__ get_R12(void) {
	register uint32_t R12 __ASM("r12");
	return R12; }

/*!\brief Get content of link register
** \return Link register value
**/
__INLINE uint32_t INLINE__ get_LR(void) {
	register uint32_t LR __ASM("r14");	// r14: Link Register
	return LR; }


/*!\brief Get content of xPSR
** \return Current program status register value
**/
__INLINE uint32_t INLINE__ get_PSR(void) {
	uint32_t psr;
	__ASM volatile ("mrs %0, XPSR" : "=r" (psr));
	return(psr); }


/*!\brief Prints main stack address
** \note This function is for debug purposes while running only
** \return Nothing
**/
void print_stack_address(void);

/*!\brief Print contents of ARM registers
** \note This function is for debug purposes while running only
** \return Nothing
**/
void print_global_regs(void);


/*!\brief prints contents of global registers & stack address
** \return Nothing
**/
__INLINE void INLINE__ stack_dump(void) {
	print_global_regs();
	print_stack_address(); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __STACK_UTILS_H */
/****************************************************************/
