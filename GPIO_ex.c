/*!\file GPIO_ex.c
** \author SMFSW
** \version v0.3
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Simple extension for GPIOs
**/
#include <string.h>
#include <assert.h>

#include "GPIO_ex.h"

#define MAX_PINS_PORT	16


int str_GPIO_name(char * name, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	const char *port, prt[][7] = { "GPIOA", "GPIOB", "GPIOC", "GPIOD", "GPIOE", "GPIOF", "GPIOG", "GPIOH", "GPIO?" };

	/* Check the parameters */
	assert_param(name);
	assert_param(IS_GPIO_PIN(GPIO_Pin));

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
			return 0;	// Match, return 0
		}
	}
	sprintf(name, "%s%s", port, "xx");
	return -1;			// No match, return -1
}
