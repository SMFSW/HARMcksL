/*!\file flash_check.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Function to check flash CRC
*/
/****************************************************************/
#ifndef __FLASHCRC_H__
	#define __FLASHCRC_H__
/****************************************************************/

#include "sarmfsw.h"


/*!\brief Flash CRC check
** \note Assuming flash crc to check against is located in the last 4bytes of flash
**
** \retval ERROR_OK - Flash CRC consistent
** \retval ERROR_CRC - Flash CRC inconsistent
**/
FctERR flash_crc_check(const uint32_t flash_size);


/****************************************************************/
#endif
/****************************************************************/
