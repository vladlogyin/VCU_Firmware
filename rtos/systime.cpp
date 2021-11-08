#include <rtos/systime.h>

systime_t systime::_sysmillis;
systime_t systime::_sysoverflow;

bool (*systime::overflowHandler)(){nullptr};
bool (*systime::heatDeathHandler)(){nullptr};

void systime::systickEnable()
{
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
  systick_set_reload(rcc_ahb_frequency/8000 - 1);
  //systick_set_frequency(SYSTIME_TICKRATE, rcc_ahb_frequency);
  systick_interrupt_enable();
  nvic_enable_irq(NVIC_SYSTICK_IRQ);
  systick_counter_enable();
}
void systime::systickDisable()
{
  systick_counter_disable();
  nvic_disable_irq(NVIC_SYSTICK_IRQ);
  systick_interrupt_disable();
}

void systime::systickReset()
{
  _sysoverflow=0;
  _sysmillis=0;
}

systime_t systime::getMillis()
{
  return _sysmillis;
}

void systime::delayMs(systime_t ms)
{
  systime_t targetmillis=_sysmillis+ms;
  systime_t targetoverflow=_sysoverflow;
  // Check for a possible overflow
  if(targetmillis<_sysmillis)
  {
    targetoverflow++;
  }
  while((_sysmillis<targetmillis)||(targetoverflow!=_sysoverflow))
  {__asm("nop");}
}

void systime::doSystick()
{
  systime_t new_sysmillis = _sysmillis + 1;

  // Check if the system clock overflowed
  if(new_sysmillis < _sysmillis)
  {
#ifdef SYSTIME_HEATDEATH
    systime_t new_sysoverflow = _sysoverflow + 1;

    // Check if we've done the impossible
    if(new_sysoverflow < _sysoverflow)
    {
      // Unless the heat death handler is implemented, just break the timer
      if(systime::heatDeathHandler)
      {
        systime::heatDeathHandler();
        _sysoverflow = new_sysoverflow;
      }
      // Don't even bother calling the overflow handler at this point
      return;
    }
    _sysoverflow=new_sysoverflow;

#else
    _sysoverflow++;
#endif

    if(systime::overflowHandler)
    {
      systime::overflowHandler();
    }
  }
  _sysmillis = new_sysmillis;
}

void sys_tick_handler()
{
  systime::doSystick();
}
