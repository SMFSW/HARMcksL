/*!\file Logic_in.h
** \author SMFSW
** \copyright MIT (c) 2017-2024, SMFSW
** \brief Logic input handling
**/
/****************************************************************/
#ifndef __LOGIC_IN_H
	#define __LOGIC_IN_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\struct Logic_in
** \brief Logic input structure
**/
typedef struct Logic_in {
	bool			in;											//!< Input value
	eEdge			edge;										//!< Input edge
	/*pvt*/
	bool			mem;										//!< Memo value
	uint32_t		hIn;										//!< Filter time
	struct {
	void			(*onSet)(const struct Logic_in * const);	//!< Push callback ON function pointer
	void			(*onReset)(const struct Logic_in * const);	//!< Push callback OFF function pointer
	GPIO_PinState	(*get)(const struct Logic_in * const);		//!< Getter function
	void *			LOGx;
	uint16_t		LOG_Pos;									//!< Monitored bit position in variable
	uint16_t		filt;										//!< Filter time (ms)
	GPIO_PinState	polarity;									//!< Input polarity
	bool			repeat;										//!< Callback ON repeat
	} cfg;
} Logic_in;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Get Logic_in input value
** \param[in] in - input instance
** \return Input value
**/
__INLINE bool NONNULL_INLINE__ get_Logic_in(const Logic_in * const in) {
	return in->in; }


/*!\brief Get Logic_in input edge
** \param[in] in - input instance
** \return Input edge
**/
__INLINE bool NONNULL_INLINE__ get_Logic_in_edge(const Logic_in * const in) {
	return in->edge; }


/*!\brief Initialize Logic_in instance
** \param[in,out] in - input instance to initialize
** \param[in] getter - Pointer to variable getter function (may be NULL: default behavior for handling RAM variable at address \b addr)
** \param[in] addr - Variable address to read from (pointer to unsigned 32b, may be NULL if getter handles everything)
** \param[in] pos - monitored bit position (may be unused if getter function is used)
** \param[in] polarity - active state
** \param[in] filter - input filtering time
** \param[in] onSet - Pointer to callback ON function
** \param[in] onReset - Pointer to callback OFF function
** \param[in] repeat - To repeat callback ON as long as input is set
**/
void NONNULLX__(1) Logic_in_init(	Logic_in * const in, GPIO_PinState (*getter)(const Logic_in * const),
									uint32_t * const addr, const uint32_t pos, const GPIO_PinState polarity, const uint16_t filter,
									void (*onSet)(const Logic_in * const), void (*onReset)(const Logic_in * const), const bool repeat);


/*!\brief Handles Logic_in read and treatment
** \param[in,out] in - input instance to handle
**/
void NONNULL__ Logic_in_handler(Logic_in * const in);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __LOGIC_IN_H */
/****************************************************************/
