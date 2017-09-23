/*!\file exceptions.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Debug tool and helpers declaration
**/
/****************************************************************/
#ifndef __EXCEPTIONS_H
	#define __EXCEPTIONS_H

#include "sarmfsw.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
//! \note The exception_Handler should be called with corresponding exception name \b e as parameter
#define exception_Handler(e)					\
	__asm(	"tst lr, #4 \r\n"					\
			"ite EQ \r\n"						\
			"mrseq r0, MSP \r\n"				\
			"mrsne r0, PSP \r\n"				\
			"b " #e "_Handler_callback \r\n")	//!< Exception handler asm caller


#define dump_stack()				\
	__asm(	"tst lr, #4 \r\n"		\
			"ite EQ \r\n"			\
			"mrseq r0, MSP \r\n"	\
			"mrsne r0, PSP \r\n"	\
			"b stackDump \r\n")		//!< Dump stack asm caller


/*!\brief prints contents of stack
** \param[in] stack - pointer to stack address
** \note stackDump should not be called directly, unless a particular stack is needed
** 		 use dump_stack() which prepares pointer to current stack instead
** \return Nothing
**/
void stackDump(uint32_t stack[]);


/*!\brief prints informations about current Hard Fault exception
** \param[in] stack - pointer to stack address
** \note HardFault_Handler_callback should not be called directly
** 		 use exception_Handler() which prepares pointer to current stack instead
** \warning Depending how arm is fucked up, informations may not be printed,
** 			at least, you could inspect exception and stack through debug breakpoint
** \return Never (anyways, arm fubared!)
**/
void HardFault_Handler_callback(uint32_t stack[]);


/*!\brief prints informations about current Hard Fault exception
** \param[in] stack - pointer to stack address
** \note HardFault_Handler_callback should not be called directly
** 		 use exception_Handler() which prepares pointer to current stack instead
** \warning Depending how arm is fucked up, informations may not be printed,
** 			at least, you could inspect exception and stack through debug breakpoint
** \return Never (anyways, arm fubared!)
**/
void Error_Handler_callback(uint32_t stack[]);


/****************************************************************/
#endif
/****************************************************************/
