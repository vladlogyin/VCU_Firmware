#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>

#include <stdlib.h>

#include <rtos/rtos.h>
#include <rtos/perf.h>
/**
 * Sets up PLL and alternate function clocksetup
 */
void clocksetup(void);

/**
 * Sets up USART1 for transmission
 */
void usartsetup(void);

/**
 * VCU init task
 *
 * This task manages all other VCU related tasks
 */
class vcu_init_task : public rtos_task
{
public:
  profiler prof;
  vcu_init_task() : rtos_task()
  {
    char task_name[]="vcu init";
    std::strcpy(name,task_name);
  }
  virtual task_status start() override
  {
    rtos::log("Adding profiler task");
    parentScheduler->addTask(&prof);
    return rtos_task::RTOS_TASK_LOOP;
  }
  virtual task_status loop() override
  {
    profiler::startCount(parentScheduler->currentTaskID);
    rtos::log("alive!");
    sleepFor(5000);
    profiler::endCount(parentScheduler->currentTaskID);
    return rtos_task::RTOS_TASK_SLEEP;
  }
};
