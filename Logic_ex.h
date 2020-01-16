/*!\file Logic_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief Simple extension for logic variable handling
**/
/****************************************************************/
#ifndef __LOGIC_EX_H
	#define __LOGIC_EX_H

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
	bool			in;										//!< Input value
	eEdge			edge;									//!< Input edge
	/*pvt*/
	bool			mem;									//!< Memo value
	uint32_t		hIn;									//!< Filter time
	struct {
	uint32_t		(*get)(struct Logic_in * const);		//!< Getter function
	uint32_t		pos;									//!< Monitored bit position in variable
	uint16_t		filt;									//!< Filter time (ms)
	bool			logic;									//!< Input logic polarity
	bool			repeat;									//!< Callback ON repeat
	void			(*onSet)(struct Logic_in * const);		//!< Push callback ON function pointer
	void			(*onReset)(struct Logic_in * const);	//!< Push callback OFF function pointer
	} cfg;
} Logic_in;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Initialize Logic_in instance
** \param[in,out] in - input instance to initialize
** \param[in] getter - Pointer to variable getter function
** \param[in] pos - monitored bit position
** \param[in] logic - active state
** \param[in] filter - input filtering time
** \param[in] onSet - Pointer to callback ON function
** \param[in] onReset - Pointer to callback OFF function
** \param[in] repeat - To repeat callback ON as long as input is set
**/
void NONNULLX__(1, 2) Logic_in_init(Logic_in * const in,
									uint32_t (*getter)(Logic_in * const), const uint32_t pos, const bool logic, const uint16_t filter,
									void (*onSet)(Logic_in * const), void (*onReset)(Logic_in * const), const bool repeat);


/*!\brief Handles Logic_in read and treatment
** \param[in,out] in - input instance to handle
**/
void NONNULL__ Logic_in_handler(Logic_in * const in);


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


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* __LOGIC_EX_H */
/****************************************************************/
