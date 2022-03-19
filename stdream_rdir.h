/*!\file stdream_rdir.h
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
#ifndef __STDREAM_RDIR_H
	#define __STDREAM_RDIR_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdio.h>

#include "sarmfsw.h"
/****************************************************************/


#ifndef STDREAM_RDIR_SND_SYSCALLS
//!\note STDREAM_RDIR_SND_SYSCALLS can be defined at project level to define redirection behavior (syscalls implementation preferred, shadowing being deprecated)
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

#if !STDREAM_RDIR_SND_SYSCALLS
#include <stdarg.h>

#define	printf		printf_redir	//!< Shadowing printf
#define	vprintf		vprintf_redir	//!< Shadowing vprintf

#ifndef SZ_DBG_OUT
//!\note SZ_DBG_OUT can be defined at project level to define debug transmit buffer size (for ITM and/or if STDREAM_RDIR_SND_SYSCALLS is not used)
#define	SZ_DBG_OUT	128				//!< DEBUG send buffer size
#endif

#else

/*!\brief Change stdout buffer state to no buffering, allowing to send any char written to stream on the fly.
** \note If this inline is not called once, strings will be written to stream only when '\\n' (new line) is written to buffer.
** \note Please keep in mind that disabling buffering will call _write once for every character vs once for every buffered line when buffering is enabled.
**/
__INLINE void INLINE__ stdout_no_buffer(void) {
	setvbuf(stdout, NULL, _IONBF, 0); }

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
void NONNULL__ ITM_port_send(char * str, const size_t len, const int port);

#if !STDREAM_RDIR_SND_SYSCALLS
// printf_ITM & vprintf_ITM will be redirected to ITM port 0 (ITM_SendChar used)
/*!\brief printf like redirected to ITM port 0
** \param[in] str - pointer to string to send
** \param[in] ... - Variadic string arguments
** \return Function status
** \retval 0 - OK
**/
int NONNULL__ printf_ITM(char * str, ...);

/*!\brief printf like redirected to ITM port 0
** \param[in] str - pointer to string to send
** \param[in] args - Variadic string arguments
** \return Function status
** \retval 0 - OK
**/
int NONNULL__ vprintf_ITM(char * str, va_list args);
#endif
#endif


#if !STDREAM_RDIR_SND_SYSCALLS
/*!\brief printf like redirected to DBG_SERIAL UART and/or ITM port 0
** \param[in] str - pointer to string to send
** \param[in] ... - Variadic string arguments
** \return Function status
** \retval -1 - Problem occured
** \retval 0 - OK
**/
int	NONNULL__ printf_redir(char * str, ...);

/*!\brief printf like redirected to DBG_SERIAL UART and/or ITM port 0
** \param[in] str - pointer to string to send
** \param[in] args - Variadic string arguments
** \return Function status
** \retval -1 - Problem occured
** \retval 0 - OK
**/
int	NONNULL__ vprintf_redir(char * str, va_list args);
#endif


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __STDREAM_RDIR_H */
/****************************************************************/
