/*!\file pattern2d.h
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief 2 dimensional patterns header
**/
/****************************************************************/
#ifndef __PATTERNS2D_H
	#define __PATTERNS2D_H

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
uint16_t pattern_evaluate(const uint16_t array[][2], const uint16_t nb, uint16_t val);


/****************************************************************/
#endif	/* __PATTERNS2D_H */
/****************************************************************/
