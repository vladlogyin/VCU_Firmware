#ifndef ACCELERATOR_TASK_H
#define ACCELERATOR_TASK_H

#include <vcu/vcu_tasks.h>

#include <tuple>

class accelerator_task : public rtos_task
{
public:
  accelerator_task();

  task_status start() override;

  task_status loop() override;

  /**
   * Coeficients for a 2nd order polynomial
   *
   * polyCoefs[2] * x^2 + polyCoefs[1] * x + polyCoefs[0]
   *
   */
  const float polyCoefs[3]={-0.01, 0.7, 0.301};
  /**
   * Raw APPS value
   */
  float inputValue;
  /**
   * Input calibration range
   *
   * This is done so that the input can be calibrated without recomputing polynomial coefficients
   */
  std::tuple<float, float> inputRange={0.0,1.0};
  float normalizedValue;
  float mappedValue;

  /**
   * Normalize and map input value
   */
  void mapPoly();
};

#endif
