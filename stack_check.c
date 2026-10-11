/*!\file stack_check.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Memory check utilities: heap/stack stats and collision detection
** \note Define \c STACK_CHECK symbol at project level to use stack analysis
** \note _Stack_Fill_Value has to be defined in linker script with 32b value to fill stack with (eg: _Stack_Fill_Value = 0xB00BFACE;)
**/
/****************************************************************/
#include <stdint.h>

#include "sarmfsw.h"

#include "stack_check.h"

#if defined(STACK_CHECK)
/****************************************************************/

/**!\warning \c static qualifier of \p __sbrk_heap_end may need to be commented in sysmem.c **/
extern uint8_t *			__sbrk_heap_end;	//!< heap break address (sysmem.c)

extern const uintptr_t		_sdata;				//!< data section start address (linker defined reference)
extern const uintptr_t		_sheap;				//!< heap start address (linker defined reference)
extern const uintptr_t		_estack;			//!< stack end address (linker defined reference)
extern const size_t			_Min_Heap_Size;		//!< minimum heap size (linker defined reference)
extern const size_t			_Min_Stack_Size;	//!< minimum stack size (linker defined reference)
extern const uint32_t		_Stack_Fill_Value;	//!< stack pattern fill value (linker defined reference)


#define PTR(x)		((const uint32_t *) (x))	//!< Cast \c uintptr_t integer to pointer of 32b


FctERR NONNULL__ stack_check(memory_res_t * const pMem)
{
	const uintptr_t data = (uintptr_t) &_sdata;
	const uintptr_t stack_end = (uintptr_t) &_estack;

	pMem->heap_start = (uintptr_t) &_sheap;
	pMem->heap_break = (__sbrk_heap_end != NULL) ? (uintptr_t) __sbrk_heap_end : pMem->heap_start;

	const uint32_t * pStack = PTR(pMem->heap_break);
	do
	{
		if (*pStack != (uint32_t) &_Stack_Fill_Value)	{ break; }
	}
	while (pStack++ < PTR(stack_end));

	pMem->stack_break = (uintptr_t) pStack;

	pMem->data_size = pMem->heap_start - data;
	pMem->heap_size = pMem->heap_break - pMem->heap_start;
	pMem->stack_size = stack_end - pMem->stack_break;

	pMem->stack_ovfw = binEval(pMem->stack_break < pMem->heap_break);
	pMem->free_size = pMem->stack_ovfw ? 0 : pMem->stack_break - pMem->heap_break;

	pMem->lnk_rsvd_heap_ovfw = binEval(pMem->heap_size > (size_t) &_Min_Heap_Size);
	pMem->lnk_rsvd_stack_ovfw = binEval(pMem->stack_size > (size_t) &_Min_Stack_Size);


	return pMem->stack_ovfw ? ERROR_MEMORY : ERROR_OK;
}


/********************************/
#endif	/* defined(STACK_CHECK) */
/********************************/
