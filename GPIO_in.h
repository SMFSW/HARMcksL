/*!\file GPIO_in.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief GPIO input handling
**/
/****************************************************************/
#ifndef __GPIO_IN_H
	#define __GPIO_IN_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
#include "Logic_ex.h"
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct GPIO_in
** \brief GPIO input structure
**/
typedef struct GPIO_in {
 	//!\warning Make sure logic member is always first member of the struct, otherwise can't be shared with Logic_in type
	Logic_in		logic;		//!< Logic_in instance
	struct {
	GPIO_TypeDef *	GPIOx;		//!< HAL GPIO instance
	uint16_t		GPIO_Pin;	//!< HAL GPIO pin
	} cfg;
} GPIO_in;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get GPIO_in input value
** \param[in] in - input instance
** \return Input value
**/
__INLINE bool NONNULL_INLINE__ get_GPIO_in(const GPIO_in * const in) {
	return in->logic.in; }


/*!\brief Get GPIO_in input edge
** \param[in] in - input instance
** \return Input edge
**/
__INLINE bool NONNULL_INLINE__ get_GPIO_in_edge(const GPIO_in * const in) {
	return in->logic.edge; }


/*!\brief Initialize GPIO_in instance
** \param[in,out] in - input instance to initialize
** \param[in] GPIOx - port to read from
** \param[in] GPIO_Pin - pin to read from
** \param[in] logic - set to 0 if active state is GND, 1 if Vdd
** \param[in] filter - input filtering time
** \param[in] onSet - Pointer to callback ON function
** \param[in] onReset - Pointer to callback OFF function
** \param[in] repeat - To repeat callback ON as long as input is set
**/
void NONNULLX__(1, 2) GPIO_in_init(	GPIO_in * const in,
									GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin, const bool logic, const uint16_t filter,
									void (*onSet)(GPIO_in * const), void (*onReset)(GPIO_in * const), const bool repeat);


/*!\brief Handles GPIO_in read and treatment
** \param[in,out] in - input instance to handle
**/
__INLINE void NONNULL_INLINE__ GPIO_in_handler(GPIO_in * const in) {
	Logic_in_handler((Logic_in *) in); }


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __GPIO_IN_H */
/****************************************************************/
