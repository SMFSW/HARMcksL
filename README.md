# HARMcksL

ARM HAL toolbox (STM32 HAL based)

HARMcksL stands for:
- ARM
- Hacks
- HAL
- Harm: what do you expect running with scissors?

... coming to you bundled in one library.
Use it at your own risk!

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

## Requires

The toolbox uses sarmfsw headers library

- sarmfsw
  - [sarmfsw](https://github.com/SMFSW/sarmfsw)

## Documentation

Doxygen doc can be generated using "Doxyfile"

See [generated documentation](https://smfsw.github.io/HARMcksL/)

## Release Notes

See [release notes](ReleaseNotes.md)
