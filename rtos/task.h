#ifndef TASK_H
#define TASK_H


#include <rtos/config.h>
#include <rtos/systime.h>

class rtos_scheduler;

class rtos_task
{
public:

   enum task_status {RTOS_TASK_FAILED=-1, RTOS_TASK_DONE=0, RTOS_TASK_START, RTOS_TASK_LOOP, RTOS_TASK_SLEEP, RTOS_TASK_STOP};

  rtos_task();

#ifdef RTOS_TASK_NAMES
  char name[RTOS_TASK_NAME_LENGTH]="unnamed task";
#endif

  /**
   * Task status
   *
   * The task status hints to the scheduler what it should do.
   * If initialized as RTOS_TASK_START, it tells the scheduler to run start() as soon as the task is added
   */
  task_status status = RTOS_TASK_START;
  int priority = 0;
  /**
   * Sleep counter
   *
   * Number of milliseconds left to sleep. If set to -1, task will sleep until scheduled externally
   */
  uint32_t sleepUntil = -1;

  rtos_scheduler* parentScheduler;

  void sleepFor(systime_t millis);

  virtual task_status start()
  {
    return RTOS_TASK_FAILED;
  }
  virtual task_status loop()
  {
    return RTOS_TASK_FAILED;
  }
  virtual task_status stop()
  {
    return RTOS_TASK_FAILED;
  }

};
#endif // TASK_H
