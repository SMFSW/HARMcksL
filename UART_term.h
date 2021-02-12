/*!\file UART_term.h
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief UART terminal header
** \note UART redirection is enabled when UART_REDIRECT symbol is defined at project level
** \note define DBG_SERIAL at project level with an UART instance to send printf likes strings to UART
** \note It is recommended to have interrupts enabled for UART instance (and is required when reception is needed)
*/
/****************************************************************/
#ifndef __UART_TERM_H
	#define __UART_TERM_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_UART_MODULE_ENABLED)
#if defined(UART_REDIRECT)
/****************************************************************/

#ifndef STDREAM__UART_TX_IT
#if STDREAM_RDIR_SND_SYSCALLS
#define STDREAM__UART_TX_IT		0			//!< Set to send to uart using interrupts
#else
//!\note STDREAM__UART_TX_IT can be defined at project level to define if UART messages are sent in blocking/non blocking mode
#define STDREAM__UART_TX_IT		1			//!< Set to send to uart using interrupts
#endif
#endif

#ifndef SZ_DBG_IN
//!\note SZ_DBG_IN can be defined at project level to define debug receive buffer size
#define	SZ_DBG_IN	32						//!< DEBUG receive buffer size
#endif

//! \note Default user breakout char set to '!' and '\\r' is built-in default breakout char
extern char breakout_char;					//!< breakout char (message complete)

extern UART_HandleTypeDef * const dbg_uart;	//!< UART debug terminal instance


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
FctERR NONNULL__ UART_Term_Launch_It_Rx(UART_HandleTypeDef * const huart);

/*!\brief Waiting for UART global state to be ready for next transmission
** \param[in] huart - UART handle
** \return Error code
**/
FctERR NONNULL__ UART_Term_Wait_Ready(UART_HandleTypeDef * const huart);

/*!\brief Clear buffer in used for SERIAL DEBUG
** \param[in] huart - UART handle
** \return Error code
**/
FctERR NONNULL__ UART_Term_Flush_RxBuf(UART_HandleTypeDef * const huart);

/*!\brief Sends string to UART
** \param[in] huart - UART handle
** \param[in] str - pointer to string to send
** \param[in] len - length of string
** \return Error code
**/
FctERR NONNULL__ UART_Term_Send(UART_HandleTypeDef * const huart, const char * str, const int len);


/*!\brief Treat fully received message
** \weak This function is implemented as weak to be implemented in projects (weak one only prints & flushes the buffer)
** \param[in] msg - pointer to received message
** \param[in] len - received message length
** \return Error code
**/
FctERR NONNULL__ UART_Term_Message_Handler(const char * msg, const uint8_t len);


/*!\brief UART Term Rx Transfer completed callback
** \note Can be used with callback registering CubeMX functionality
** \warning When not using callback registering CubeMX functionality, you may need to implement your own custom HAL_UART_RxCpltCallback
** 			weak HAL_UART_RxCpltCallback is defined in the library (handle for debug UART only), you may have to define your own when multiple UARTs are in use.
** \param[in,out] huart - UART handle
** \note Shall be called from HAL_UART_RxCpltCallback with proper UART instance
**/
void UART_Term_RxCpltCallback(UART_HandleTypeDef * const huart);

/*!\brief UART Term Tx Transfer completed callback
** \note Can be used with callback registering CubeMX functionality
** \warning When not using callback registering CubeMX functionality, you may need to implement your own custom HAL_UART_TxCpltCallback
** 			weak HAL_UART_TxCpltCallback is defined in the library (handle for debug UART only), you may have to define your own when multiple UARTs are in use.
** \param[in,out] huart - UART handle
** \note Shall be called from UART_Term_TxCpltCallback with proper UART instance
**/
void UART_Term_TxCpltCallback(UART_HandleTypeDef * const huart);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_UART_MODULE_ENABLED) */
#endif	/* defined(UART_REDIRECT) */
#endif	/* __UART_TERM_H */
/****************************************************************/
