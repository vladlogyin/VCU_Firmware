#ifndef PERF_H
#define PERF_H

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

#include <rtos/config.h>
#include <rtos/task.h>
#include <rtos/rtos.h>

#define RTOS_PROFILER_TIMER TIM6
#define RTOS_PROFILER_TIMER_RCC RCC_TIM6

class profiler : public rtos_task
{
public:
  profiler();

  /**
   * Update flag
   * 
   * Set this to true to trigger the task to print the performance counters
   */
  bool updateStats;

  /**
   * Counter depth flags
   * 
   * These are flags to determine if we're already timing a certain task. uint8_t is used instead of bool to enable nested irq performance counting
   */
  static uint32_t perfStartTime;
  static uint8_t perfCounting[RTOS_MAX_TASKS];
  static uint16_t perfStarts[RTOS_MAX_TASKS];
  static uint32_t perfCounters[RTOS_MAX_TASKS];

  static void startCount(int tasknum);
  static void endCount(int tasknum);
  static void resetCounts();

  rtos_task::task_status start() override;
  rtos_task::task_status loop() override;
  //rtos_task::task_status end() override;

};

#endif // PERF_H
