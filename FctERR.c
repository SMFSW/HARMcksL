/*!\file FctERR.c
** \author SMFSW
** \version v0.6
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief errors to SMFSW FctERR code
**/
/****************************************************************/
/****************************************************************/

#include "FctERR.h"


FctERR HALERRtoFCTERR(HAL_StatusTypeDef st)
{
	if		(st == HAL_OK)			return ERR_OK;
	else if (st == HAL_ERROR)		return ERR_FAULT;
	else if (st == HAL_BUSY)		return ERR_BUSY;
	else if (st == HAL_TIMEOUT)		return ERR_TIMEOUT;
	else 							return ERR_FAULT;
}
