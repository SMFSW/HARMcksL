/*!\file stream_redirect.c
** \author SMFSW
** \version v0.1
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Stream redirection
*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "stdream_rdir.h"


#if defined(HAL_UART_MODULE_ENABLED)

#if !defined(DBG_SERIAL)
#error "You have to define DBG_SERIAL in usart.h with an UART instance for this to work!"
#endif


static char buf_stream[128] = "";


// Used to redirect stuff printed with printf and puts
/*int _write(int file, char *ptr, int len)
{
	int i=0;
	for(i=0 ; i<len ; i++)
		ITM_SendChar((*ptr++));
	return len;
}*/


__STATIC_INLINE void __attribute__((always_inline)) uart_prt(char* ptr, int len)
{
	HAL_UART_Transmit(DBG_SERIAL, (uint8_t *) ptr, len, 30);
}


int uart_printf(char *string, ...)
{
#if defined(IS_DEBUG)
	va_list args;

	va_start(args, string);
	vsprintf(buf_stream, string, args);
	va_end(args);
	uart_prt(buf_stream, strlen(buf_stream));
	buf_stream[0] = '\0';	// Erase string

	return 0;
#else
	return -1;
#endif
}


int uart_vprintf(char *string, va_list args)
{
#if defined(IS_DEBUG)
	vsprintf(buf_stream, string, args);
	uart_prt(buf_stream, strlen(buf_stream));
	buf_stream[0] = '\0';	// Erase string

	return 0;
#else
	return -1;
#endif
}


#endif
