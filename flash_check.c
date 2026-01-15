/*!\file flash_check.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Function to check flash CRC
*/
/****************************************************************/
#include "sarmfsw.h"
#include "CRC_stm32.h"
#include "flash_check.h"
/****************************************************************/


__WEAK uint32_t Get_app_start_address(void) {
	return FLASH_BASE; }

__WEAK uint32_t Get_app_size(void) {
	return FLASH_SIZE; }


FctERR flash_crc_check(void)
{
	const uint32_t base = Get_app_start_address();
	const size_t sz = Get_app_size();

	// Deepsource would raise possible misaligned pointer access, keeping it quiet with 'skipcq'
	const uint32_t crc_ref = *(uint32_t *) (base + sz - sizeof(uint32_t));	// skipcq: CXX-S1014
	const size_t len = sz - sizeof(uint32_t);

	return crc_check(crc_ref, CRC_Feed_DWORD, base, len);
}


FctERR flash_full_crc_check(void)
{
	// Deepsource would raise possible misaligned pointer access, keeping it quiet with 'skipcq'
	const uint32_t crc_ref = *(uint32_t *) (FLASH_BASE + FLASH_SIZE - sizeof(uint32_t));	// skipcq: CXX-S1014
	const size_t len = (size_t) FLASH_SIZE - sizeof(uint32_t);

	return crc_check(crc_ref, CRC_Feed_DWORD, FLASH_BASE, len);
}
