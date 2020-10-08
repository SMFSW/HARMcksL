/*!\file stdream_rdir.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
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

#ifndef STDREAM_RDIR_SND_SYSCALLS
//!\note STDREAM_RDIR_SND_SYSCALLS can be defined at project level to define redirection behavior (syscall way encouraged, as shadowing is deprecated)
//!\note Define USE_IO_PUTCHAR at project level also for __io_putchar implementation instead of full _write implementation (from syscalls)
#define STDREAM_RDIR_SND_SYSCALLS	1
#endif

#ifndef STDREAM_RDIR_RCV_SYSCALLS
//!\note STDREAM_RDIR_RCV_SYSCALLS can be defined at project level to define redirection behavior
#define STDREAM_RDIR_RCV_SYSCALLS	0
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************

#ifndef STDREAM_RDIR_SND_SYSCALLS
#define	printf		printf_redir	//!< Shadowing printf
#define	vprintf		vprintf_redir	//!< Shadowing vprintf
#endif


#if defined(ITM) || !STDREAM_RDIR_SND_SYSCALLS
#ifndef SZ_DBG_OUT
//!\note SZ_DBG_OUT can be defined at project level to define debug transmit buffer size (for ITM and/or if STDREAM_RDIR_SND_SYSCALLS is not used)
#define	SZ_DBG_OUT	128				//!< DEBUG send buffer size
#endif
#endif


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
#if defined(ITM)
/*!\brief Send string to ITM port
** \param[in] str - pointer to message to send
** \param[in] len - length of message to send
** \param[in] port - ITM port number
**/
void NONNULL__ ITM_port_send(const char * str, const int len, const int port);

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
#endif


#if !STDREAM_RDIR_SND_SYSCALLS
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

#endif

/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __STDREAM_RDIR_H */
/****************************************************************/
