/*!\file UART_term.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief UART terminal header
** \note UART redirection is enabled when UART_REDIRECT symbol is defined at project level
** \note define DBG_SERIAL in compiler defines with an UART instance to send printf likes strings to UART
*/
/****************************************************************/
#ifndef __UART_TERM_H
	#define __UART_TERM_H

#include "sarmfsw.h"

#if defined(HAL_UART_MODULE_ENABLED)
#if defined(UART_REDIRECT)
/****************************************************************/


#define STDREAM__UART_TX_IT			//!< To be defined to send to uart using interrupts


//! \note Default user breakout char set to '!' and '\\r' is built-in default breakout char
extern char breakout_char;						//!< breakout char (message complete)

extern UART_HandleTypeDef * dbg_uart;			//!< UART debug terminal instance


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get UART Rx breakout character
** \return Breakout character
**/
__INLINE char INLINE__ getBreakout_char(void) {
	return breakout_char; }

/*!\brief Set a new breakout character
** \param[in] breakout - new breakout character
**/
__INLINE void INLINE__ setBreakout_char(const char breakout) {
	breakout_char = breakout; }

/*!\brief Start UART SERIAL DEBUG Rx interruptions
** \param[in] huart - UART handle
** \return Error code
**/
FctERR SERIAL_DBG_Launch_It_Rx(UART_HandleTypeDef * huart);

/*!\brief Clear buffer in used for SERIAL DEBUG
** \param[in] huart - UART handle (reserved for future use if needed)
** \return Error code
**/
FctERR SERIAL_DBG_Flush_RxBuf(UART_HandleTypeDef * huart);

/*!\brief Treat fully received message
** \weak This function is implemented as weak to be implemented in projects (weak one only prints & flushes the buffer)
** \param[in] msg - pointer to received message
** \param[in] len - received message length
** \return Error code
**/
__WEAK FctERR SERIAL_DBG_Message_Handler(const char * msg, const uint8_t len);


/*!\brief Waiting for UART global state to be ready for next transmission
** \param[in] huart - UART handle
**/
__INLINE void INLINE__ SERIAL_DBG_Wait_Ready(UART_HandleTypeDef * huart)
{
	#if defined(STDREAM__UART_TX_IT)
		while (huart->gState != HAL_UART_STATE_READY)
		{
			#if defined(HAL_IWDG_MODULE_ENABLED)
				HAL_IWDG_Refresh(&hiwdg);
			#endif
		}
	#endif
}


/*!\brief Sends string to UART
** \param[in] huart - UART handle
** \param[in] str - pointer to string to send
** \param[in] len - length of string
** \return HAL Status
**/
__INLINE HAL_StatusTypeDef INLINE__ SERIAL_DBG_Send(UART_HandleTypeDef * huart, const char * str, const int len)
{
	// TODO: find a way to determine if UART Tx interrupts are enabled or not
	#if defined(STDREAM__UART_TX_IT)
		return HAL_UART_Transmit_IT(huart, (uint8_t *) str, len);
	#else
		return HAL_UART_Transmit(huart, (uint8_t *) str, len, 30);
	#endif
}


/****************************************************************/
#endif	/* defined(HAL_UART_MODULE_ENABLED) */
#endif	/* defined(UART_REDIRECT) */
#endif	/* __UART_TERM_H */
/****************************************************************/
