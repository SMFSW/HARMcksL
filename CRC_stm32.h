/*!\file CRC_stm32.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief Function dealing with STM32 CRC peripheral
*/
/****************************************************************/
#ifndef __CRC_STM32_H
	#define __CRC_STM32_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


/*!\enum eCRCFeedSize
** \brief CRC feed size
**/
typedef enum eCRCFeedSize {
	CRC_Feed_BYTE = 1,		//!< CRC Feed is 8b size
	CRC_Feed_WORD = 2,		//!< CRC Feed is 16b size
	CRC_Feed_DWORD = 4,		//!< CRC Feed is 32b size
} eCRCFeedSize;


/*!\brief CRC computation
** \param[in,out] crc - Pointer to address to store CRC
** \param[in] feed_size - CRC peripheral feed size
** \param[in] start_addr - Start address of block to compute CRC
** \param[in] size - Size of block to compute CRC (in Bytes)
** \retval ERROR_OK - CRC computed
** \retval ERROR_VALUE - feed_size is not a value of enum eCRCFeedSize
** \retval ERROR_COMMON - size is not int division with feed_size
**/
FctERR NONNULL__ crc_compute(uint32_t * const crc, const eCRCFeedSize feed_size, const uint32_t start_addr, const uint32_t size);


/*!\brief CRC check
** \param[in] crc_ref - CRC to check against
** \param[in] feed_size - CRC peripheral feed size
** \param[in] start_addr - Start address of block to compute CRC
** \param[in] size - Size of block to compute CRC (in Bytes)
** \retval ERROR_OK - CRC consistent
** \retval ERROR_CRC - CRC inconsistent
** \retval ERROR_VALUE - feed_size is not a value of enum eCRCFeedSize
** \retval ERROR_COMMON - size is not int division with feed_size
**/
FctERR crc_check(const uint32_t crc_ref, const eCRCFeedSize feed_size, const uint32_t start_addr, const uint32_t size);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif /* __CRC_STM32_H */
/****************************************************************/
