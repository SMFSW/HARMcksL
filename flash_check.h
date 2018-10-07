/*!\file flash_check.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Function to check flash CRC
*/
/****************************************************************/
#ifndef __FLASH_CHECK_H
	#define __FLASH_CHECK_H

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
FctERR flash_crc_check(const uint32_t flash_size);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif /* __FLASH_CHECK_H */
/****************************************************************/
