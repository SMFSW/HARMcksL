# HARMcksL [![Build Status](https://travis-ci.com/SMFSW/HARMcksL.svg?branch=master)](https://travis-ci.com/SMFSW/HARMcksL)

ARM HAL toolbox (STM32 HAL based)

HARMcksL stands for:
- ARM
- Hacks
- HAL
- Harm: what do you expect running with scissors?

... coming to you bundled in one library.
Use it at your own risk!

## Modules

- ADC_ex: extensions for ADC peripherals (convenient way to acquire and convert ADC channels AIO) 
- CRC_stm32: Function dealing with STM32 CRC peripheral
- exceptions: Exceptions debug tool helpers
- flash_check: Function to check flash CRC
- GPIO_ex: Extensions for GPIOs (R/W)
- Logic_ex: Extensions for logic variable handling
- PWM_IC: PWM Input Capture handling
- PWM: Straightforward PWM handling
- random_utils: (A little less pseudo) random numbers generation utilities
- RTC_ex: Basic RTC handling
- stack_utils: Stack utilities
- stdream_rdir: Stream redirection (workaround without using syscalls redirections for console, somehow deprecated)
- tick_utils: Core ticks related utilities
- TIM_ex: Extensions for TIM peripherals
- time_utils: Time related utilities
- UART_term: UART terminal

## Requires

The toolbox uses sarmfsw headers library

- sarmfsw
  - [sarmfsw](https://github.com/SMFSW/sarmfsw)

## Documentation

Doxygen doc can be generated using "Doxyfile"

See [generated documentation](https://smfsw.github.io/HARMcksL/)

## Release Notes

See [release notes](ReleaseNotes.md)
