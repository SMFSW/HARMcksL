/*!\file GPIO_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Extensions for GPIOs (R/W)
**/
/****************************************************************/
#include <stdio.h>
#include "sarmfsw.h"
#include "GPIO_ex.h"

#if defined(HAL_GPIO_MODULE_ENABLED)
/****************************************************************/


void NONNULL__ write_GPIO(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const eGPIOState action)
{
	if (action == Reset)		{ HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET); }
	else if (action == Set)		{ HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); }
	else if (action == Toggle)	{ HAL_GPIO_TogglePin(GPIOx, GPIO_Pin); }
	else						{ return; }

	#if defined(VERBOSE)
		char port[10] = "";
		str_GPIO_name(port, GPIOx, GPIO_Pin);
		UNUSED_RET printf("Written %s to %u (%lums)\r\n", port, HAL_GPIO_ReadPin(GPIOx, GPIO_Pin), HALTicks());
	#endif
}


GPIO_PinState NONNULL__ read_GPIO(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin)
{
	const GPIO_PinState pin = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

	#if defined(VERBOSE)
		char port[10] = "";
		str_GPIO_name(port, GPIOx, GPIO_Pin);
		UNUSED_RET printf("Read %s is %u (%lums)\r\n", port, pin, HALTicks());
	#endif

	return pin;
}


void NONNULL__ str_GPIO_name(char * name, const GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin)
{
	const uintCPU_t	max_pins = 16U;	// Maximum pins on a port
	const char		prt[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', '?' };
	char			port;

	// Find port by testing GPIO instance
	if (GPIOx == GPIOA)			{ port = prt[0]; }
#if defined(GPIOB)
	else if (GPIOx == GPIOB)	{ port = prt[1]; }
#endif
#if defined(GPIOC)
	else if (GPIOx == GPIOC)	{ port = prt[2]; }
#endif
#if defined(GPIOD)
	else if (GPIOx == GPIOD)	{ port = prt[3]; }
#endif
#if defined(GPIOE)
	else if (GPIOx == GPIOE)	{ port = prt[4]; }
#endif
#if defined(GPIOF)
	else if (GPIOx == GPIOF)	{ port = prt[5]; }
#endif
#if defined(GPIOG)
	else if (GPIOx == GPIOG)	{ port = prt[6]; }
#endif
#if defined(GPIOH)
	else if (GPIOx == GPIOH)	{ port = prt[7]; }
#endif
	else						{ port = prt[8]; }

	// Find pin shifting values to get pin index
	for (uintCPU_t pin = 0U ; pin < max_pins ; pin++)
	{
		if ((1U << pin) == GPIO_Pin)
		{
			UNUSED_RET sprintf(name, "GPIO%c%u", port, pin);
			goto ret;
		}
	}

	// Not found
	UNUSED_RET sprintf(name, "GPIO%c%c", port, 'x');

	ret:
	return;
}


/********************************************/
#endif	/* defined(HAL_GPIO_MODULE_ENABLED) */
/********************************************/
