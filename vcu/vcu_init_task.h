#ifndef VCU_INIT_TASK_H
#define VCU_INIT_TASK_H
#include <vcu/vcu_tasks.h>
#include <vcu/accelerator_task.h>
#include <vcu/can_firewall_task.h>
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
  accelerator_task accelerator;
  can_firewall_task firewall;

  vcu_init_task();

  task_status start() override;

  task_status loop() override;
};
#endif
