#include <rtos/rtos.h>

rtos_scheduler rtos::scheduler;
ostream rtos::stdout(nullptr, nullptr);

rtos_task::task_status rtos::bringup(rtos_task* t)
{
  // Init serial so we can have logs
  // Register stream callbacks
  stdout._stringcb = &rtos::usart1_ostring;
  stdout._charcb = &rtos::usart1_ochar;
  
  // Test serial by printing out version
  stdout<<"[BRINGUP]"<<RTOS_VERSION<<"\n\r";
  // Initialize systick timer
  //systime::systickSetup();
  systime::systickEnable();
  rtos::log("Systick initialized");
  
  // Add init task
  scheduler.addTask(t);
#ifdef RTOS_TASK_NAMES
  rtos::log("Added init task: ",false)<<t->name<<"\n\r";
#else
  rtos::log("Added init task");
#endif

  // Start scheduler
  if(!scheduler.start())
  {
    // If we reach this point, something has gone wrong with the scheduler
    rtos::err("Failed to start scheduler");
    return rtos_task::RTOS_TASK_FAILED;
  }
  return rtos_task::RTOS_TASK_DONE;
}

ostream& rtos::log(const char* str, bool nr)
{
  // Print log in dmesg-esque format
  const oformat timeformat={.lz=true ,.width=3,.base=10};
#ifdef RTOS_TIME_STAMPS
  stdout<<timeformat<<"\033[32m["<<systime::getMillis()/1000<<"."<<systime::getMillis()%1000<<"]";
#endif
#ifdef RTOS_TASK_NAMES
  if(scheduler.currentTaskPtr!=nullptr)
  {
    stdout<<"\033[33m"<<scheduler.currentTaskPtr->name<<": ";
  }
  else
  {
    stdout<<"\033[33m"<<"rtos: ";
  }
#endif
  stdout<<"\033[0m"<<str;
  if(nr)
  {
    stdout<<"\n\r";
  }

  return stdout;
}

ostream& rtos::err(const char* str, bool nr)
{
  // TODO maybe disable blinking
  stdout<<"\033[5;1;31m[ERROR]\033[0m"<<str;

  if(nr)
  {
    stdout<<"\n\r";
  }

  return stdout;
}

size_t rtos::usart1_ostring(const char* str, size_t n)
{
  // TODO change "while" to "for" in order to only send n characters
  size_t i = 0;

  while(str[i] != '\0')
  {
    usart_send_blocking(USART1, str[i++]);
  }

  return i;
}

void rtos::usart1_ochar(const char c)
{
  usart_send_blocking(USART1, c);
}
