/*!\file random_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief (A little less pseudo) random numbers generation utilities
**/
/****************************************************************/
#ifndef __RANDOM_UTILS_H
	#define __RANDOM_UTILS_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


/*!\brief Generate a random number based on STM32 unique ID
** \param[in] start - Value for a first seed (if a value has been stored for next reset)
**
** \return Generated random number
**/
uint32_t random_Get(const uint32_t start);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __RANDOM_UTILS_H */
/****************************************************************/
