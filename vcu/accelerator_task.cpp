#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>

#include <rtos/perf.h>
#include <vcu/accelerator_task.h>

#include <algorithm>
#include <cmath>



pwm_accelerator_task::timer_t pwm_accelerator_task::timer;

bool pwm_accelerator_task::setupInput()
{

  this->timer.duty = 0;
  this->timer.period = 0xFFFF;
  this->timer.validPeriod = false;
  this->timer.validDuty = false;

  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_TIM5);
  //rcc_periph_reset_pulse(RCC_TIM5);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO0);
  gpio_set_af(GPIOA, GPIO_AF2, GPIO0);
  timer_set_mode(TIM5, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_period(TIM5, 0xFFFF);
  // Set timer clock to 1MHz which means the max period we can measure is 65ms
  // This limits the min PWM frequency we can measure to 16 Hz
  timer_set_prescaler(TIM5, 2 * rcc_apb1_frequency / 1000000 - 1);
  timer_enable_counter(TIM5);

  // Configure input compare 1 and 2 to have the same input
  timer_ic_set_input(TIM5, TIM_IC1, TIM_IC_IN_TI1);
  timer_ic_set_input(TIM5, TIM_IC2, TIM_IC_IN_TI1);
  // Start counting the period on the falling edge of the input and the duty on the rising edge
  timer_ic_set_polarity(TIM5, TIM_IC1, TIM_IC_RISING);
  timer_ic_set_polarity(TIM5, TIM_IC2, TIM_IC_FALLING);

  timer_slave_set_trigger(TIM5, TIM_SMCR_TS_TI1FP1);

  timer_slave_set_mode(TIM5, TIM_SMCR_SMS_RM);

  //nvic_set_priority(NVIC_TIM3_IRQ, 7);
  nvic_enable_irq(NVIC_TIM5_IRQ);

  timer_enable_irq(TIM5, TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_UIE);

  timer_ic_enable(TIM5, TIM_IC1);
  timer_ic_enable(TIM5, TIM_IC2);
  return true;
}

bool pwm_accelerator_task::readInput()
{
  this->inputValue = this->timer.duty / (float)this->timer.period;
  return true;
}

bool pwm_accelerator_task::cleanupInput()
{
  //TODO actually implement some cleanup code
  return false;
}


bool dummy_accelerator_task::readInput()
{
  this->inputValue = (systime::getMillis() % 2000) / float(2000.0f);
  return true;
}


basic_accelerator_task::basic_accelerator_task() : rtos_task()
{
  const char task_name[] = "accelerator";
  std::strcpy(this->name, task_name);
}



rtos_task::task_status basic_accelerator_task::start()
{
  return this->setupInput() ? rtos_task::RTOS_TASK_LOOP : rtos_task::RTOS_TASK_FAILED;
}

rtos_task::task_status basic_accelerator_task::loop()
{
  profiler::startCount(parentScheduler->currentTaskID);

  // Set this task to run again after 4ms -> 250Hz
  sleepFor(4);

  // TODO use the return value of readInput to invalidate the APPS value
  if(this->readInput())
  {
    this->mapPoly();
    rtos::log("percentage: ", false) << (long)(this->inputValue * 100) << "%\n\r";


  }

  profiler::stopCount(parentScheduler->currentTaskID);
  return rtos_task::RTOS_TASK_SLEEP;
}

rtos_task::task_status basic_accelerator_task::stop()
{
  return this->cleanupInput() ? rtos_task::RTOS_TASK_DONE : rtos_task::RTOS_TASK_FAILED;
}


void basic_accelerator_task::mapPoly()
{
  // TODO maybe move this into a member variable since it doesn't need to be reevaluated unless the range changes
  float range = std::get<1>(this->inputRange) - std::get<0>(this->inputRange);
  // Normallize input value and clamp it to make sure it's between 0 and 1
  // If the normalized value isn't between 0 and 1 spooky stuff might happen
  this->normalizedValue = std::clamp<float>((this->inputValue - std::get<0>(this->inputRange)) / range, 0.0f, 1.0f);
  // Start with x^0
  float xn = 1.0f;
  this->mappedValue = 0.0f;

  for(float coef : polyCoefs)
  {
    // sum i-th power of rawValue times its coefficient
    this->mappedValue += xn * coef;
    // Move onto next exponent
    xn *= this->normalizedValue;
  }
}

void tim5_isr()
{
  if(TIM5_SR & TIM_SR_CC1IF)
  {
    timer_clear_flag(TIM5, TIM_SR_CC1IF);
    pwm_accelerator_task::timer.period = TIM5_CCR1;
    pwm_accelerator_task::timer.validPeriod = true;
  }

  if(TIM5_SR & TIM_SR_CC2IF)
  {
    timer_clear_flag(TIM5, TIM_SR_CC2IF);
    pwm_accelerator_task::timer.duty = TIM5_CCR2;
    pwm_accelerator_task::timer.validDuty = true;
  }

  if(TIM5_SR & TIM_SR_UIF)
  {
    timer_clear_flag(TIM5, TIM_SR_UIF);
  }
}
