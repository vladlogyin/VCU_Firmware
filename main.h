#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>

#include <stdlib.h>

#include <rtos/rtos.h>

#include <vcu/vcu_tasks.h>
/**
 * Sets up PLL
 */
void clocksetup(void);

/**
 * Sets up USART1 for transmission
 */
void usartsetup(void);
