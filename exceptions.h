/*!\file exceptions.h
** \author SMFSW
** \copyright MIT (c) 2017-2022, SMFSW
** \brief Debug tool and helpers
**/
/****************************************************************/
#ifndef __EXCEPTIONS_H
	#define __EXCEPTIONS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
#if defined(STM32G0)
//! \note The exception_Handler should be called with corresponding exception name \b e as parameter
#define exception_Handler(e)					\
	__ASM (	"mrs r0, MSP \r\n"					\
			"bl " #e "_Handler_callback \r\n");	//!< Exception handler asm caller
#else
//! \note The exception_Handler should be called with corresponding exception name \b e as parameter
#define exception_Handler(e)					\
	__ASM (	"tst lr, #4 \r\n"					\
			"ite EQ \r\n"						\
			"mrseq r0, MSP \r\n"				\
			"mrsne r0, PSP \r\n"				\
			"b " #e "_Handler_callback \r\n");	//!< Exception handler asm caller
#endif


/*!\brief prints informations about current Hard Fault exception
** \param[in] stack - pointer to stack address
** \note HardFault_Handler_callback should not be called directly
** 		 use exception_Handler() which prepares pointer to current stack instead
** \warning Depending how arm is fucked up, informations may not be printed,
** 			at least, you could inspect exception and stack through debug breakpoint
** \return Never (anyways, arm fubared!)
**/
void NONNULL__ HardFault_Handler_callback(const uint32_t stack[]);


/*!\brief prints informations about current Hard Fault exception
** \param[in] stack - pointer to stack address
** \note HardFault_Handler_callback should not be called directly
** 		 use exception_Handler() which prepares pointer to current stack instead
** \warning Depending how arm is fucked up, informations may not be printed,
** 			at least, you could inspect exception and stack through debug breakpoint
** \return Never (anyways, arm fubared!)
**/
void NONNULL__ Error_Handler_callback(const uint32_t stack[]);


/*!\brief Get the last source of reset
** \warning This function should be called soon after reset (before or after SystemClock_Config())
** \note This function using some special macros can't be inlined, thus can't be in sarmfsw stm32 header unfortunately
** \return Last Reset Source
**/
eResetSource Get_Reset_Source(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __EXCEPTIONS_H */
/****************************************************************/
