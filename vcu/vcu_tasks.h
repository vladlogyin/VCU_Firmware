#ifndef VCU_TASKS_H
#define VCU_TASKS_H

#include <rtos/rtos.h>
#include <rtos/perf.h>

/**
 * VCU init task
 *
 * This task manages all other VCU related tasks
 */
class vcu_init_task : public rtos_task
{
public:
  profiler prof;
  vcu_init_task();

  task_status start() override;

  task_status loop() override;
};
#endif
