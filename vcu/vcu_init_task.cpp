#include <vcu/vcu_init_task.h>
#include <vcu/accelerator_task.h>

vcu_init_task::vcu_init_task() : rtos_task()
{
  const char task_name[] = "vcu init";
  std::strcpy(name, task_name);
}

rtos_task::task_status vcu_init_task::start()
{
  rtos::log("Adding profiler task");
  parentScheduler->addTask(&prof);
  rtos::log("Adding accelerator task");
  parentScheduler->addTask(&accelerator);
  return rtos_task::RTOS_TASK_LOOP;
}

rtos_task::task_status vcu_init_task::loop()
{
  profiler::startCount(parentScheduler->currentTaskID);
  sleepFor(5000);
  rtos::log("alive!");
  profiler::stopCount(parentScheduler->currentTaskID);
  return rtos_task::RTOS_TASK_SLEEP;
}
