/*!\file random_utils.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief (A little less pseudo) random numbers generation utilities
**/
/****************************************************************/
#ifndef __RANDOM_UTILS_H
	#define __RANDOM_UTILS_H

#include "sarmfsw.h"
/****************************************************************/


/*!\brief Generate a random number based on STM32 unique ID
** \param[in] start - Value for a first seed (if a value has been stored for next reset)
**
** \return Generated random number
**/
uint32_t random_Get(uint32_t start);


/****************************************************************/
#endif	/* __RANDOM_UTILS_H */
/****************************************************************/
