# HARMcksL (release notes)

The MIT License (MIT)

Copyright (c) 2017-2025 SMFSW (Sebastien Bizien)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## v1.6

* Doxygen comments review and fixes
* UART_term: fix call to str_clr (now strclr)
* Logic_in & GPIO_in: edge getters return type fix
* PWM_IC: refactoring to catch continous signals (after some timeout) + enabling possibility to use without interrupts
* UART_term: handling reception of backspace char
* RTC_ex: Setting date automatically computes corresponding weekday
* RTC_ex: Sets weekday according to STM32 RTC definitions
* time_utils: Functions added to get strings of Day and Month
* time_utils: Months starts from 1 from now on
* WDG_ex: Functions added to get current period of IWDG 
* WDG_ex: Functions added to save/restore watchdogs configurations
* tick_utils: init function returns FctERR instead of HAL_StatusTypeDef
* workload: Module added
* More HAL_ENABLED checks
* USB_CDC_IF_ex: USE_USB_CDC_IF_EX symbol check to enable features
* TIM_ex: H7 family TIM peripherals handled

## v1.5

* GPIO_in & Logic_in: Internal init fix to be in release state (no spurious callback triggered after init)
* TIM_ex: fix possible div by 0
* ADC_ex: Fixes for compensation when calibrated values differ from VDD_VALUE
* ADC_ex: Added customizable pre-processing symbols to allow better ADC configuration when needed (ADC_RESOLUTION, Def_VAlim, Def_VCal, Def_VBatFactor)
* ADC_ex: Few more families addressed (whole F G L ranges, H7, U5, WB, WL / missing C0, MP1, H5, WBA)
* USB_CDC_IF_ex module added
* Fix ADC_cfg template name case
* __IO changed to __IOM for struct members (conformance to symbols specifications)
* stdream_rdir & UART_term: use of size_t typedef for length parameters
* exceptions: HardFault_Handler_callback checks for __CORTEX_M symbol definition and value to add extra informations
* Some unneeded assert_param macros removed (when already done later by HAL functions)
* TIM_ex: get_TIM_clock refactored (check for bus peripheral CLK)
* TIM_ex: get_TIM_clock renamed to RCC_GetTIMCLKFreq
* ADC_ex: header comments update (HAL config)
* GPIO_in & Logic_in: Refactored to be using same structure definition (Logic_in structure extended with data address if needed)
* Logic_out: added (logical output abstract of physical layer, may be bit in a variable or a dedicated setter function)
* GPIO_out: Refactored to be using Logic_out module
* Logic_ex: former code moved to Logic_in, now only wrapping Logic_in & Logic_out (keeping include compatibility with older versions)
* Logic_ex: Masking read value after shifting, otherwise value can be in set state if higher significant bit is set in monitored variable
* Logic_ex: Read is done prior to treatment (treatment is executed in the current call of Logic_in_handler without further delay)
* WDG_ex: added
* WDG_ex: updated to work with single core families (dual core will have to be implemented later)
* WDG_ex: fixes for G0 family & WWDG (in general)
* WDG_ex: set_IWDG_Period_xx functions added
* WDG_ex: added missing peripherals testing in preprocessing
* stdream_rdir & UART_Term: sending without using interrupts by default and comments added
* stdream_rdir: stdout_no_buffer inline added with comments to explain behaviors
* stdream_rdir: fix in __io_putchar code
* tick_utils: init function added (for consistency with the use of a TIM instance)
* tick_utils: possibility added to use a TIM peripheral instance for us delay generation (for M0/M0+ cores, or on purpose for other cores)
* tick_utils: added Delay_ms function
* TIM_ex: set_TIM_Tick_Freq added to set a TIM peripheral to get a free running counter with desired tick frequency
* UART_Term: UART_Term_Init function added to initialize message queue and launch interrupts (internal code updated)
* Doxyfiles updated to reflect changes in Doxygen v1.9.x

## v1.4

