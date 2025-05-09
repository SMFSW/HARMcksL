/*!\file GPIO_out.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief GPIO output handling
** \note Define \c LOGIC_OUT_IT shared symbol at project level to use \ref GPIO_out from timer interrupts (for more timing precision if required)
** \note When using \ref GPIO_out from interrupts, shared \c LOGIC_OUT_IT_PER period is defined by default with a period of 1000µs (can be customly defined)
** \warning \ref Logic_out & \ref GPIO_out shares interrupt behavior, thus needs to be implemented the same way (it or loop) if both are used at the same time
**/
/****************************************************************/
#include "sarmfsw.h"
#include "GPIO_out.h"

#if defined(HAL_GPIO_MODULE_ENABLED)
/****************************************************************/


/*!\brief Set GPIO port value
** \param[in,out] out - GPIO_out instance
**/
__STATIC_INLINE void NONNULL_INLINE__ GPIO_setter(const GPIO_out * const out, const GPIO_PinState state)
{
	HAL_GPIO_WritePin((GPIO_TypeDef *) out->cfg.LOGx, out->cfg.LOG_Pos, state);
}


FctERR NONNULL__ GPIO_out_init(GPIO_out * const out, GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState polarity)
{
	return Logic_out_init(out, GPIO_setter, (uint32_t *) GPIOx, GPIO_Pin, polarity);
}


/********************************************/
#endif	/* defined(HAL_GPIO_MODULE_ENABLED) */
/********************************************/
