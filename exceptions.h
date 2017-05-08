/*!\file exceptions.h
** \author SMFSW
** \version v0.5
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Debug tool and helpers declaration
**/
/****************************************************************/
#ifndef __EXCEPTIONS_H
	#define __EXCEPTIONS_H

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
#define exception_Handler(e)					\
	__asm(	"tst lr, #4 \n"						\
			"ite EQ \n"							\
			"mrseq r0, MSP \n"					\
			"mrsne r0, PSP \n"					\
			"b " #e "_Handler_callback \n")		//!< The exception_Handler should be called with corresponding exception name \b e as parameter


#define dump_stack()			\
	__asm(	"tst lr, #4 \n"		\
			"ite EQ \n"			\
			"mrseq r0, MSP \n"	\
			"mrsne r0, PSP \n"	\
			"b stackDump \n")


/* Handled callbacks for reference
** (not really needed as called by assembly from macro)
** use macros to pass stack pointer properly */
void HardFault_Handler_callback(uint32_t stack[]);	// HardFault handler
void Error_Handler_callback(uint32_t stack[]);		// HAL Error handler


/****************************************************************/
#endif
/****************************************************************/
