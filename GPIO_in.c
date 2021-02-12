/*!\file GPIO_in.c
** \author SMFSW
** \copyright MIT (c) 2017-2021, SMFSW
** \brief GPIO input handling
**/
/****************************************************************/
#include "Logic_ex.h"
#include "GPIO_in.h"
/****************************************************************/


/*!\brief Get GPIO port value
** \param[in,out] in - GPIO_in instance
** \return GPIO port value
**/
__STATIC_INLINE uint32_t NONNULL_INLINE__ GPIO_getter(GPIO_in * const in)
{
	return HAL_GPIO_ReadPin(in->cfg.GPIOx, in->cfg.GPIO_Pin);
}


void NONNULLX__(1, 2) GPIO_in_init(	GPIO_in * const in,
									GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState polarity, const uint16_t filter,
									void (*onSet)(GPIO_in * const), void (*onReset)(GPIO_in * const), const bool repeat)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	in->cfg.GPIOx = GPIOx;
	in->cfg.GPIO_Pin = GPIO_Pin;
	Logic_in_init(&in->logic, (uint32_t (*)(Logic_in *)) GPIO_getter, 0, polarity, filter, (void (*)(Logic_in *)) onSet, (void (*)(Logic_in *)) onReset, repeat);
}
