/*!\file pattern2d.c
** \author SMFSW
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief 2 dimensional patterns utilities
**/
/****************************************************************/
#include "sarmfsw.h"
#include "pattern2d.h"
/****************************************************************/


uint16_t pattern_linearize(const uint16_t tab[][2], const uint16_t nb, uint16_t val)
{
	uint16_t idxMin = 0, idxMax = nb - 1;

	// Find indexes
	while (idxMax > idxMin + 1)
	{
		uint16_t idx = ((idxMax - idxMin) / 2 + idxMin);

		if (val < tab[idx][0])	{ idxMax = idx; }
		else					{ idxMin = idx; }
	}

	// Get interval values
	uint16_t x0 = tab[idxMin][0];
	uint16_t y0 = tab[idxMin][1];
	uint16_t x1 = tab[idxMax][0];
	uint16_t y1 = tab[idxMax][1];

	// Linearize
	float coef = (float) (y1 - y0) / (float) (x1 - x0);
	return (uint16_t) (coef * (val -  x0) + y0);
}
