# HARMcksL (release notes)

The MIT License (MIT)

Copyright (c) 2017-2019 SMFSW (Sebastien Bizien)

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

## v1.4

* templates: Added folder (with adc_cfg example)
* Added cpp preprocessor directives in headers
* Doxyfile update
* const pointers in functions parameters reviewed (for const pointer, const content or both)
* Use of NONNULL__ attribute instead of checking pointer set to value in code
* exceptions: Added function to catch last reset source (unfortunately can't be included in sarmfsw lib)
* exceptions: Changes using G0 family
* exceptions: Added reset sources for L4 family (Get_Reset_Source)
* ADC_ex: Added base ADC module for single ended without injection conversions (with support from ADC_cfg.h in project)
* ADC_ex: Added USE_ADC_EX symbol test to enable ADC_ex functions
* ADC_ex: Def_VIn automatically computed
* ADC_ex: ADC_CALIBRATION & ADC_CALIBRATION set when not available only generates warning now (symbol undef)
* ADC_ex: Some conversion calculations changed for various families
* CRC_stm32: Added functions to compute and check STM32 CRC using CRC peripheral
* flash_check: Added to check flash against computed CRC (using FLASH_SIZE defined in sarmfsw)
* GPIO_ex: On and Off callbacks have pointer to GPIO_in passed as argument
* GPIO_ex: str_GPIO_name optimized
* GPIO_ex: refactoring and including stdio.h
* PWM: Set preload configuration bit when initializing PWM channel
* PWM: Renamed write_CRR to Set_PWM_CCR
* PWM: changed pre-comp test for consistency
* PWM: set_PWM_Duty_Scaled calulations now only using integers
* PWM: more precision when trying to set TIM frequency (still 16b max)
* PMW: logic PWM getters now returns value directly instead of using pointer in function parameters
* PWM: added start_polarity parameter in init_PWM_Chan (to ensure there is no glitch with wrong polarity at PWM engine start)
* PWM: added clock path for G0 family in get_TIM_clock
* stdream_rdir & UART_term: refactoring
* stdream_rdir: added pre-comp option for ITM
* UART_term: dissociated HAL & UART Term callbacks (to be able to call UART Term callbacks in project from HAL callbacks with multiple busses to handle)
* pattern2D: removed (now part of sarmfsw in arm_inlines_linearization.h)

## v1.3

* Adding support for doxygen documentation generation with Travis CI
* added const qualifier to more function paramaters
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
