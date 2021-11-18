#include <rtos/perf.h>
#include <vcu/accelerator_task.h>
#include <algorithm>

accelerator_task::accelerator_task() : rtos_task()
{
  const char task_name[] = "accelerator";
  std::strcpy(name, task_name);
}

rtos_task::task_status accelerator_task::start()
{

  return rtos_task::RTOS_TASK_LOOP;
}

// Dummy FPU benchmark
rtos_task::task_status accelerator_task::loop()
{
  profiler::startCount(parentScheduler->currentTaskID);
  sleepFor(1000);
  for(int i=0;i<1000000;i++)
  {
    this->mapPoly();
  }
  profiler::stopCount(parentScheduler->currentTaskID);
  return rtos_task::RTOS_TASK_SLEEP;
}


void accelerator_task::mapPoly()
{
  // TODO maybe move this into a member variable since it doesn't need to be reevaluated unless the range changes
  float range = std::get<1>(this->inputRange) - std::get<0>(this->inputRange);
  // Normallize input value and clamp it to make sure it's between 0 and 1
  // If the normalized value isn't between 0 and 1 spooky stuff might happen
  this->normalizedValue = std::clamp<float>((this->inputValue - std::get<0>(this->inputRange)) / range, 0.0, 1.0);
  // Start with x^0
  float xn = 1.0;
  this->mappedValue = 0.0;

  for(int i = 0; i < 3; i++)
  {
    // sum i-th power of rawValue times its coefficient
    this->mappedValue += xn * polyCoefs[i];
    // Move onto next exponent
    xn *= this->normalizedValue;
  }
}
