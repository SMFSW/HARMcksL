/*!\file Logic_in.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief Logic input handling
**/
/****************************************************************/
#include "Logic_in.h"
/****************************************************************/


void NONNULLX__(1) Logic_in_init(	Logic_in * const in, GPIO_PinState (*getter)(const Logic_in * const),
									uint32_t * const addr, const uint32_t pos, const GPIO_PinState polarity, const uint16_t filter,
									void (*onSet)(const Logic_in * const), void (*onReset)(const Logic_in * const), const bool repeat)
{
	/* Check the parameters */
	//assert_param(pos < 32);	// Would raise an assert error in case of GPIO pin

	in->cfg.get = getter;
	in->cfg.LOGx = addr;
	in->cfg.LOG_Pos = pos;
	in->cfg.polarity = polarity;
	in->cfg.filt = filter;
	in->cfg.onSet = onSet;
	in->cfg.onReset = onReset;
	in->cfg.repeat = repeat;

	in->in = polarity;
	in->mem = polarity;
	in->edge = NoEdge;
	in->hIn = 0;
}


void NONNULL__ Logic_in_handler(Logic_in * const in)
{
	GPIO_PinState val = in->cfg.polarity;

	if (in->cfg.get)		{ val = in->cfg.get(in); }
	else if (in->cfg.LOGx)	{ val = RSHIFT(*((uint32_t *) in->cfg.LOGx), in->cfg.LOG_Pos) & 1U; }

	if (val == in->cfg.polarity)
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

