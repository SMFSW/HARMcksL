/*!\file stdream_rdir.h
** \author SMFSW
** \version v0.7
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Stream redirection header
** \note define DBG_SERIAL in compiler defines with an UART instance to send printf likes strings to UART
** 		 otherwise, stings will be printed to ITM0 port only
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

#define	printf		printf_rdir		//!< Shadowing printf
#define	vprintf		vprintf_rdir	//!< Shadowing vprintf


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get floating point number decimal part
** \param[in] port - ITM port number
** \param[in] str - pointer to message to send
** \param[in] len - length of message to send
** \return Nothing
**/
void print_itm_port(int port, const char * str, int len);

// printf_ITM & vprintf_ITM will be redirected to ITM port 0 (ITM_SendChar used)
int printf_ITM(char * str, ...);
int vprintf_ITM(char * str, va_list args);

// General printf & vprintf redirection, will flood all enabled ports (at the cost of speed)
int	printf_rdir(char * str, ...);
int	vprintf_rdir(char * str, va_list args);


/*!\brief Get floating point number decimal part
** \note in need to print floats, add '-u _printf_float' in Linker options
** \warning enabling floating point support from linker seems to fubar printing long variables
** \param[in] f - floating point value
** \param[in] nb - Number of decimal to get after floating point
** \return nb decimal part as integer
**/
int32_t get_fp_dec(float f, uint8_t nb);


/****************************************************************/
#endif
/****************************************************************/
