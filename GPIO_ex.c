/*!\file GPIO_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Simple extension for GPIOs
**/
/****************************************************************/
#include <string.h>
#include <assert.h>

#include "GPIO_ex.h"
/****************************************************************/


#define MAX_PINS_PORT	16


void GPIO_in_init(	GPIO_in * in,
					GPIO_TypeDef * GPIOx, const uint16_t GPIO_Pin, const bool logic, const uint16_t filter,
					void (*onSet)(void), void (*onReset)(void), const bool repeat)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	in->cfg.GPIOx = GPIOx;
	in->cfg.GPIO_Pin = GPIO_Pin;
	in->cfg.logic = logic;
	in->cfg.filt = filter;
	in->cfg.onSet = onSet;
	in->cfg.onReset = onReset;
	in->cfg.repeat = repeat;
}


void GPIO_in_handler(GPIO_in * in)
{
	if (in->in == in->mem)
	{
		in->edge = NoEdge;
		if ((in->cfg.repeat) && (in->in))
		{
			if (in->cfg.onSet)	{ in->cfg.onSet(); }
		}
	}
	else if (in->in > in->mem)
	{
		in->edge = Rising;
		if (in->cfg.onSet)		{ in->cfg.onSet(); }
	}
	else
	{
		in->edge = Falling;
		if (in->cfg.onReset)	{ in->cfg.onReset(); }
	}

	in->mem = in->in;
	if (HAL_GPIO_ReadPin(in->cfg.GPIOx, in->cfg.GPIO_Pin) == (GPIO_PinState) in->cfg.logic)
	{
		if (TPSSUP_MS(in->hIn, in->cfg.filt))	{ in->in = true; }
	}
	else
	{
		in->in = false;
		in->hIn = HALTicks();
	}
}


FctERR str_GPIO_name(char * name, const GPIO_TypeDef * GPIOx, const uint16_t GPIO_Pin)
{
	const char *port, prt[][7] = { "GPIOA", "GPIOB", "GPIOC", "GPIOD", "GPIOE", "GPIOF", "GPIOG", "GPIOH", "GPIO?" };

	/* Check the parameters */
	assert_param(name);

	if (!name)	{ return -1; }	// pointer for storage is not defined

	// Find port comparing address
	if (GPIOx == GPIOA)			port = prt[0];
#if defined(GPIOB)
	else if (GPIOx == GPIOB)	port = prt[1];
#endif
#if defined(GPIOC)
	else if (GPIOx == GPIOC)	port = prt[2];
#endif
#if defined(GPIOD)
	else if (GPIOx == GPIOD)	port = prt[3];
#endif
#if defined(GPIOE)
	else if (GPIOx == GPIOE)	port = prt[4];
#endif
#if defined(GPIOF)
	else if (GPIOx == GPIOF)	port = prt[5];
#endif
#if defined(GPIOG)
	else if (GPIOx == GPIOG)	port = prt[6];
#endif
#if defined(GPIOH)
	else if (GPIOx == GPIOH)	port = prt[7];
#endif
	else						port = prt[8];

	// Find pin shifting values to get pin index
	for (int pin = 0 ; pin < MAX_PINS_PORT ; pin++)
	{
		if (1U << pin == GPIO_Pin)
		{
			sprintf(name, "%s%i", port, pin);
			return ERROR_OK;	// Match
		}
	}
	sprintf(name, "%s%s", port, "xx");
	return ERROR_VALUE;		// No match
}
