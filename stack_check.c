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

extern const uint32_t		_sdata;				//!< linker defined reference
extern const uint32_t		_sheap;				//!< heap start address (linker defined reference)
extern const uint32_t		_estack;			//!< linker defined reference
extern const size_t			_Min_Heap_Size;		//!< minimum heap size (linker defined reference)
extern const size_t			_Min_Stack_Size;	//!< minimum stack size (linker defined reference)
extern const uint32_t		_Stack_Fill_Value;	//!< linker defined reference

#define MIN_HEAP_SIZE		((const size_t) &_Min_Heap_Size)		//!<  (given to linker for static analysis)
#define MIN_STACK_SIZE		((const size_t) &_Min_Stack_Size)		//!<  (given to linker for static analysis)

#define FREE_STACK_VALUE	((const uint32_t) &_Stack_Fill_Value)	//!< stack pattern fill value


FctERR stack_check(memory_res_t * const pMem)
{
	const uintptr_t data = (uintptr_t) &_sdata;

	const uint32_t * const pHeap = &_sheap;
	const uint32_t * pHeapHigh = (const uint32_t *) __sbrk_heap_end;
	const uint32_t * const pStackHigh = &_estack;

	const uint32_t * pStack;

	pHeapHigh = (pHeapHigh != NULL) ? pHeapHigh : pHeap;
	pStack = pHeapHigh;

	do
	{
		if (*pStack != FREE_STACK_VALUE)	{ break; }
	}
	while (pStack++ < pStackHigh);

	pMem->heap_start = (uintptr_t) pHeap;
	pMem->heap_break = (uintptr_t) pHeapHigh;
	pMem->stack_addr = (uintptr_t) pStack;

	pMem->data_size = pMem->heap_start - data;
	pMem->heap_size = pMem->heap_break - pMem->heap_start;
	pMem->stack_size = (uintptr_t) pStackHigh - pMem->stack_addr;
	pMem->free_size = (pMem->stack_addr < pMem->heap_break) ? 0 : pMem->stack_addr - pMem->heap_break;

	pMem->stack_ovfw = binEval(pStack <= pHeapHigh);

	pMem->lnk_rsvd_heap_ovfw = binEval(pMem->heap_size > MIN_HEAP_SIZE);
	pMem->lnk_rsvd_stack_ovfw = binEval(pMem->stack_size > MIN_STACK_SIZE);


	return pMem->stack_ovfw ? ERROR_MEMORY : ERROR_OK;
}

/********************************/
#endif	/* defined(STACK_CHECK) */
/********************************/
