/*!\file stdream_rdir.h
** \author SMFSW
** \version v0.5
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Stream redirection header
*/
/****************************************************************/
#ifndef __STREAM_REDIRECT_H
	#define __STREAM_REDIRECT_H

#include <stdarg.h>

#include "sarmfsw.h"
#include CMSIS_INC
#include CMSIS_CFG
/****************************************************************/
// TODO: add puts redirect ??


// *****************************************************************************
// Section: Constants
// *****************************************************************************
// #define ITM_ENABLED				//!< Enable ITM send if defined

#define	printf		printf_rdir		//!< Shadowing printf use
#define	vprintf		vprintf_rdir	//!< Shadowing vprintf use


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
void print_itm_port(int port, const char * msg, int len);

// printf_ITM & vprintf_ITM will be redirected to ITM port 0 (ITM_SendChar used)
int printf_ITM(char * string, ...);
int vprintf_ITM(char * string, va_list args);

// General printf & vprintf redirection, will flood all enabled ports (at the cost of speed)
int	printf_rdir(char * string, ...);
int	vprintf_rdir(char * string, va_list args);


/****************************************************************/
#endif
/****************************************************************/
