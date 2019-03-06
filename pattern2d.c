/*!\file pattern2d.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief 2 dimensional patterns utilities
**/
/****************************************************************/
#include "sarmfsw.h"
#include "pattern2d.h"
/****************************************************************/


uint16_t NONNULL__ pattern_evaluate(const uint16_t array[][2], const uint16_t nb, const uint16_t val)
{
	uint16_t idxMin = 0, idxMax = nb - 1;

	// Find indexes
	while (idxMax > idxMin + 1)
	{
		const uint16_t idx = ((idxMax - idxMin) / 2 + idxMin);

		if (val < array[idx][0])	{ idxMax = idx; }
		else						{ idxMin = idx; }
	}

	// Get interval values
	const uint16_t x0 = array[idxMin][0];
	const uint16_t y0 = array[idxMin][1];
	const uint16_t x1 = array[idxMax][0];
	const uint16_t y1 = array[idxMax][1];

	// Linearize
	const float coef = (float) (y1 - y0) / (float) (x1 - x0);
	return (uint16_t) (coef * (val -  x0) + y0);
}
