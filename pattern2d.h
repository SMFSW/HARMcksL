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


#define	PATTERN_TAB(name, nbElem)		typedef struct pattern##name {		\
											const uint16_t	nb;				\
											const uint16_t	tab[nbElem][2];	\
										} pattern##name;	//!< Pattern tab typedef declaration with \b name catenation and \b nbElem max tab elements

#define PATTERN_LINEARIZE(name, val)	pattern_linearize(name.tab, name.nb, val)	//!< Macro to call linearization on a PATTERN_TAB typedef


uint16_t pattern_linearize(const uint16_t tab[][2], const uint16_t nbItems, uint16_t val);


/****************************************************************/
#endif	/* __PATTERNS2D_H */
/****************************************************************/
