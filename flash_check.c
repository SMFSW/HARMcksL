/*!\file flash_check.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Function to check flash CRC
*/

#include "sarmfsw.h"


FctERR flash_crc_check(const uint32_t flash_size)
{
	const uint32_t	crc_ref = *(uint32_t *) (FLASH_BASE + flash_size - sizeof(uint32_t));
	uint32_t 		crc, len;
	uint32_t *		dat;

	// Enable CRC
	__HAL_RCC_CRC_CLK_ENABLE();

	CRC->CR |= 1;

	dat = (uint32_t *) FLASH_BASE;
	len = (flash_size - sizeof(uint32_t)) / sizeof(uint32_t);

	while (len--)
	{
		CRC->DR = *dat++;
	}

	crc = CRC->DR;

	// Disable CRC
	__HAL_RCC_CRC_CLK_DISABLE();

	return (crc_ref == crc) ? ERROR_OK : ERROR_CRC;
}
