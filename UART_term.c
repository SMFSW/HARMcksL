/*!\file UART_term.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief UART terminal
** \note UART redirection is enabled when \c UART_REDIRECT symbol is defined at project level
** \note define \c DBG_SERIAL at project level with an UART instance to send printf likes strings to UART
** \note It is recommended to have interrupts enabled for UART instance (and is required when reception is needed)
*/
/****************************************************************/
#include <stdio.h>

#include "sarmfsw.h"

#include "stdream_rdir.h"
#include "UART_term.h"

#if defined(HAL_UART_MODULE_ENABLED)
#if defined(UART_REDIRECT)
/****************************************************************/

#if !defined(DBG_SERIAL)
#warning "You have to define DBG_SERIAL in usart.h with an UART instance for this to work!"
#endif

char breakout_char = '!';		//!< breakout char (message complete)

UART_HandleTypeDef * dbg_uart;	//!< Instance of UART debug terminal

static sUARTbuffer * pUARTrx;	//!< UART reception buffer


FctERR NONNULL__ UART_Term_Init(UART_HandleTypeDef * const huart, const size_t len)
{
	FctERR err = ERROR_MEMORY;

	assert_param(IS_UART_INSTANCE(huart->Instance));

	pUARTrx = (sUARTbuffer *) malloc(sizeof(*pUARTrx) + ((len + 1U) * sizeof(pUARTrx->data[0])));

	if (pUARTrx != NULL)
	{
		dbg_uart = huart;

		pUARTrx->max_len = len + 1U;
		UNUSED_RET UART_Term_Flush_RxBuf(huart);

		err = UART_Term_Launch_It_Rx(huart);
	}

	return err;
}


FctERR NONNULL__ UART_Term_Launch_It_Rx(UART_HandleTypeDef * const huart)
{
	if (huart != dbg_uart)	{ return ERROR_INSTANCE; }

	UNUSED_RET HAL_UART_Receive_IT(huart, (uint8_t *) &pUARTrx->data[pUARTrx->len], 1U);

	return ERROR_OK;
}


FctERR NONNULL__ UART_Term_Wait_Ready(UART_HandleTypeDef * const huart)
{
	#if STDREAM__UART_TX_IT
		if (huart->gState == HAL_UART_STATE_RESET)	{ return ERROR_NOTAVAIL; }

		while (huart->gState != HAL_UART_STATE_READY);
	#else
		UNUSED(huart);
	#endif
	return ERROR_OK;
}


FctERR NONNULL__ UART_Term_Flush_RxBuf(const UART_HandleTypeDef * const huart)
{
	if (huart != dbg_uart)	{ return ERROR_INSTANCE; }
	if (!pUARTrx)			{ return ERROR_MEMORY; }

	strclr(pUARTrx->data);	// Clear input buffer
	pUARTrx->len = 0U;		// Empty char number

	return ERROR_OK;
}


FctERR NONNULL__ UART_Term_Send(UART_HandleTypeDef * const huart, const char * str, const size_t len)
{
	#if STDREAM__UART_TX_IT
		#if STDREAM_RDIR_SND_SYSCALLS
		UART_Term_Wait_Ready(huart);
		#endif
		return HALERRtoFCTERR(HAL_UART_Transmit_IT(huart, (uint8_t *) str, len));
	#else
		return HALERRtoFCTERR(HAL_UART_Transmit(huart, (const uint8_t *) str, len, 30U));
	#endif
}


/*!\brief Received character handler on SERIAL DEBUG
** \param[in] huart - UART handle
** \return Error code
**/
static FctERR NONNULL__ UART_Term_Char_Handler(const UART_HandleTypeDef * const huart)
{
	if (!pUARTrx)	{ return ERROR_MEMORY; }

	if (	(pUARTrx->data[pUARTrx->len] == '\r')			// Carriage return as default breakout char
		||	(pUARTrx->data[pUARTrx->len] == breakout_char)	// User defined breakout char
		||	(pUARTrx->len >= pUARTrx->max_len))				// Full buffer
	{
		pUARTrx->data[pUARTrx->len] = charNull;
		#if !STDREAM_RDIR_RCV_SYSCALLS
		UNUSED_RET UART_Term_Message_Handler(pUARTrx->data, pUARTrx->len);
		UNUSED_RET UART_Term_Flush_RxBuf(huart);
		#endif
	}
	else if	(pUARTrx->data[pUARTrx->len] == '\b')			// Backspace char handling
	{
		if (pUARTrx->len != 0U)		{ pUARTrx->len--; }
	}
	else							{ pUARTrx->len++; }		// Incrementing only when char received & no full message

	return ERROR_OK;
}


__WEAK FctERR NONNULL__ UART_Term_Message_Handler(const char * msg, const size_t len)
{
	if (len != 0U)	{ UNUSED_RET printf("%s\r\n", msg); }	// Parrot
	return ERROR_OK;
}


/*****************/
/*** CALLBACKS ***/
/*****************/
#if !STDREAM_RDIR_SND_SYSCALLS
extern char dbg_msg_out[SZ_DBG_OUT];	//!< stdream buffer for output

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


void UART_Term_RxCpltCallback(UART_HandleTypeDef * const huart)
{
	UNUSED_RET UART_Term_Char_Handler(huart);
	UNUSED_RET UART_Term_Launch_It_Rx(huart);			// Waiting for next char to receive
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


/****************************************************************/
#endif	/* defined(UART_REDIRECT) */
#endif	/* defined(HAL_UART_MODULE_ENABLED) */
/****************************************************************/
