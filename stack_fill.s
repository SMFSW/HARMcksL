/**!\file stack_fill.s
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Memory check utilities: stack filler
** \note _Stack_Fill_Value has to be defined in linker script with 32b value to fill stack with (eg: _Stack_Fill_Value = 0xB00BFACE;)
** \note MemStackFill has to be called by startup routine (after memory init steps, AND prior to any memory used on heap or stack),
**		You may want to call stack fill routine:\n
**		1) for every build target, add: \code bl MemStackFill \endcode
**		2) only for specific targets:
** \code PREFERED METHOD
	  // Declare weak external symbol. If the linker finds a real definition elsewhere, it's used;
	  // if not, the symbol resolves to 0 (NULL) instead of causing a link error.
	  .weak MemStackFill
	  // Load the (possibly 0) address of external symbolinto r0. This is resolved by the linker,
	  // so it correctly reflects whether the symbol was ultimately defined anywhere in the final image.
	  ldr r0, =MemStackFill
	  // "Compare and Branch if Zero": if r0 == 0 (symbol was never defined, so it's NULL),
	  // skip the call and jump to label 1.
	  cbz r0, 1f
	  // Otherwise, branch-and-link to the address in r0, i.e. call optional_cb().
	  // "x" allows switching between ARM/Thumb state if needed.
	  blx r0
	1: // Local label "1:" — landing point when there was nothing to call.
	// `cbz .. 1f` is GNU assembler's **local label** syntax, not a special instruction field.
	// **The rule:**
	// - A label like `1:` (just a digit) is a *local/numeric label* — you can reuse the number `1` many times in the same file for different spots.
	// - When you *reference* it, you must say which direction to search:
	//  - `1f` = search **f**orward from here for the next `1:` label.
	//  - `1b` = search **b**ackward from here for the nearest previous `1:` label.
	\endcode
** \code CORES WITHOUT CBZ INSTRUCTION SUPPORT
...
//Call memory stack filler (following build target)
  bl MemStackFill
...

// Create weak MemStackFill routine (in startup file)
  .section  .text.MemStackFill
  .weak  MemStackFill
  .type  MemStackFill, %function
// Fill the user heap & stack.
MemStackFill:
  bx lr

  .size  MemStackFill, .-MemStackFill
	\endcode
**/

.syntax unified
.thumb

.global  MemStackFill

/* Stack fill value defined in linker script */
.word _Stack_Fill_Value
/* start address for the user heap section defined in linker script */
.word  _sheap
/* start address for the user stack section (last RAM address) defined in linker script */
.word  _estack

  .section  .text.MemStackFill
  .type  MemStackFill, %function
/* Fill the user heap & stack */
MemStackFill:
  ldr r2, =_sheap
  ldr r4, =_estack
  ldr r3, =#_Stack_Fill_Value
  b LoopFillHeapStack

FillHeapStack:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillHeapStack:
  cmp r2, r4
  bcc FillHeapStack
  bx lr

  .size  MemStackFill, .-MemStackFill
