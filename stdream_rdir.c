/*!\file stdream_rdir.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Stream redirection
** \note define ITM_REDIRECT in compiler defines for stings to be printed to ITM0 port
** \note define UART_REDIRECT and DBG_SERIAL in compiler defines with an UART instance to send printf likes strings to UART
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


/********************/
/*** ITM TRANSMIT ***/
/********************/
void NONNULL__ ITM_port_send(const int port, const char * str, const int len)
{
	for (char * pStr = (char *) str ; pStr < &str[len] ; pStr++)
	{
		while (ITM->PORT[port].u32 == 0);	// Wait for port to be ready
		ITM->PORT[port].u8 = *pStr;
	}
}

/*!\brief Sends string to ITM0 port
** \param[in] str - pointer to string to send
** \param[in] len - length of string
**/
static void NONNULL__ ITM_send(const char * str, const int len)
{
	for (char * pStr = (char *) str ; pStr < &str[len] ; pStr++)
	{
		ITM_SendChar(*pStr);
	}
}


/***********************/
/*** ITM REDIRECTION ***/
/***********************/
int NONNULL__ printf_ITM(const char * str, ...)
{
	va_list args;

	va_start(args, str);
	vsprintf(dbg_msg_out, str, args);
	va_end(args);
	ITM_send(dbg_msg_out, strlen(dbg_msg_out));
	str_clr(dbg_msg_out);	// Empty string

	return 0;
}

int NONNULL__ vprintf_ITM(const char * str, va_list args)
{
	vsprintf(dbg_msg_out, str, args);
	ITM_send(dbg_msg_out, strlen(dbg_msg_out));
	str_clr(dbg_msg_out);	// Empty string

	return 0;
}


/***************************/
/*** GENERAL REDIRECTION ***/
/***************************/
int NONNULL__ printf_redir(const char * str, ...)
{
	va_list args;

	#if defined(UART_REDIRECT)
	if (UART_Term_Wait_Ready(dbg_uart) != ERROR_OK)	{ return -1; }
	#endif

	va_start(args, str);
	vsprintf(dbg_msg_out, str, args);
	va_end(args);

	#if defined(ITM_REDIRECT) || defined(UART_REDIRECT)
	uint16_t len = strlen(dbg_msg_out);
	#endif

	#if defined(ITM_REDIRECT)
	ITM_send(dbg_msg_out, len);
	#endif

	#if defined(UART_REDIRECT)
	UART_Term_Send(dbg_uart, dbg_msg_out, len);
	#if !defined(STDREAM__UART_TX_IT)
	str_clr(dbg_msg_out);	// Empty string
	#endif
	#endif

	return 0;
}


int NONNULL__ vprintf_redir(const char * str, va_list args)
{
	#if defined(UART_REDIRECT)
	if (UART_Term_Wait_Ready(dbg_uart) != ERROR_OK)	{ return -1; }
	#endif

	vsprintf(dbg_msg_out, str, args);

	#if defined(ITM_REDIRECT) || defined(UART_REDIRECT)
	uint16_t len = strlen(dbg_msg_out);
	#endif

	#if defined(ITM_REDIRECT)
	ITM_send(dbg_msg_out, len);
	#endif

	#if defined(UART_REDIRECT)
	UART_Term_Send(dbg_uart, dbg_msg_out, len);
	#if !defined(STDREAM__UART_TX_IT)
	str_clr(dbg_msg_out);	// Empty string
	#endif
	#endif

	return 0;
}
