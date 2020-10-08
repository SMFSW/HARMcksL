/*!\file UART_term.c
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief UART terminal
** \note UART redirection is enabled when UART_REDIRECT symbol is defined at project level
** \note define DBG_SERIAL in compiler defines with an UART instance to send printf likes strings to UART
*/
/****************************************************************/
#include <stdio.h>
#include <string.h>

#include "sarmfsw.h"

#include "stdream_rdir.h"
#include "UART_term.h"

#if defined(HAL_UART_MODULE_ENABLED)
#if defined(UART_REDIRECT)
/****************************************************************/

#if !defined(DBG_SERIAL)
#warning "You have to define DBG_SERIAL in usart.h with an UART instance for this to work!"
#endif

char breakout_char = '!';							//!< breakout char (message complete)

UART_HandleTypeDef * const dbg_uart = DBG_SERIAL;	//!< Instance of UART debug terminal

//! \warning dbg_msg_in buffer for UART_term is limited to \b SZ_DBG_IN
static char dbg_msg_in[SZ_DBG_IN + 1] = "";			//!< UART_term buffer for input

static uint8_t uart_in_nb = 0;						//!< Number of chars in input buffer of UART debug terminal


FctERR NONNULL__ UART_Term_Launch_It_Rx(UART_HandleTypeDef * const huart)
{
#if !STDREAM_RDIR_RCV_SYSCALLS
	if (huart != dbg_uart)	{ return ERROR_INSTANCE; }

	HAL_UART_Receive_IT(huart, (uint8_t *) &dbg_msg_in[uart_in_nb], 1);
#endif

	return ERROR_OK;
}


FctERR NONNULL__ UART_Term_Flush_RxBuf(UART_HandleTypeDef * const huart)
{
	if (huart != dbg_uart)	{ return ERROR_INSTANCE; }

	str_clr(dbg_msg_in);	// Clear input buffer
	uart_in_nb = 0;			// Empty char number

	return ERROR_OK;
}


FctERR NONNULL__ UART_Term_Char_Handler(UART_HandleTypeDef * const huart, const char ch)
{
	#if STDREAM_RDIR_RCV_SYSCALLS
	dbg_msg_in[uart_in_nb] = ch;
	#else
	UNUSED(ch);
	#endif

	if (	(dbg_msg_in[uart_in_nb] == '\r')			// Carriage return as default breakout char
		||	(dbg_msg_in[uart_in_nb] == breakout_char)	// User defined breakout char
		||	(uart_in_nb >= sizeof(dbg_msg_in) - 1))		// Full buffer
	{
		dbg_msg_in[uart_in_nb] = charNull;
		UART_Term_Message_Handler(dbg_msg_in, uart_in_nb);
		UART_Term_Flush_RxBuf(huart);
	}
	else	{ uart_in_nb++; }							// Incrementing only when char received & no full message

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ UART_Term_Message_Handler(const char * msg, const uint8_t len)
{
	if (len)	{ printf("%s\r\n", msg); }	// Parrot
	return ERROR_OK;
}


/*****************/
/*** CALLBACKS ***/
/*****************/
#if !STDREAM_RDIR_SND_SYSCALLS
extern char dbg_msg_out[SZ_DBG_OUT];

void UART_Term_TxCpltCallback(UART_HandleTypeDef * const huart)
{
	UNUSED(huart);	// Prevent compiler warnings

	str_clr(dbg_msg_out);	// Clear output buffer
}

#if STDREAM__UART_TX_IT
/*!\brief Tx Transfer completed callback
** \weak Weak implementation of HAL_UART_TxCpltCallback in the library (will get precedence over HAL function)
** \warning You should probably implement your own callback, especially with multiple UART busses
** \param[in,out] huart - UART handle
**/
__WEAK void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)
{
	if (huart == dbg_uart)	{ UART_Term_TxCpltCallback(huart); }
}
#endif
#endif


#if !STDREAM_RDIR_RCV_SYSCALLS
void UART_Term_RxCpltCallback(UART_HandleTypeDef * const huart)
{
	UART_Term_Char_Handler(huart, 0);
	UART_Term_Launch_It_Rx(huart);					// Waiting for next char to receive
}

/*!\brief Rx Transfer completed callback
** \weak Weak implementation of HAL_UART_RxCpltCallback in the library (will get precedence over HAL function)
** \warning You should probably implement your own callback, especially with multiple UART busses
** \param[in,out] huart - UART handle
**/
__WEAK void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
	if (huart == dbg_uart)	{ UART_Term_RxCpltCallback(huart); }
}
#endif


/****************************************************************/
#endif	/* defined(UART_REDIRECT) */
#endif	/* defined(HAL_UART_MODULE_ENABLED) */
/****************************************************************/
