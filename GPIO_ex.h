/*!\file GPIO_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2018, SMFSW
** \brief Simple extension for GPIOs
**/
/****************************************************************/
#ifndef __GPIO_EX_H
	#define __GPIO_EX_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <string.h>

#include "sarmfsw.h"
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct GPIO_in
** \brief GPIO input structure
**/
typedef struct GPIO_in {
	bool			in;						//!< Input value
	eEdge			edge;					//!< Input edge
	/*pvt*/
	bool			mem;					//!< Memo value
	uint32_t		hIn;					//!< Filter time
	struct {
	GPIO_TypeDef *	GPIOx;					//!< HAL GPIO instance
	uint16_t		GPIO_Pin;				//!< HAL GPIO pin
	uint16_t		filt;					//!< Filter time (ms)
	bool			logic;					//!< Input logic polarity
	bool			repeat;					//!< Callback ON repeat
	void			(*onSet)(void*);		//!< Push callback ON function pointer
	void			(*onReset)(void*);	//!< Push callback OFF function pointer
	} cfg;
} GPIO_in;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Initialize GPIO_in instance
** \param[in,out] in - input instance to initialize
** \param[in] GPIOx - port to read from
** \param[in] GPIO_Pin - pin to read from
** \param[in] logic - set to 0 if pull-up (switching to GND), 1 if pull-down (switching to Vdd)
** \param[in] filter - input filtering time
** \param[in] onSet - Pointer to callback ON function
** \param[in] onReset - Pointer to callback OFF function
** \param[in] repeat - To repeat callback ON as long as input is set
** \return Nothing
**/
void NONNULLX__(1, 2) GPIO_in_init (GPIO_in * in,
									GPIO_TypeDef * GPIOx, const uint16_t GPIO_Pin, const bool logic, const uint16_t filter,
									void (*onSet)(GPIO_in*), void (*onReset)(GPIO_in*), const bool repeat);


/*!\brief Handles GPIO_in read and treatment
** \param[in,out] in - input instance to handle
** \return Nothing
**/
void NONNULL__ GPIO_in_handler(GPIO_in * in);


/*!\brief Get GPIO_in input value
** \param[in] in - input instance
** \return Input value
**/
__INLINE bool NONNULL_INLINE__ get_GPIO_in(const GPIO_in * in) { return in->in; }


/*!\brief Get GPIO_in input edge
** \param[in] in - input instance
** \return Input edge
**/
__INLINE bool NONNULL_INLINE__ get_GPIO_in_edge(const GPIO_in * in) { return in->edge; }


/*!\brief Get name from Port, Pin
** \param[in,out] name - pointer to string for name
** \param[in] GPIOx - port to write to
** \param[in] GPIO_Pin - pin to write to
** \return Error code
**/
FctERR NONNULL__ str_GPIO_name(char * name, const GPIO_TypeDef * GPIOx, const uint16_t GPIO_Pin);


/*!\brief Write GPIO
** \param[in] GPIOx - port to write to
** \param[in] GPIO_Pin - pin to write to
** \param[in] Act - type of write
** \return Nothing
**/
__INLINE void NONNULL_INLINE__ write_GPIO(GPIO_TypeDef * GPIOx, const uint16_t GPIO_Pin, const eGPIOState Act)
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
			printf("Written %s to %u (%lums)\r\n", port, HAL_GPIO_ReadPin(GPIOx, GPIO_Pin), HAL_GetTick());
		#endif
	}
}


/*!\brief Read GPIO
** \param[in] GPIOx - port to read from
** \param[in] GPIO_Pin - pin to read from
** \return Pin state
**/
__INLINE GPIO_PinState NONNULL_INLINE__ read_GPIO(GPIO_TypeDef * GPIOx, const uint16_t GPIO_Pin)
{
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	#if defined(VERBOSE)
		GPIO_PinState pin = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
		char port[10] = "";
		str_GPIO_name(port, GPIOx, GPIO_Pin);
		printf("Read %s is %u (%lums)\r\n", port, pin, HAL_GetTick());
		return pin;
	#else
		return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	#endif
}


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __GPIO_EX_H */
/****************************************************************/
