/*!\file GPIO_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief Simple extension for GPIOs
**/
/****************************************************************/
#include <stdio.h>

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
	in->cfg.onSet = onSet;
	in->cfg.onReset = onReset;
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


/*!\brief Get name from Port, Pin
** \note Static, but may not be called, no static declaration to prevent warnings
** \param[in,out] name - pointer to string for name
** \param[in] GPIOx - port
** \param[in] GPIO_Pin - pin
** \return Error code
**/
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
	return ERROR_VALUE;			// No match
}


void NONNULL__ write_GPIO(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const eGPIOState Act)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	if (Act > Toggle)		{ return; }
	else
	{
		if (Act == Reset)	{ HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET); }
		if (Act == Set)		{ HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); }
		if (Act == Toggle)	{ HAL_GPIO_TogglePin(GPIOx, GPIO_Pin); }
		#if defined(VERBOSE)
			char port[10] = "";
			str_GPIO_name(port, GPIOx, GPIO_Pin);
			printf("Written %s to %u (%lums)\r\n", port, HAL_GPIO_ReadPin(GPIOx, GPIO_Pin), HALTicks());
		#endif
	}
}


GPIO_PinState NONNULL__ read_GPIO(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	const GPIO_PinState pin = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

	#if defined(VERBOSE)
		char port[10] = "";
		str_GPIO_name(port, GPIOx, GPIO_Pin);
		printf("Read %s is %u (%lums)\r\n", port, pin, HALTicks());
	#endif

	return pin;
}
