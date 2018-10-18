/*!\file stdream_rdir.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Stream redirection
** \note define ITM_REDIRECT in compiler defines for stings to be printed to ITM0 port
** \note define UART_REDIRECT and DBG_SERIAL in compiler defines with an UART instance to send printf likes strings to UART
*/
/****************************************************************/
#ifndef __STDREAM_RDIR_H
	#define __STDREAM_RDIR_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdarg.h>

#include "sarmfsw.h"
/****************************************************************/
// TODO: add puts redirect ??


// *****************************************************************************
// Section: Constants
// *****************************************************************************

#define	printf		printf_redir	//!< Shadowing printf
#define	vprintf		vprintf_redir	//!< Shadowing vprintf


#define	SZ_DBG_OUT	128				//!< DEBUG send buffer size
#define	SZ_DBG_IN	32				//!< DEBUG receive buffer size

//! \warning dbg_msg_out buffer for stdream is limited to \b SZ_DBG_OUT
extern char dbg_msg_out[SZ_DBG_OUT];	//!< stdream buffer for output
#if defined(UART_REDIRECT)
//! \warning dbg_msg_in buffer for stdream is limited to \b SZ_DBG_IN
//! \note dbg_msg_in is only related to UART_term
extern char dbg_msg_in[SZ_DBG_IN + 1];	//!< stdream buffer for input
#endif


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Send string to ITM port
** \param[in] port - ITM port number
** \param[in] str - pointer to message to send
** \param[in] len - length of message to send
**/
void NONNULL__ ITM_port_send(const int port, const char * str, const int len);

// printf_ITM & vprintf_ITM will be redirected to ITM port 0 (ITM_SendChar used)
/*!\brief printf like redirected to ITM port 0
** \param[in] str - pointer to string to send
** \param[in] ... - Variadic string arguments
** \return Function status
** \retval 0 - OK
**/
int NONNULL__ printf_ITM(const char * str, ...);

/*!\brief printf like redirected to ITM port 0
** \param[in] str - pointer to string to send
** \param[in] args - Variadic string arguments
** \return Function status
** \retval 0 - OK
**/
int NONNULL__ vprintf_ITM(const char * str, va_list args);

// General printf & vprintf redirection, will flood all enabled ports (at the cost of speed)
/*!\brief printf like redirected to DBG_SERIAL UART and/or ITM port 0
** \param[in] str - pointer to string to send
** \param[in] ... - Variadic string arguments
** \return Function status
** \retval -1 - Problem occured
** \retval 0 - OK
**/
int	NONNULL__ printf_redir(const char * str, ...);

/*!\brief printf like redirected to DBG_SERIAL UART and/or ITM port 0
** \param[in] str - pointer to string to send
** \param[in] args - Variadic string arguments
** \return Function status
** \retval -1 - Problem occured
** \retval 0 - OK
**/
int	NONNULL__ vprintf_redir(const char * str, va_list args);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __STDREAM_RDIR_H */
/****************************************************************/
