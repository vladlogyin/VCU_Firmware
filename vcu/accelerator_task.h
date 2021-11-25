#ifndef ACCELERATOR_TASK_H
#define ACCELERATOR_TASK_H

#include <vcu/vcu_tasks.h>

#include <tuple>


//class

/**
 * Accelerator task
 *
 * This task does APPS signal acquisition and processing
 *
 * TODO implement an analog input method
 * TODO use a timer to invalidate the PWM input after some inactivity (~10ms)
 */

class basic_accelerator_task : public rtos_task
{
public:

  basic_accelerator_task();

  task_status start() override;

  task_status loop() override;

  task_status stop() override;

  /**
   * Coeficients for a 2nd order polynomial
   *
   * polyCoefs[2] * x^2 + polyCoefs[1] * x + polyCoefs[0]
   *
   */
  const float polyCoefs[3] = {-0.01f, 0.7f, 0.301f};
  /**
   * Raw APPS value
   */
  float inputValue;
  /**
   * Input calibration range
   *
   * This is done so that the input can be calibrated without recomputing polynomial coefficients
   */
  std::tuple<float, float> inputRange = {0.0f, 1.0f};
  float normalizedValue;
  float mappedValue;

  /**
   * Input sensor setup function
   *
   * This stub should be implemented in child classes
   *
   * @return false if the sensor's setup failed
   */
  virtual bool setupInput(){return false;}
  /**
   * Input sensor read function
   *
   * This stub should be implemented in child classes
   *
   * @return false if the sensor could not be read
   */
  virtual bool readInput(){return false;}
  /**
   * Input sensor cleanup function
   *
   * This stub should be implemented in child classes
   *
   * @return false if the sensor could not be de-initialized
   */
  virtual bool cleanupInput(){return false;}
  /**
   * Normalize and map input value
   */
  void mapPoly();

};

/**
 * Dummy accelerator task
 */
class dummy_accelerator_task : public basic_accelerator_task
{
  bool setupInput() override {return true;}

  bool readInput() override;

  bool cleanupInput() override {return true;}


};

class pwm_accelerator_task : public basic_accelerator_task
{
public:

  bool setupInput() override;

  bool readInput() override;

  bool cleanupInput() override;

  /**
   * Values set by timer ISR
   *
   * Used for a PWM APPS
   */
  static struct timer_t
  {
    uint16_t period;
    uint16_t duty;
    bool validPeriod;
    bool validDuty;
  } timer;
};

#endif
