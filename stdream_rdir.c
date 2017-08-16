/*!\file stdream_rdir.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Stream redirection
*/
/****************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "stdream_rdir.h"

#include "arm_stdclib.h"
/****************************************************************/


//! \warning Buffer for stdream is limited to \b 128B
static char buf_stream[128] = "";	//!< stdream buffer for output


/****************************************************************/
#if defined(HAL_UART_MODULE_ENABLED)
#include "usart.h"

#if !defined(DBG_SERIAL)
#warning "You have to define DBG_SERIAL in usart.h with an UART instance for this to work!"
#endif

//#define STDREAM__UART_IT		//!< To be defined to send to uart using interrupts

/*!\brief Sends string to UART
** \note define STDREAM__UART_IT in compiler defines to send strings using interruptions
** \param[in] str - pointer to string to send
** \param[in] len - length of string
** \return HAL Status
**/
__STATIC_INLINE HAL_StatusTypeDef INLINE__ print_uart(char * str, int len)
{
	// TODO: find a way to determine if UART interrupts are enabled or not
	#if defined(STDREAM__UART_IT)
		return HAL_UART_Transmit_IT(DBG_SERIAL, (uint8_t *) str, len);
	#else
		return HAL_UART_Transmit(DBG_SERIAL, (uint8_t *) str, len, 30);
	#endif
}

#endif
/****************************************************************/


/*!\brief Sends string to ITM0 port
** \param[in] str - pointer to string to send
** \param[in] len - length of string
** \return Nothing
**/
static void print_itm(const char * str, int len)
{
	//while (*str != '\0')
	for (int i = 0 ; i < len ; i++)
		ITM_SendChar(*str++);
}


/*!\brief Sends string to chosen ITM port
** \param[in] port - ITM port number
** \param[in] str - pointer to string to send
** \param[in] len - length of string
** \return Nothing
**/
void print_itm_port(int port, const char * str, int len)
{
	for (int i = 0 ; i < len ; i++)
	{
		while (ITM->PORT[port].u32 == 0);	// Wait for port to be ready
		ITM->PORT[port].u8 = *str++;
	}
}


/********************/
/*** PRINTF LIKES ***/
/********************/

/*** ITM ***/
int printf_ITM(char * str, ...)
{
	va_list args;

	va_start(args, str);
	vsprintf(buf_stream, str, args);
	va_end(args);
	print_itm(buf_stream, strlen(buf_stream));
	str_clr(buf_stream);	// Empty string
	return 0;
}

int vprintf_ITM(char * str, va_list args)
{
	vsprintf(buf_stream, str, args);
	print_itm(buf_stream, strlen(buf_stream));
	str_clr(buf_stream);	// Empty string
	return 0;
}


/*** GENERAL REDIRECTION ***/
int printf_rdir(char * str, ...)
{
	uint16_t len;
	va_list args;

	va_start(args, str);
	vsprintf(buf_stream, str, args);
	va_end(args);
	len = strlen(buf_stream);

#if defined(ITM_ENABLED)
	print_itm(buf_stream, len);
#endif

#if defined(DBG_SERIAL)
	print_uart(buf_stream, len);
#endif

#if (!defined(ITM_ENABLED) && !defined(DBG_SERIAL))
	UNUSED(len);
#endif

	str_clr(buf_stream);	// Empty string
	return 0;
}


int vprintf_rdir(char * str, va_list args)
{
	uint16_t len;

	vsprintf(buf_stream, str, args);
	len = strlen(buf_stream);

#if defined(ITM_ENABLED)
	print_itm(buf_stream, len);
#endif

#if defined(DBG_SERIAL)
	print_uart(buf_stream, len);
#endif

#if (!defined(ITM_ENABLED) && !defined(DBG_SERIAL))
	UNUSED(len);
#endif

	str_clr(buf_stream);	// Empty string
	return 0;
}


int32_t get_fp_dec(float f, uint8_t nb)
{
	int32_t mul = 1;
	f -= (int32_t) f;
	while(nb-- != 0)	{ mul *= 10; }
	return (int32_t) (f * mul);
}


