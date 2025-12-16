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


/*!\brief Get Application start address
** \weak Function declared as weak, needs to be customly implemented in user code, otherwise returns \p FLASH_BASE value
** \return Application start address
**/
uint32_t Get_app_start_address(void);


/*!\brief Get Application size
** \weak Function declared as weak, needs to be customly implemented in user code, otherwise returns \p FLASH_SIZE value
** \return Application size
**/
uint32_t Get_app_size(void);


/*!\brief Application Flash CRC check
** \note Assuming flash crc to check against is located in the last 4bytes of application reserved flash
** \note \ref Get_app_start_address & \ref Get_app_size needs to be implemented in project, otherwise acts as \ref flash_crc_check
**
** \retval ERROR_OK - Flash CRC consistent
** \retval ERROR_CRC - Flash CRC inconsistent
**/
FctERR flash_crc_check(void);


/*!\brief Flash CRC check (complete FLASH area)
** \note Assuming flash crc to check against is located in the last 4bytes of flash
** \note Ideal when application takes the whole FLASH area, otherwise use \ref flash_app_crc_check instead
**
** \retval ERROR_OK - Flash CRC consistent
** \retval ERROR_CRC - Flash CRC inconsistent
**/
FctERR flash_full_crc_check(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif /* FLASH_CHECK_H__ */
/****************************************************************/
