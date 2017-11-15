/*!\file stdream_rdir.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Stream redirection
*/
/****************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "sarmfsw.h"

#include "UART_term.h"
#include "stdream_rdir.h"
/****************************************************************/


char dbg_msg_out[SZ_DBG_OUT] = "";		//!< stdream buffer for output
#if defined(UART_REDIRECT)
char dbg_msg_in[SZ_DBG_IN + 1] = "";	//!< stdream buffer for input
#endif


/*!\brief Sends string to ITM0 port
** \param[in] str - pointer to string to send
** \param[in] len - length of string
** \return Nothing
**/
static void ITM_send(const char * str, const int len)
{
	//while (*str != '\0')
	for (int i = 0 ; i < len ; i++)
	{
		ITM_SendChar(*str++);
	}
}


/*!\brief Sends string to chosen ITM port
** \param[in] port - ITM port number
** \param[in] str - pointer to string to send
** \param[in] len - length of string
** \return Nothing
**/
void ITM_port_send(const int port, const char * str, const int len)
{
	for (int i = 0 ; i < len ; i++)
	{
		while (ITM->PORT[port].u32 == 0);	// Wait for port to be ready
		ITM->PORT[port].u8 = *str++;
	}
}


/********************/
/*** PRINTF LIKES ***/
/********************/

/*** ITM ***/
int printf_ITM(const char * str, ...)
{
	va_list args;

	va_start(args, str);
	vsprintf(dbg_msg_out, str, args);
	va_end(args);
	ITM_send(dbg_msg_out, strlen(dbg_msg_out));
	str_clr(dbg_msg_out);	// Empty string

	return 0;
}

int vprintf_ITM(const char * str, va_list args)
{
	vsprintf(dbg_msg_out, str, args);
	ITM_send(dbg_msg_out, strlen(dbg_msg_out));
	str_clr(dbg_msg_out);	// Empty string

	return 0;
}


/*** GENERAL REDIRECTION ***/
int printf_rdir(const char * str, ...)
{
	uint16_t	len;
	va_list		args;

	#if defined(UART_REDIRECT)
	if(SERIAL_DBG_Wait_Ready(dbg_uart) != ERROR_OK)	{ return -1; }
	#endif

	va_start(args, str);
	vsprintf(dbg_msg_out, str, args);
	va_end(args);
	len = strlen(dbg_msg_out);

	#if defined(ITM_ENABLED)
	ITM_send(dbg_msg_out, len);
	#endif

	#if defined(UART_REDIRECT)
	SERIAL_DBG_Send(dbg_uart, dbg_msg_out, len);
	#endif

	#if !defined(STDREAM__UART_TX_IT)
	str_clr(dbg_msg_out);	// Empty string
	#endif

	UNUSED(len);	// To prevent compilation warnings
	return 0;
}


int vprintf_rdir(const char * str, va_list args)
{
	uint16_t len;

	#if defined(UART_REDIRECT)
	if(SERIAL_DBG_Wait_Ready(dbg_uart) != ERROR_OK)	{ return -1; }
	#endif

	vsprintf(dbg_msg_out, str, args);
	len = strlen(dbg_msg_out);

	#if defined(ITM_ENABLED)
	ITM_send(dbg_msg_out, len);
	#endif

	#if defined(UART_REDIRECT)
	SERIAL_DBG_Send(dbg_uart, dbg_msg_out, len);
	#endif

	#if !defined(STDREAM__UART_TX_IT)
	str_clr(dbg_msg_out);	// Empty string
	#endif

	UNUSED(len);	// To prevent compilation warnings
	return 0;
}
