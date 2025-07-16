# HARMcksL

[![author](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/auth_SMFSW.json)](https://github.com/SMFSW)
![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/HARMcksL_status_badge.json)
[![license](https://img.shields.io/badge/License-MIT-darkred.svg)](LICENSE)
[![CodeFactor](https://www.codefactor.io/repository/github/smfsw/harmcksl/badge)](https://www.codefactor.io/repository/github/smfsw/harmcksl)

![platform](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/platform_STM32.json)

[![doxy](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/tool_DOXY.json)](https://smfsw.github.io/HARMcksL)
[![re_note](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/tool_RN.json)](ReleaseNotes.md)

![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/SMFSW_rws.json)
![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/SMFSW_fwwm.json)
![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/SMFSW/a9a2b2a02fda1b33461d53ddfe69d649/raw/HARMcksL_custom_repo_badge.json)

ARM HAL toolbox (STM32 HAL based)

HARMcksL stands for:
- ARM
- Hacks
- HAL
- Harm: what do you expect running with scissors?

... coming to you bundled in one library.
Use it at your own risk!

## Requires

The toolbox uses sarmfsw headers library

- sarmfsw
  - [sarmfsw](https://github.com/SMFSW/sarmfsw)

## Modules

- ADC_ex: Extensions for ADC peripherals (convenient way to acquire and convert ADC channels AIO)
  - template [adc_cfg.c](templates/adc_cfg.c.txt)/[adc_cfg.h](templates/adc_cfg.h.txt): ADC configuration to use with ADC_EX
- CRC_stm32: Function dealing with STM32 CRC peripheral
- exceptions: Exceptions debug tool helpers
- flash_check: Function to check flash CRC
- GPIO_ex: Extensions for GPIOs (R/W) and wrapper for other GPIO extensions modules
- GPIO_in: GPIO input handling
  - example [GPIO_in_example.c](templates/GPIO_in_example.c.txt)/[GPIO_in_example.h](templates/GPIO_in_example.h.txt): GPIO_in example
- GPIO_out: GPIO output handling
  - example [GPIO_out_example.c](templates/GPIO_out_example.c.txt)/[GPIO_out_example.h](templates/GPIO_out_example.h.txt): GPIO_out example
- Logic_ex: Extensions for logic variables (R/W) and wrapper for other Logic extensions modules
- Logic_in: Logic input handling
  - example [Logic_in_example.c](templates/Logic_in_example.c.txt)/[Logic_in_example.h](templates/Logic_in_example.h.txt): Logic_in example
- Logic_out: Logic output handling
  - example [Logic_out_example.c](templates/Logic_out_example.c.txt)/[Logic_out_example.h](templates/Logic_out_example.h.txt): Logic_out example
- PWM_IC: PWM Input Capture handling
- PWM_GPIO: GPIO PWM emulation handling
  - example [PWM_GPIO_example.c](templates/PWM_GPIO_example.c.txt)/[PWM_GPIO_example.h](templates/PWM_GPIO_example.h.txt): PWM_GPIO example
- PWM: Straightforward PWM handling
- random_utils: (A little less pseudo) random numbers generation utilities
- RTC_ex: Basic RTC handling
- stack_utils: Stack utilities
- stdream_rdir: Stream redirection (workaround without using syscalls redirections for console, somehow deprecated)
- tick_utils: Core ticks related utilities
- TIM_ex: Extensions for TIM peripherals
- time_utils: Time related utilities
- UART_term: UART terminal
- USB_CDC_IF_ex: Extensions for USB VCP
- WDG_ex: Extensions for WDG peripherals
- workload: Workload estimation utilities

