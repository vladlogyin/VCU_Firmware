#include <rtos/perf.h>
#include <stdlib.h>

uint32_t profiler::perfStartTime;
uint8_t profiler::perfCounting[RTOS_MAX_TASKS];
uint16_t profiler::perfStarts[RTOS_MAX_TASKS];
uint32_t profiler::perfCounters[RTOS_MAX_TASKS];

profiler::profiler()
{
  char new_name[]="profiler";
  std::strcpy(name, new_name);
  resetCounts();
}

rtos_task::task_status profiler::start()
{
  rcc_periph_clock_enable(RTOS_PROFILER_TIMER_RCC);
  // Set prescaler to 10us
  timer_set_prescaler(RTOS_PROFILER_TIMER, rcc_apb1_frequency/100000 - 1);
  // Set timer period to (2^16 - 1)
  timer_set_period(RTOS_PROFILER_TIMER, 0xFFFF);
  timer_continuous_mode(RTOS_PROFILER_TIMER);
  timer_enable_counter(RTOS_PROFILER_TIMER);

  return rtos_task::RTOS_TASK_LOOP;
}

rtos_task::task_status profiler::loop()
{
    updateStats=false;
    uint32_t now = systime::getMillis();
    rtos::log("ran for ", false)<<now-perfStartTime<<" ms:\n\r";
    for(int i=0;i<RTOS_MAX_TASKS; i++)
    {
      if(perfCounters[i]>0)
      {
        rtos::log("task \"", false)<<parentScheduler->tasks[i]->name<<"\" ran for "<<perfCounters[i]*10<<" us\n\r";
      }
    }
    resetCounts();
  sleepFor(10000);
  return rtos_task::RTOS_TASK_SLEEP;
}

void profiler::startCount(int tasknum)
{
  // TODO Maybe check for possible overflow, no idea how to handle them though
  if(!(perfCounting[tasknum]++))
  {
    perfStarts[tasknum]=timer_get_counter(RTOS_PROFILER_TIMER);
  }
}

void profiler::endCount(int tasknum)
{
  // TODO Same as above: check for a possible underflow
  if(!perfCounting[tasknum])
  {
    return;
  }

  if(!(--perfCounting[tasknum]))
  {
    uint16_t now = timer_get_counter(RTOS_PROFILER_TIMER);
    perfCounters[tasknum]+=(uint16_t)(now-perfStarts[tasknum]);
  }
}

void profiler::resetCounts()
{
  perfStartTime= systime::getMillis();
  for(int i=0; i<RTOS_MAX_TASKS; i++)
  {
    perfCounting[i]=0;
    perfStarts[i]=0;
    perfCounters[i]=0;
  }
}
