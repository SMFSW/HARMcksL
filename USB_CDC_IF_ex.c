/*!\file USB_CDC_IF_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Extensions for USB VCP
**
** \code
**	static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
**	{
**		switch(cmd)
**		{
**			...
**
**			case CDC_SET_LINE_CODING:
**				CDC_Control_Set_Line_Coding(pbuf);
**			break;
**
**			case CDC_GET_LINE_CODING:
**				CDC_Control_Get_Line_Coding(pbuf);
**			break;
**
**			case CDC_SET_CONTROL_LINE_STATE:
**				CDC_Control_Set_Line_State(pbuf);
**			break;
**
**			...
**		}
**	}
** \endcode
** \note Define \c USE_USB_CDC_IF_EX symbol at project level to use CDC_IF_ex functionalities
**/
/****************************************************************/
#include <string.h>

#include "sarmfsw.h"

#include "USB_CDC_IF_ex.h"

#if defined(HAL_PCD_MODULE_ENABLED)
#if defined(USE_USB_CDC_IF_EX)

#include "usbd_cdc_if.h"
/****************************************************************/

#define VCP_DEFAULT_SPEED	38400

/*********************************************************************************/
/** Line Coding Structure                                                       **/
/**-----------------------------------------------------------------------------**/
/** Offset | Field       | Size | Value  | Description                          **/
/** 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second**/
/** 4      | bCharFormat |   1  | Number | Stop bits                            **/
/**                                        0 - 1 Stop bit                       **/
/**                                        1 - 1.5 Stop bits                    **/
/**                                        2 - 2 Stop bits                      **/
/** 5      | bParityType |  1   | Number | Parity                               **/
/**                                        0 - None                             **/
/**                                        1 - Odd                              **/
/**                                        2 - Even                             **/
/**                                        3 - Mark                             **/
/**                                        4 - Space                            **/
/** 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          **/
/*********************************************************************************/
static USBD_CDC_LineCodingTypeDef vcp_lineCoding = { VCP_DEFAULT_SPEED, 0x00, 0x00, 0x08 };		// 38400bps, 1stop, no parity, 8bit

static bool vcp_open = false;	// Variable to be used as external to check if COM port is in use


__WEAK FctERR CDC_Control_Set_Line_State_Callback(const bool open)
{
	UNUSED(open);

	/**\code
	static bool connected = false;
	if (open != connected)	// To avoid multiple control line state disconnections
	{
		connected = open;
		// Custom code here
	}
	\endcode**/

	return ERROR_OK;
}

FctERR CDC_Control_Set_Line_State(const uint8_t * const pbuf) {
	vcp_open = ((USBD_SetupReqTypedef *) pbuf)->wValue & 1U;
	return CDC_Control_Set_Line_State_Callback(vcp_open); }


void CDC_Control_Set_Line_Coding(const uint8_t * const pbuf) {
	const size_t size = sizeof(USBD_CDC_LineCodingTypeDef);
	UNUSED_RET memcpy(&vcp_lineCoding, pbuf, size); }


void CDC_Control_Get_Line_Coding(uint8_t * const pbuf) {
	const size_t size = sizeof(USBD_CDC_LineCodingTypeDef);
	UNUSED_RET memcpy(pbuf, &vcp_lineCoding, size); }


bool get_CDC_Line_State(void) {
	return vcp_open; }


/****************************************************************/
#endif	/* defined(USE_USB_CDC_IF_EX) */
#endif	/* defined(HAL_PCD_MODULE_ENABLED) */
/****************************************************************/
