/*!\file FctERR.h
** \author SMFSW
** \version v0.5
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief errors to SMFSW FctERR declarations
**/
/****************************************************************/
#ifndef __FCTERR_H
	#define __FCTERR_H

#include "sarmfsw.h"
#include CMSIS_INC
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum EnumFctERR
** \brief Enum of high level functions return state
**/
typedef enum PACK__ EnumFctERR{
	ERR_OK			= 0U,	//!< OK
	ERR_SPEED		= 1U,	//!< This device does not work in the active speed mode.
	ERR_RANGE		= 2U,	//!< Parameter out of range.
	ERR_VALUE		= 3U,	//!< Parameter of incorrect value.
	ERR_OVERFLOW	= 4U,	//!< Overflow.
	ERR_MATH		= 5U,	//!< Overflow during evaluation.
	ERR_ENABLED		= 6U,	//!< Device is enabled.
	ERR_DISABLED	= 7U,	//!< Device is disabled.
	ERR_BUSY		= 8U,	//!< Device is busy.
	ERR_NOTAVAIL	= 9U,	//!< Requested value or method not available.
	ERR_RXEMPTY		= 10U,	//!< No data in receiver.
	ERR_TXFULL		= 11U,	//!< Transmitter is full.
	ERR_BUSOFF		= 12U,	//!< Bus not available.
	ERR_OVERRUN		= 13U,	//!< Overrun error is detected.
	ERR_FRAMING		= 14U,	//!< Framing error is detected.
	ERR_PARITY		= 15U,	//!< Parity error is detected.
	ERR_NOISE		= 16U,	//!< Noise error is detected.
	ERR_IDLE		= 17U,	//!< Idle error is detected.
	ERR_FAULT		= 18U,	//!< Fault error is detected.
	ERR_BREAK		= 19U,	//!< Break char is received during communication.
	ERR_CRC			= 20U,	//!< CRC error is detected.
	ERR_ARBITR		= 21U,	//!< A node lost arbitration. This error occurs if two nodes start transmission at the same time.
	ERR_PROTECT		= 22U,	//!< Protection error is detected.
	ERR_UNDERFLOW	= 23U,	//!< Underflow error is detected.
	ERR_UNDERRUN	= 24U,	//!< Underrun error is detected.
	ERR_COMMON		= 25U,	//!< Common error of a device.
	ERR_LINSYNC		= 26U,	//!< LIN synchronization error is detected.
	ERR_FAILED		= 27U,	//!< Requested functionality or process failed.
	ERR_QFULL		= 28U,	//!< Queue is full.
	ERR_CMD			= 29U,	//!< Command error is detected
	ERR_TIMEOUT		= 30U,	//!< Abort on timeout error
	ERR_NOTIMPLEM	= 31U,	//!< Function not implemented error
	ERR_MEMORY		= 32U,	//!< Memory error
	ERR_INSTANCE	= 33U	//!< Instance error
} FctERR;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Convert HAL_StatusTypeDef to FctERR
** \param[in] st - HAL_StatusTypeDef status
** \return FctERR status
**/
FctERR HALERRtoFCTERR(HAL_StatusTypeDef st);


/****************************************************************/
#endif /* __FCTERR_H */
/****************************************************************/
