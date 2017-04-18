/*!\file GPIO_ex.c
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Simple extension for GPIOs
**/
#include <string.h>

#include "GPIO_ex.h"

#define MAX_PINS_PORT	16


int str_GPIO_name(char * name, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	const char *port, prt[][7] = { "GPIOA", "GPIOB", "GPIOC", "GPIOD", "GPIOF", "GPIO?" };
	char pin;

	if (!name)	{ return -1; }	// pointer for storage is not defined

	// Find port comparing address
	if (GPIOx == GPIOA)			{ port = prt[0]; }
	else if (GPIOx == GPIOB)	{ port = prt[1]; }
	else if (GPIOx == GPIOC)	{ port = prt[2]; }
	else if (GPIOx == GPIOD)	{ port = prt[3]; }
	else if (GPIOx == GPIOF)	{ port = prt[4]; }
	else						{ port = prt[5]; }

	// Find pin shifting values to get pin index
	for (pin = 0 ; pin < MAX_PINS_PORT ; pin++)
	{
		if (1U << pin == GPIO_Pin)
		{
			sprintf(name, "%s%i", port, pin);
			return 0;	// Match, return 0
		}
	}
	sprintf(name, "%s%s", port, "xx");
	return -1;			// No match, return -1
}
