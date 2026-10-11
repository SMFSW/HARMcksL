/*!\file stack_check.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Memory check utilities: heap/stack stats and collision detection
** \note Define \c STACK_CHECK symbol at project level to use stack analysis
** \note _Stack_Fill_Value has to be defined in linker script with 32b value to fill stack with (eg: _Stack_Fill_Value = 0xB00BFACE;)
**/
/****************************************************************/
#ifndef STACK_CHECK_H__
	#define STACK_CHECK_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


/*!\struct memory_res_t
** \brief Memory health results structure
**/
typedef struct {
	uintptr_t	heap_start;				//!< heap start address
	uintptr_t	heap_break;				//!< heap break address
	uintptr_t	stack_break;			//!< max static stack cost address
	size_t		data_size;				//!< Static data size
	size_t		heap_size;				//!< Currently used heap size
	size_t		stack_size;				//!< Currently used stack size
	size_t		free_size;				//!< Free RAM size
	bool		stack_ovfw;				//!< stack overflow flag
	bool		lnk_rsvd_heap_ovfw;		//!< Linker defined minimal heap overflow (informational)
	bool		lnk_rsvd_stack_ovfw;	//!< Linker defined minimal stack overflow (informational)
} memory_res_t;


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
/*!\brief Stack check routine
** \param[in,out] pMem - Pointer to stack memory check results
** \return Error code
** \retval ERROR_OK if stack is OK
** \retval ERROR_MEMORY if stack overflow detected
**/
FctERR NONNULL__ stack_check(memory_res_t * const pMem);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* STACK_CHECK_H__ */
/****************************************************************/
