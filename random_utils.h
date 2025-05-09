/*!\file random_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief (A little less pseudo) random numbers generation utilities
**/
/****************************************************************/
#ifndef RANDOM_UTILS_H__
	#define RANDOM_UTILS_H__

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

#endif /* RANDOM_UTILS_H__ */
/****************************************************************/
