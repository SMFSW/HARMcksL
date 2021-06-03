/*!\file Logic_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief Extensions for logic variable handling
**/
/****************************************************************/
#include <string.h>

#include "Logic_ex.h"
/****************************************************************/


void NONNULLX__(1, 2) Logic_in_init(Logic_in * const in,
									uint32_t (*getter)(Logic_in * const), const uint32_t pos, const bool logic, const uint16_t filter,
									void (*onSet)(Logic_in * const), void (*onReset)(Logic_in * const), const bool repeat)
{
	/* Check the parameters */
	assert_param(pos < 32);

	memset(in, 0, OFFSET_OF(Logic_in, cfg));
	in->cfg.get = getter;
	in->cfg.pos = pos;
	in->cfg.logic = logic;
	in->cfg.filt = filter;
	in->cfg.onSet = onSet;
	in->cfg.onReset = onReset;
	in->cfg.repeat = repeat;
}


void NONNULL__ Logic_in_handler(Logic_in * const in)
{
	const bool val = in->cfg.get ? RSHIFT(in->cfg.get(in), in->cfg.pos) & 1U : false;
	if (val == in->cfg.logic)
	{
		if (TPSSUP_MS(in->hIn, in->cfg.filt))	{ in->in = true; }
	}
	else
	{
		in->in = false;
		in->hIn = HALTicks();
	}

	if (in->in == in->mem)
	{
		in->edge = NoEdge;
		if ((in->in) && (in->cfg.repeat))
		{
			if (in->cfg.onSet)	{ in->cfg.onSet(in); }
		}
	}
	else if (in->in > in->mem)
	{
		in->edge = Rising;
		if (in->cfg.onSet)		{ in->cfg.onSet(in); }
	}
	else
	{
		in->edge = Falling;
		if (in->cfg.onReset)	{ in->cfg.onReset(in); }
	}

	in->mem = in->in;
}

