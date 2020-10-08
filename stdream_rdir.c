/*!\file stdream_rdir.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
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

#if (defined(UART_REDIRECT) || defined(ITM_REDIRECT)) && !STDREAM_RDIR_SND_SYSCALLS
#warning "Using ITM or UART stream redirection, you should consider using syscalls, the old printf likes shadowing way is only kept for compatibility!"
#endif
/****************************************************************/

#if defined(ITM) || !STDREAM_RDIR_SND_SYSCALLS
char dbg_msg_out[SZ_DBG_OUT] = "";		//!< stdream buffer for output
#endif


#if (defined(UART_REDIRECT) || defined(ITM_REDIRECT)) && STDREAM_RDIR_SND_SYSCALLS
#if defined(USE_IO_PUTCHAR)

int __io_putchar(int ch)
{
	static bool init = false;

	if (!init)
	{
		setvbuf(stdout, NULL, _IONBF, 0);
		init = true;
	}

	#if defined(ITM) && defined(ITM_REDIRECT)
		ITM_SendChar(ch);
	#endif

	#if defined(HAL_UART_MODULE_ENABLED) && defined(UART_REDIRECT)
		if (HAL_UART_Transmit(dbg_uart, (uint8_t *) &ch, 1, 30))	{ return -1; }
	#endif

	return ch;
}

#else

int _write(int file, char * ptr, int len)
{
	#if defined(ITM) && defined(ITM_REDIRECT)
		ITM_port_send(ptr, len, 0);
	#endif

	#if defined(HAL_UART_MODULE_ENABLED) && defined(UART_REDIRECT)
		if (UART_Term_Send(dbg_uart, ptr, len))	{ return -1; }
	#endif

//	for (int DataIdx = 0 ; DataIdx < len ; DataIdx++)
//	{
//		__io_putchar(*ptr++);
//	}

	return len;
}

#endif
#endif


#if defined(UART_REDIRECT) && STDREAM_RDIR_RCV_SYSCALLS
// FIXME: bad implementation (and not working)
int __io_getchar(void)
{
	static bool init = false;
	int ch;

	if (!init)
	{
		//setvbuf(stdin, NULL, _IONBF, 0);
		init = true;
	}

	#if defined(HAL_UART_MODULE_ENABLED) && defined(UART_REDIRECT)
		if (HAL_UART_Receive(dbg_uart, (uint8_t *) &ch, 1, 1))		{ return -1; }
		UART_Term_Char_Handler(dbg_uart, ch);
	#endif

	return ch;
}

//int _read(int file, char * ptr, int len)
//{
//	for (int DataIdx = 0 ; DataIdx < len ; DataIdx++)
//	{
//		*ptr++ = __io_getchar();
//	}
//	return len;
//}

#endif


/********************/
/*** ITM TRANSMIT ***/
/********************/
#if defined(ITM)
void NONNULL__ ITM_port_send(const char * str, const int len, const int port)
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
#endif


#if !STDREAM_RDIR_SND_SYSCALLS
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

	#if defined(ITM_REDIRECT) && defined(ITM)
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

	#if defined(ITM_REDIRECT) && defined(ITM)
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
#endif
