/*!\file GPIO_in.c
** \author SMFSW
** \copyright MIT (c) 2017-2023, SMFSW
** \brief GPIO input handling
**/
/****************************************************************/
#include "sarmfsw.h"
#include "GPIO_in.h"

#if defined(HAL_GPIO_MODULE_ENABLED)
/****************************************************************/


/*!\brief Get GPIO port value
** \param[in,out] in - GPIO_in instance
** \return GPIO port value
**/
__STATIC_INLINE GPIO_PinState NONNULL_INLINE__ GPIO_getter(GPIO_in * const in)
{
	return HAL_GPIO_ReadPin(in->cfg.LOGx, in->cfg.LOG_Pos);
}


void NONNULLX__(1, 2) GPIO_in_init(	GPIO_in * const in,
									GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState polarity, const uint16_t filter,
									void (*onSet)(const GPIO_in * const), void (*onReset)(const GPIO_in * const), const bool repeat)
{
	Logic_in_init(	in, (GPIO_PinState (*)(const Logic_in * const)) GPIO_getter, (uint32_t *) GPIOx, GPIO_Pin, polarity, filter,
					(void (*)(const Logic_in * const)) onSet, (void (*)(const Logic_in * const)) onReset, repeat);
}


/********************************************/
#endif	/* defined(HAL_GPIO_MODULE_ENABLED) */
/********************************************/
