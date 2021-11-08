#include <rtos/sched.h>
#include <rtos/rtos.h>

rtos_scheduler::rtos_scheduler()
{
  for(unsigned int i = 0; i < RTOS_MAX_TASKS; i++)
  {
    tasks[i] = nullptr;
    sleepEnd[i] = 0;
    sleepOverflow[i]=false;
  }

}

bool rtos_scheduler::addTask(rtos_task* task)
{
  // TODO replace this stub to actually look for empty task slot
  for(int i=0;i<RTOS_MAX_TASKS;i++)
  {
    if(tasks[i]==nullptr)
    {
       tasks[i]=task;
       tasks[i]->parentScheduler=this;
       return true;
    }
  }
  return false;
}

void rtos_scheduler::sleepFor(systime_t millis)
{
  // TODO maybe check is currentTaskID is valid
  systime_t now = systime::getMillis();
  sleepEnd[currentTaskID]=now+millis;
  sleepOverflow[currentTaskID]=sleepEnd[currentTaskID]<now;

}

bool rtos_scheduler::start()
{
  // TODO replace this stub
  // Set this to true to find any tasks
  bool anyTasksLeft=true;
  while(anyTasksLeft)
  {
    anyTasksLeft=false;
    for(unsigned int i = 0; i < RTOS_MAX_TASKS; i++)
    {
      currentTaskPtr = tasks[i];
      if(currentTaskPtr==nullptr)
      {
        currentTaskID=-1;
        continue;
      }
      currentTaskID=i;
      anyTasksLeft=true;
      bool sleeping;
      switch(currentTaskPtr->status)
      {
        case rtos_task::RTOS_TASK_START:
          currentTaskPtr->status=currentTaskPtr->start();
          break;
        case rtos_task::RTOS_TASK_LOOP:
          currentTaskPtr->status=currentTaskPtr->loop();
          break;
        case rtos_task::RTOS_TASK_STOP:
          currentTaskPtr->status=currentTaskPtr->stop();
          break;
        case rtos_task::RTOS_TASK_SLEEP:
          sleeping = systime::getMillis()<=sleepEnd[currentTaskID];
          
          if(!sleepOverflow[currentTaskID] & !sleeping)
          {
            currentTaskPtr->status=currentTaskPtr->loop();
            break;
          }
          sleepOverflow[currentTaskID] &= !sleeping;
          break;
        default:
          break;
      }
    }
  }

  return false;
}
