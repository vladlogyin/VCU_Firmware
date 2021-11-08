#include <rtos/task.h>
#include <rtos/sched.h>

rtos_task::rtos_task()
{

}
void rtos_task::sleepFor(systime_t millis)
{
  parentScheduler->sleepFor(millis);
}
