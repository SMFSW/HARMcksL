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


void NONNULLX__(1, 2) GPIO_in_init(	GPIO_in * const in,
									GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const bool logic, const uint16_t filter,
									void (*onSet)(GPIO_in * const), void (*onReset)(GPIO_in * const), const bool repeat)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	in->cfg.GPIOx = GPIOx;
	in->cfg.GPIO_Pin = GPIO_Pin;
	in->cfg.logic = logic;
	in->cfg.filt = filter;
	in->cfg.onSet = (void *) onSet;
	in->cfg.onReset = (void *) onReset;
	in->cfg.repeat = repeat;
}


void NONNULL__ GPIO_in_handler(GPIO_in * const in)
{
	if (in->in == in->mem)
	{
		in->edge = NoEdge;
		if ((in->cfg.repeat) && (in->in))
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


FctERR NONNULL__ str_GPIO_name(char * name, const GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin)
{
	const char	prt[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', '?' };
	char		port;

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
		if (1 << pin == GPIO_Pin)
		{
			sprintf(name, "%s%c%i", "GPIO", port, pin);
			return ERROR_OK;	// Match
		}
	}
//	sprintf(name, "%s%c%c", "GPIO", port, 'x');
//	return ERROR_VALUE;			// No match
}
