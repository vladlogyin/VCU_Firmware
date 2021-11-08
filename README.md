# Firmware for UGRacing's 2022 VCU
This codebase is written with STM32 MCUs in mind, but should be portable to all other cortex-m MCUs supported by [libopencm3](https://github.com/libopencm3/libopencm3)

Currently targeting the STM32F407VGT6 (cortex m4)
Other candidates are:
- SAMC21N/J/G (dual CAN) (cortex m0+) (somewhat cheap)
- STM32F105/7 (dual CAN) (cortex m3) (familiar)
- STM32F205/7 (dual can) (cortex m3) (familiar) (very cheap)

# Hardware assignments

## UARTs
- UART1 is used for debugging
## Timers
- SYSTICK is used by systime
- TIM6 is used by the profiler
## TODO

- Implement IPC mechanism
- Implement some sort of a shell
- Implement istream
- Implement stream buffering (ideally use DMA when flushing the buffer)