* std libs includes reviewed in all files
* strict aliasing types in printf statements
* templates: Added folder (with templates and examples)
* templates: refactoring of examples
* Added cpp preprocessor directives in headers
* Doxyfile update
* const pointers in functions parameters reviewed (for const pointer, const content or both)
* Use of NONNULL__ attribute instead of checking pointer set to value in code
* exceptions: Added function to catch last reset source (unfortunately can't be included in sarmfsw lib)
* exceptions: Changes using G0 family
* exceptions: Added reset sources for L4 family (Get_Reset_Source)
* exceptions: Added reset sources for F7, H7 families (Get_Reset_Source)
* ADC_ex: Def_VIn & Def_Step moved to header file
* ADC_ex: Using ADC_COMPENSATION, Hardware fault may occur at MCU start when average Vref == 0 (not ready), returns 0 for any query in this case
* ADC_ex: split slope and reference temp defines
* ADC_ex: alias TEMP_CALC_V25... to TEMP_CALC_VTEMP (for legacy use)
* ADC_ex: alias & fix for Def_V25... to Def_VTemp (for legacy use)
* ADC_ex: merged static ADC_ConvertVal in public ADC_GetConvertedVal
* ADC_ex: undef TEMP_CALC_VTEMP for L0/L1 temperature conversion (using calibration values instead)
* adc_cfg.h: template update following changes
* ADC_ex: Added base ADC module for single ended without injection conversions (with support from ADC_cfg.h in project)
* ADC_ex: Added USE_ADC_EX symbol test to enable ADC_ex functions
* ADC_ex: Def_VIn automatically computed
* ADC_ex: ADC_CALIBRATION & ADC_CALIBRATION set when not available only generates warning now (symbol undef)
* ADC_ex: Some conversion calculations changed for various families
* CRC_stm32: Added functions to compute and check STM32 CRC using CRC peripheral
* flash_check: Added to check flash against computed CRC (using FLASH_SIZE defined in sarmfsw)
* GPIO_out: GPIO_out module added
* GPIO_out: init variable added in instance to ensure no action before instance properly initialized 
* GPIO_out: GPIO_OUT_IT can be defined at project level to adapt code for handler called inside TIM interrupt
* GPIO_ex: split between GPIO_ex and GPIO_in files (GPIO_in header included in GPIO_ex as a wrapper for all GPIO extensions)
* GPIO_ex: On and Off callbacks have pointer to GPIO_in passed as argument (with fix to struct typedef instead of void*)
* GPIO_ex: str_GPIO_name optimized
* GPIO_ex: refactoring and including stdio.h
* GPIO_ex: cut code to Logic_ex to make it generic for both GPIO (working same as before) and any variable  
* Logic_ex: cut from GPIO_ex GPIO_in (and now used by GPIO_in) to have code working to monitor bit in a variable as a logic input signal
* Logic_ex: set working elements to 0 at instance init (no need of initialized struct)
* PWM: split between PWM and PWM_GPIO files (PWM_GPIO module function and types names refactored)
* PWM: Set preload configuration bit when initializing PWM channel
* PWM: changed pre-comp test for consistency
* PWM: set_PWM_Duty_Scaled calculations now only using integers
* PWM: more precision when trying to set TIM frequency (still 16b max)
* PMW: logic PWM getters now returns value directly instead of using pointer in function parameters
* PWM: added start_polarity parameter in init_PWM_Chan (to ensure there is no glitch with wrong polarity at PWM engine start)
* PWM: added clock path for G0 family in get_TIM_clock
* PWM: init_PWM_Chan refactored to produce less code and performing more checks not to init undesired or non-existing TIM channel
* PWM: set_PWM_Duty_Scaled duty & scale parameters needs to be 32b (for 32b timers TIM2 & TIM5) + computation cast to uint64_t to operate correctly 
* PWM_IC: Input capture for PWM input module added
* PWM_IC: Measured Frequency & Duty Cycle getters added
* PWM_IC: Removed active channel 5 & 6 in init_PWM_IC (seems those channels, when available cannot be used for Input Capture)
* TIM_ex: PWM file split between TIM_ex & PWM (with functions name refactoring)
* TIM_ex: set_TIM_Freq handling for TIM2 & TIM5 32b range period (increasing output duty cycle accuracy depending requested freq)
* TIM_ex: get_TIM_clock not static (can be used to get TIM reference clock when needed)
* TIM_ex: write_TIM_CCR CCR_val needs to be 32b (for 32b timers TIM2 & TIM5)
* stdream_rdir & UART_term: refactoring
* stdream_rdir: added pre-comp option for ITM
* tick_utils: Delay_us added
* tick_utils: Delay_us disabled on cortex M0/M0+ (no DWT peripheral)
* tick_utils: Fix core debug trace not necessarily enabled
* time_utils: Added functions to determine if leap year and weekday for a given date
* UART_term: dissociated HAL & UART Term callbacks (to be able to call UART Term callbacks in project from HAL callbacks with multiple busses to handle)
* pattern2D: removed (now part of sarmfsw in arm_inlines_linearization.h)
* stdream_rdir & UART_term: total overhaul (using syscalls to write data to stream). Backward compatibility kept, yet discouraged with a warning
* stdream_rdir & UART_term: code cleanup, preprocessor directives fix and standard lib includes reviewed
* stdream_rdir: some code added (not activated) & refactoring for syscalls use of input UART stream (only for test, not working, and not that relevant... Maybe removed soon)
* stdream_rdir: interrupt driven disabled for UART_TX when using syscalls redirection
* UART_term: no watchdog refresh while waiting for UART to be ready in UART_Term_Wait_Ready

## v1.3

* Adding support for doxygen documentation generation with Travis CI
* added const qualifier to more function parameters
* pattern2d: added linearization patterns
* UART_term & stdream_rdir: use UART_REDIRECT define to enable UART redirection
* uart_term: SERIAL_DBG_Wait_Ready returns ERROR_NOTAVAIL when UART not initialized
* stdream_rdir: test of SERIAL_DBG_Wait_Ready before sending message (prevents errors when UART is not yet initialized)
* stdream_rdir: renamed ITM_ENABLED to ITM_REDIRECT for consistency with UART_REDIRECT symbol
* PWM: added case where duty cycle == scale in set_PWM_Duty_Scaled (to achieve real 100% duty cycles)
* PWM: check if scale is equal 0 in set_PWM_Duty_Scaled (to avoid Division by 0)
* GPIO_ex: added gpio logic and possible callbacks when input set / reset
* GPIO_ex: removed eActOut (replaced by eGPIOstate in sarmfsw lib)
* exceptions: removed erroneous dump_stack macro (functions will now be in stack_utils)
* exceptions: dumpStack renamed to print_exception_stack, now static to exceptions.c
* stack_utils: new module to get informations from global registers & stack
* stack_utils: stack_dump now refactored to work fine
* random_utils: new rand wrapper module to enhance randomness some more

## v1.2

* removed peripheral includes (already done by sarmfsw library)
* removed FctERR module (and calls to header) which is now part of sarmfsw
* PWM: fixed possible warning in for loop
* PWM: fixed frequency calculation when PLL clock source used (STM32F3)
* PWM: added emulated PWM functionality & added config sub-struct (for low frequency PWM generation on GPIO)
* stdream_rdir: refactoring & removed get_fp_dec (now in sarmfsw)
* UART_term: added UART terminal hacks
* RTC_ex: added some helper functions for RTC peripheral
* time_utils: some functions to deal with time_t
* ERR_xx refactored to ERROR_xx

## v1.1

* PWM: Fixed start/stop PWM channels all at once
* PWM: check for clock path following STM family used
* PWM: Added APB clock automatic scaling for TIM used (clock fed for frequency calculation)
* PWM: Added function to initialize TIM base and start its period elapsed interruptions
* PWM: Added inline to start/stop TIM module period elapsed interruptions
* stddream_rdir: added UNUSED macro when needed to avoid variable unused compilation warnings
* Removed CMSIS & HAL includes from files (already included by sarmfsw library)

## v1.0

* PWM: Added function init_PWM_Chan (calls setFreq & starts PWM channel output)
* PWM: Added inline to Start/Stop PWM channel output
* PWM: added precomp define (from sarmfsw) to define channel 5 & 6 only for F3 (may be needed on some other families)
* FctERR: enum is now signed and precomp defined(lwip) in enum discards duplicates with lwip (while keeping common values)

## v0.7 and below

* initial commit and beta versions
