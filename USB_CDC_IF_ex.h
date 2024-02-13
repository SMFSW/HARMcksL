/*!\file USB_CDC_IF_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
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
** \note Define USE_USB_CDC_IF_EX symbol at project level to use CDC_IF_ex functionalities
**/
/****************************************************************/
#ifndef __USB_CDC_IF_EX_H
	#define __USB_CDC_IF_EX_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(USE_USB_CDC_IF_EX)
/****************************************************************/

/*!\brief Callback for CDC port connection/disconnection
** \weak CDC_Control_Set_Line_State_Callback may be user implemented to suit custom needs
** \note Several disconnection messages can appear on bus, use example code to get only state change
** \note \ref get_CDC_Line_State can be used instead (if not willing to use callback) to check port current status
** \param[in] open - port state
** \return FctERR - Error code
**/
FctERR CDC_Control_Set_Line_State_Callback(const bool open);

/*!\brief CDC Set Line State message handler
** \note Call CDC_Control_Set_Line_State in usbd_cdc_if.c \ref CDC_Control_FS function, CDC_SET_CONTROL_LINE_STATE case
** \note Several disconnection messages can appear on bus, use example code in callback to get only state change
** \param[in] pbuf - Pointer to message data
** \return FctERR - Error code
**/
FctERR CDC_Control_Set_Line_State(const uint8_t * const pbuf);

/*!\brief CDC Set Line Coding message handler
** \note Call CDC_Control_Set_Line_Coding in usbd_cdc_if.c \ref CDC_Control_FS function, CDC_SET_LINE_CODING case
** \param[in] pbuf - Pointer to message data
**/
void CDC_Control_Set_Line_Coding(const uint8_t * const pbuf);

/*!\brief CDC Get Line Coding message handler
** \note Call CDC_Control_Get_Line_Coding in usbd_cdc_if.c \ref CDC_Control_FS function, CDC_GET_LINE_CODING case
** \param[in,out] pbuf - Pointer to message data
**/
void CDC_Control_Get_Line_Coding(uint8_t * const pbuf);


/*!\brief CDC Get current port state
** \return Port state
**/
bool get_CDC_Line_State(void);

/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(USE_USB_CDC_IF_EX) */
#endif	/* __USB_CDC_IF_EX_H */
/****************************************************************/
