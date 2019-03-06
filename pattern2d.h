/*!\file pattern2d.h
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief 2 dimensional patterns utilities
**/
/****************************************************************/
#ifndef __PATTERN2D_H
	#define __PATTERN2D_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


#define	PATTERN_TAB(name, nbElem)		typedef struct pattern##name {			\
											const uint16_t	nb;					\
											const uint16_t	array[nbElem][2];	\
										} pattern##name;	//!< Pattern tab typedef declaration with \b name catenation and \b nbElem max tab elements

#define PATTERN_EVALUATE(name, val)		pattern_evaluate(name.array, name.nb, val)	//!< Macro to call linearization on a PATTERN_TAB typedef


/*!\brief 2 dimensional pattern evaluation algorithm
**
** \param [in] array - pointer to 2 dimensional
** \param [in] nb - Number of items of the array
** \param [in] val - Value to evaluate
** \return Evaluated value in regard of val
**/
uint16_t NONNULL__ pattern_evaluate(const uint16_t array[][2], const uint16_t nb, const uint16_t val);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __PATTERN2D_H */
/****************************************************************/
