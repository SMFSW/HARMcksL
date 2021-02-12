/*!\file flash_check.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief Function to check flash CRC
*/
/****************************************************************/
#include "sarmfsw.h"
#include "CRC_stm32.h"
#include "flash_check.h"
/****************************************************************/


FctERR flash_crc_check(void)
{
	const uint32_t crc_ref = *(uint32_t *) (FLASH_BASE + FLASH_SIZE - sizeof(uint32_t));
	const uint32_t len = FLASH_SIZE - sizeof(uint32_t);

	return crc_check(crc_ref, CRC_Feed_DWORD, FLASH_BASE, len);
}
