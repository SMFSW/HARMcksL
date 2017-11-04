/*!\file UART_term.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief UART terminal
*/
/****************************************************************/
#include <stdio.h>
#include <string.h>

#include "sarmfsw.h"

#include "stdream_rdir.h"
#include "UART_term.h"

#if defined(HAL_UART_MODULE_ENABLED)
/****************************************************************/

#if !defined(DBG_SERIAL)
#warning "You have to define DBG_SERIAL in usart.h with an UART instance for this to work!"
#else

char breakout_char = '!';					//!< breakout char (message complete)

static uint8_t uart_in_nb = 0;				//!< Number of chars in input buffer of UART debug terminal

UART_HandleTypeDef * dbg_uart = DBG_SERIAL;	//!< Instance of UART debug terminal


FctERR SERIAL_DBG_Launch_It_Rx(UART_HandleTypeDef * huart)
{
	if (huart != dbg_uart)	{ return ERROR_INSTANCE; }

	HAL_UART_Receive_IT(huart, (uint8_t *) &dbg_msg_in[uart_in_nb], 1);

	return ERROR_OK;
}


FctERR SERIAL_DBG_Flush_RxBuf(UART_HandleTypeDef * huart)
{
	if (huart != dbg_uart)	{ return ERROR_INSTANCE; }

	diInterrupts();
	str_clr(dbg_msg_in);	// Clear input buffer
	uart_in_nb = 0;			// Empty char number
	enInterrupts();

	return ERROR_OK;
}


/*****************/
/*** CALLBACKS ***/
/*****************/
__WEAK FctERR SERIAL_DBG_Message_Handler(const char * msg, uint8_t len)
{
	UNUSED(msg);
	UNUSED(len);

	if (len)	{ printf("%s\r\n", msg); }	// Parrot
	return ERROR_OK;
}


/*!\brief Rx Transfer completed callback.
** \param huart UART handle.
** \retval None
**/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
	if (huart == dbg_uart)
	{
		if (	(dbg_msg_in[uart_in_nb] == '\r')			// Carriage return as default breakout char
			||	(dbg_msg_in[uart_in_nb] == breakout_char)	// User defined breakout char
			||	(uart_in_nb >= sizeof(dbg_msg_in) - 1))		// Full buffer
		{
			dbg_msg_in[uart_in_nb] = charNUL;
			SERIAL_DBG_Message_Handler(dbg_msg_in, uart_in_nb);
			SERIAL_DBG_Flush_RxBuf(dbg_uart);
		}
		else
		{ uart_in_nb++; }	// Incrementing only when char received & no full message

		SERIAL_DBG_Launch_It_Rx(dbg_uart);	// Waiting for next char to receive
	}
}

#if defined(STDREAM__UART_TX_IT)
/*!\brief Tx Transfer completed callback (clear uart_out buffer)
** \param huart - UART handle
** \retval None
**/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)
{
	if (huart == dbg_uart)	{ str_clr(dbg_msg_out); }	// Clear output buffer
}
#endif

/****************************************************************/
#endif	/* defined(DBG_SERIAL) */
#endif	/* defined(HAL_UART_MODULE_ENABLED) */
/****************************************************************/
