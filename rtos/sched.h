#ifndef SCHED_H
#define SCHED_H


#include <rtos/config.h>
#include <rtos/task.h>

class rtos_scheduler
{
public:
  rtos_scheduler();
  int currentTaskID;
  rtos_task* currentTaskPtr=nullptr;
  rtos_task* tasks[RTOS_MAX_TASKS];
  systime_t  sleepEnd[RTOS_MAX_TASKS];
  bool       sleepOverflow[RTOS_MAX_TASKS];

  bool addTask(rtos_task* task);
  bool start();

  void sleepFor(systime_t millis);
};


#endif
