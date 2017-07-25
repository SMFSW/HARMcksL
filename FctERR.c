/*!\file FctERR.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief errors to SMFSW FctERR code
**/
/****************************************************************/
/****************************************************************/

#include "FctERR.h"


FctERR HALERRtoFCTERR(HAL_StatusTypeDef status)
{
	if		(status == HAL_OK)			return ERR_OK;
	else if (status == HAL_ERROR)		return ERR_FAULT;
	else if (status == HAL_BUSY)		return ERR_BUSY;
	else if (status == HAL_TIMEOUT)		return ERR_TIMEOUT;
	else 								return ERR_FAULT;
}
