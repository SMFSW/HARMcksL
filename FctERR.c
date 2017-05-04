/*!\file FctERR.c
** \author SMFSW
** \version v0.4
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief errors to SMFSW FctERR code
**/

#include "FctERR.h"


FctERR HALERRtoFCTERR(HAL_StatusTypeDef Status)
{
	if (Status == HAL_OK)				return ERR_OK;
	else if (Status == HAL_ERROR)		return ERR_FAULT;
	else if (Status == HAL_BUSY)		return ERR_BUSY;
	else if (Status == HAL_TIMEOUT)		return ERR_TIMEOUT;
	else 								return ERR_FAULT;
}
