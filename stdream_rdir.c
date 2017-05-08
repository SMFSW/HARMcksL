/*!\file stdream_rdir.c
** \author SMFSW
** \version v0.5
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Stream redirection
*/
/****************************************************************/
/****************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "stdream_rdir.h"

#include "arm_stdclib.h"


static char buf_stream[128] = "";


#if defined(HAL_UART_MODULE_ENABLED)
#include "usart.h"

#if !defined(DBG_SERIAL)
#warning "You have to define DBG_SERIAL in usart.h with an UART instance for this to work!"
#endif

__STATIC_INLINE void INLINE__ print_uart(char* ptr, int len)
{
	HAL_UART_Transmit(DBG_SERIAL, (uint8_t *) ptr, len, 30);
}

#endif


static void print_itm(const char * msg, int len)
{
	//while (*msg != '\0')
	for (int i = 0 ; i < len ; i++)
		ITM_SendChar(*msg++);
}


void print_itm_port(int port, const char * msg, int len)
{
	for (int i = 0 ; i < len ; i++)
	{
		while (ITM->PORT[port].u32 == 0);
		ITM->PORT[port].u8 = *msg++;
	}
}


/********************/
/*** PRINTF LIKES ***/
/********************/

/*** ITM ***/
int printf_ITM(char * string, ...)
{
	va_list args;

	va_start(args, string);
	vsprintf(buf_stream, string, args);
	va_end(args);
	print_itm(buf_stream, strlen(buf_stream));
	str_clr(buf_stream);	// Erase string
	return 0;
}

int vprintf_ITM(char * string, va_list args)
{
	vsprintf(buf_stream, string, args);
	print_itm(buf_stream, strlen(buf_stream));
	str_clr(buf_stream);	// Erase string
	return 0;
}


/*** GENERAL REDIRECTION ***/
int printf_rdir(char * string, ...)
{
	uint16_t len;
	va_list args;

	va_start(args, string);
	vsprintf(buf_stream, string, args);
	va_end(args);
	len = strlen(buf_stream);

#if defined(ITM_ENABLED)
	print_itm(buf_stream, len);
#endif

#if defined(DBG_SERIAL)
	print_uart(buf_stream, len);
#endif

	str_clr(buf_stream);	// Erase string
	return 0;
}


int vprintf_rdir(char * string, va_list args)
{
	uint16_t len;

	vsprintf(buf_stream, string, args);
	len = strlen(buf_stream);

#if defined(ITM_ENABLED)
	print_itm(buf_stream, len);
#endif

#if defined(DBG_SERIAL)
	print_uart(buf_stream, len);
#endif

	str_clr(buf_stream);	// Erase string
	return 0;
}

