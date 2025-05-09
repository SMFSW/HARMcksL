/*!\file flash_check.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief Function to check flash CRC
*/
/****************************************************************/
#ifndef FLASH_CHECK_H__
	#define FLASH_CHECK_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


/*!\brief Flash CRC check
** \note Assuming flash crc to check against is located in the last 4bytes of flash
**
** \retval ERROR_OK - Flash CRC consistent
** \retval ERROR_CRC - Flash CRC inconsistent
**/
FctERR flash_crc_check(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif /* FLASH_CHECK_H__ */
/****************************************************************/
