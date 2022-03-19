/*!\file stdream_rdir.c
** \author SMFSW
** \copyright MIT (c) 2017-2022, SMFSW
** \brief Stream redirection
** \note define ITM_REDIRECT in compiler defines for stings to be printed to ITM0 port
** \note define UART_REDIRECT and DBG_SERIAL in compiler defines with an UART instance to send printf likes strings to UART
** \warning By default when using syscalls, stdout stream is buffered, meaning that output will only be processed when
**			'\\n' (new line) character is sent to buffer. To override this behavior, \ref stdout_no_buffer has to be called once
**			in init routine to disable buffering, thus processing characters as they come.
*/
/****************************************************************/
#include <stdio.h>

#include "sarmfsw.h"

#include "UART_term.h"
#include "stdream_rdir.h"
/****************************************************************/

#if (defined(UART_REDIRECT) || defined(ITM_REDIRECT)) && !STDREAM_RDIR_SND_SYSCALLS
#warning "For ITM/UART stream redirection, you should consider using syscalls. printf likes shadowing way is only kept for backward compatibility!"

char dbg_msg_out[SZ_DBG_OUT] = "";	//!< stdream buffer for output
#endif


#if (defined(UART_REDIRECT) || defined(ITM_REDIRECT)) && STDREAM_RDIR_SND_SYSCALLS
#if defined(USE_IO_PUTCHAR)

/*!\brief syscalls implementation of __io_putchar
** \strong Strong implementation of __io_putchar for UART & ITM char send to stream
** \param[in] ch - character to send to stream
** \return char written to stream (-1 otherwise)
** \retval -1 is returned in case of failure (UART)
**/
int __io_putchar(int ch)
{
	#if defined(ITM) && defined(ITM_REDIRECT)
	ITM_SendChar(ch);
	#endif

	#if defined(HAL_UART_MODULE_ENABLED) && defined(UART_REDIRECT)
	const char snd = ch;
	if (UART_Term_Send(dbg_uart, &snd, 1))	{ return -1; }
	#endif

	return ch;
}

#else

/*!\brief syscalls implementation of _write
** \strong Strong implementation of _write for UART & ITM string send to stream
** \param[in] file - unused
** \param[in] ptr - pointer to string to send to stream
** \param[in] len - length of string to send to stream
** \return length written to stream (-1 otherwise)
** \retval -1 is returned in case of failure (UART)
**/
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

/*!\brief syscalls implementation of __io_getchar
** \strong Strong implementation of __io_getchar for UART char received from stream
** \return char received from stream (-1 otherwise)
** \retval -1 is returned in case of failure (UART)
**/
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
	//if (HAL_UART_Receive(dbg_uart, (uint8_t *) &ch, 1, 1))	{ return -1; }
	#endif

	return ch;
}

///*!\brief syscalls implementation of _read
//** \strong Strong implementation of _read for UART string received from stream
//** \param[in] file - unused
//** \param[in] ptr - pointer to string to receive from stream
//** \param[in] len - length of string to receive from stream
//** \return length received from stream (-1 otherwise)
//** \retval -1 is returned in case of failure (UART)
//**/
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
void NONNULL__ ITM_port_send(char * str, const size_t len, const int port)
{
	for (char * pStr = str ; pStr < &str[len] ; pStr++)
	{
		while (ITM->PORT[port].u32 == 0);	// Wait for port to be ready
		ITM->PORT[port].u8 = *pStr;
	}
}


#if !STDREAM_RDIR_SND_SYSCALLS
/***********************/
/*** ITM REDIRECTION ***/
/***********************/
/*!\brief Sends string to ITM0 port
** \param[in] str - pointer to string to send
** \param[in] len - length of string
**/
static void NONNULL__ ITM_send(char * str, const size_t len)
{
	for (char * pStr = str ; pStr < &str[len] ; pStr++)
	{
		ITM_SendChar(*pStr);
	}
}

int NONNULL__ printf_ITM(char * str, ...)
{
	va_list args;

	va_start(args, str);
	vsprintf(dbg_msg_out, str, args);
	va_end(args);
	ITM_send(dbg_msg_out, strlen(dbg_msg_out));
	str_clr(dbg_msg_out);	// Empty string

	return 0;
}

int NONNULL__ vprintf_ITM(char * str, va_list args)
{
	vsprintf(dbg_msg_out, str, args);
	ITM_send(dbg_msg_out, strlen(dbg_msg_out));
	str_clr(dbg_msg_out);	// Empty string

	return 0;
}
#endif
#endif


#if !STDREAM_RDIR_SND_SYSCALLS
/***************************/
/*** GENERAL REDIRECTION ***/
/***************************/
int NONNULL__ printf_redir(char * str, ...)
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


int NONNULL__ vprintf_redir(char * str, va_list args)
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
