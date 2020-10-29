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
  - template [adc_cfg.c](templates/adc_cfg.c)/[adc_cfg.h](templates/adc_cfg.h): ADC configuration to use with ADC_EX
- CRC_stm32: Function dealing with STM32 CRC peripheral
- exceptions: Exceptions debug tool helpers
- flash_check: Function to check flash CRC
- GPIO_ex: Extensions for GPIOs (R/W) and wrapper for other GPIO extensions modules
- GPIO_in: GPIO input handling
  - example [GPIO_in_example.c](templates/GPIO_in_example.c)/[GPIO_in_example.h](templates/GPIO_in_example.h): GPIO_in example
- Logic_ex: Extensions for logic variable handling
  - example [Logic_in_example.c](templates/Logic_in_example.c)/[Logic_in_example.h](templates/Logic_in_example.h): Logic_in example
- PWM_IC: PWM Input Capture handling
- PWM_GPIO: GPIO PWM emulation handling
  - example [PWM_GPIO_example.c](templates/PWM_GPIO_example.c)/[PWM_GPIO_example.h](templates/PWM_GPIO_example.h): PWM_GPIO example
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
