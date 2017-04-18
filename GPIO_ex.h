/*!\file GPIO_ex.h
** \author SMFSW
** \version v0.2
** \date 2017
** \copyright MIT (c) 2017, SMFSW
** \brief Simple extension for GPIOs
**/
/****************************************************************/
#ifndef __GPIO_EX_H
	#define __GPIO_EX_H
/****************************************************************/
#include <string.h>

#include "sarmfsw.h"
#include CMSIS_INC

/*!\enum enActOut
** \brief Enum des pilotages possibles de sorties logiques
**/
typedef enum enActOut{
	Reset = 0,	//!< Reset Output
	Set,		//!< Set Output
	Toggle		//!< Toggle Output
} ActOut;


int str_GPIO_name(char * name, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


/*!\brief Write GPIO
** \param[in] GPIOx - port to write to
** \param[in] GPIO_Pin - pin to write to
** \param[in] Act - type of write
** \return Nothing
**/
__INLINE void __attribute__((always_inline)) write_GPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, ActOut Act)
{
	if (Act > Toggle)		{ return; }
	else
	{
		if (Act == Reset)	{ HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET); }
		if (Act == Set)		{ HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); }
		if (Act == Toggle)	{ HAL_GPIO_TogglePin(GPIOx, GPIO_Pin); }
		#if defined(VERBOSE)
			char port[10] = "";
			str_GPIO_name(port, GPIOx, GPIO_Pin);
			printf("Written %s to %u at: %lums\n", port, HAL_GPIO_ReadPin(GPIOx, GPIO_Pin), HAL_GetTick());
		#endif
	}
}

/*!\brief Read GPIO
** \param[in] GPIOx - port to read from
** \param[in] GPIO_Pin - pin to read from
** \return Nothing
**/
__INLINE GPIO_PinState __attribute__((always_inline)) read_GPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	#if defined(VERBOSE)
		GPIO_PinState pin = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
		char port[10] = "";
		str_GPIO_name(port, GPIOx, GPIO_Pin);
		printf("Read %s is %u at: %lums\n", port, pin, HAL_GetTick());
		return pin;
	#else
		return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	#endif
}

/****************************************************************/
#endif /* __GPIO_EX_H */
/****************************************************************/
