/*!\file CRC_stm32.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Function dealing with STM32 CRC peripheral
*/
/****************************************************************/
#include <stdlib.h>

#include "sarmfsw.h"
#include "CRC_stm32.h"
/****************************************************************/


FctERR NONNULL__ crc_compute(uint32_t * crc, const eCRCFeedSize feed_size, const uint32_t start_addr, const uint32_t size)
{
	div_t			len;
	uint8_t *		dat8 = NULL;
	uint16_t *		dat16 = NULL;
	uint32_t *		dat32 = NULL;

	if (feed_size == CRC_Feed_BYTE)			{ dat8 = (uint8_t *) start_addr; }
	else if (feed_size == CRC_Feed_WORD)	{ dat16 = (uint16_t *) start_addr; }
	else if  (feed_size == CRC_Feed_DWORD)	{ dat32 = (uint32_t *) start_addr; }
	else									{ return ERROR_VALUE; }

	len = div(size, feed_size);
	if (len.rem)	{ return ERROR_COMMON; }

	// Enable CRC
	__HAL_RCC_CRC_CLK_ENABLE();

	CRC->CR |= CRC_CR_RESET;

	while (len.quot--)
	{
		if (feed_size == CRC_Feed_DWORD)		{ CRC->DR = *dat32++; }
		else if (feed_size == CRC_Feed_WORD)	{ CRC->DR = *dat16++; }
		else									{ CRC->DR = *dat8++; }
	}

	*crc = CRC->DR;

	// Disable CRC
	__HAL_RCC_CRC_CLK_DISABLE();

	return ERROR_OK;
}


FctERR crc_check(const uint32_t crc_ref, const eCRCFeedSize feed_size, const uint32_t start_addr, const uint32_t size)
{
	uint32_t	crc = 0;

	FctERR		err = crc_compute(&crc, feed_size, start_addr, size);
	if (err)	{ return err; }

	return (crc_ref == crc) ? ERROR_OK : ERROR_CRC;
}
