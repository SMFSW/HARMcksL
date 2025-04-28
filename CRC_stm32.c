/*!\file CRC_stm32.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief Function dealing with STM32 CRC peripheral
*/
/****************************************************************/
#include <stdlib.h>

#include "sarmfsw.h"
#include "CRC_stm32.h"
/****************************************************************/


FctERR NONNULL__ crc_compute(uint32_t * const crc, const eCRCFeedSize feed_size, const uint32_t start_addr, const size_t size)
{
	FctERR				err = ERROR_OK;
	div_t				len;
	const uint8_t *		dat8 = NULL;
	const uint16_t *	dat16 = NULL;
	const uint32_t *	dat32 = NULL;

	if (feed_size == CRC_Feed_DWORD)		{ dat32 = (uint32_t *) start_addr; }
	else if (feed_size == CRC_Feed_WORD)	{ dat16 = (uint16_t *) start_addr; }
	else if (feed_size == CRC_Feed_BYTE)	{ dat8 = (uint8_t *) start_addr; }
	else									{ err = ERROR_VALUE; }

	len = div(size, feed_size);
	if (len.rem)	{ err = ERROR_COMMON; }

	if (err == ERROR_OK)
	{
		__HAL_RCC_CRC_CLK_ENABLE();	// Enable CRC

		CRC->CR |= CRC_CR_RESET;

		while (len.quot--)
		{
			if (feed_size == CRC_Feed_DWORD)		{ CRC->DR = *dat32++; }
			else if (feed_size == CRC_Feed_WORD)	{ CRC->DR = *dat16++; }
			else									{ CRC->DR = *dat8++; }
		}

		*crc = CRC->DR;

		__HAL_RCC_CRC_CLK_DISABLE();	// Disable CRC
	}

	return err;
}


FctERR crc_check(const uint32_t crc_ref, const eCRCFeedSize feed_size, const uint32_t start_addr, const size_t size)
{
	uint32_t	crc = 0;
	FctERR		err = crc_compute(&crc, feed_size, start_addr, size);

	if (	(err == ERROR_OK)
		&&	(crc_ref != crc))
	{
		err = ERROR_CRC;
	}

	return err;
}
