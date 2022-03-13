/*!\file GPIO_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2022, SMFSW
** \brief Extensions for GPIOs (R/W)
**/
/****************************************************************/
#ifndef __GPIO_EX_H
	#define __GPIO_EX_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "GPIO_in.h"
#include "GPIO_out.h"
/****************************************************************/


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Write GPIO
** \param[in] GPIOx - port to write to
** \param[in] GPIO_Pin - pin to write to
** \param[in] action - action to perform on output port pin
**/
void NONNULL__ write_GPIO(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const eGPIOState action);


/*!\brief Read GPIO
** \param[in] GPIOx - port to read from
** \param[in] GPIO_Pin - pin to read from
** \return Pin state
**/
GPIO_PinState NONNULL__ read_GPIO(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin);


/*!\brief Get name from Port, Pin
** \param[in,out] name - pointer to string for name
** \param[in] GPIOx - port
** \param[in] GPIO_Pin - pin
** \return Error code
**/
FctERR NONNULL__ str_GPIO_name(char * name, const GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __GPIO_EX_H */
/****************************************************************/
