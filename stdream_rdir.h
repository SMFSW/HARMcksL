/*!\file stream_redirect.h
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Stream redirection header
*/
/****************************************************************/
#ifndef __STREAM_REDIRECT_H
	#define __STREAM_REDIRECT_H
/****************************************************************/


#include <stdarg.h>

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG


#if defined(HAL_UART_MODULE_ENABLED)
#include "usart.h"

#define	printf		uart_printf		// Shadowing printf use
#define	vprintf		uart_vprintf	// Shadowing vprintf use


int	uart_printf(char *string, ...);
int	uart_vprintf(char *string, va_list args);

#endif


/****************************************************************/
#endif
/****************************************************************/
