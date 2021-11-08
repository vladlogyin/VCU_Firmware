#ifndef SYSTIME_H
#define SYSTIME_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/systick.h>

#include <rtos/config.h>

#include <cstddef>


/**
 * systime runs at 1000Hz
 */

typedef uint32_t systime_t;

class systime
{
public:
  ~systime() = delete;
  static void systickSetup(void);
  /**
   * Starts the Systick timer
   */
  static void systickEnable(void);

  /**
   * Stops the Systick timer
   */
  static void systickDisable(void);

  /**
   * Resets the overflow counter and the millisecond counter
   */
  static void systickReset(void);

  /**
   * Get milliseconds since the last reset
   *
   * @returns milliseconds
   */
  static systime_t getMillis(void);

  /**
   * Blocking millisecond delay
   *
   * @param ms milliseconds
   */
  static void delayMs(systime_t ms);

  /**
   * Gets called every Systick
   */
  static void doSystick(void);

  /**
   * Sysmillis overflow callback
   *
   * This is called every time Sysmillis overflows which is once every 49.7 days
   */
  static bool (*overflowHandler)();

#ifdef SYSTIME_HEATDEATH
  /**
   * Overflow counter overflow callback
   *
   * This is called every 2^64 milliseconds. Chances are everyone will be dead by then, but doesn't hurt to be careful
   */
  static bool (*heatDeathHandler)();
#endif

private:

  static systime_t _sysoverflow;
  static systime_t _sysmillis;
};



#endif
