#ifndef RTOS_H
#define RTOS_H

#include <cstdarg>

#include <tinyprintf/tinyprintf.h>

#include <rtos/config.h>
#include <rtos/sched.h>
#include <rtos/systime.h>
#include <rtos/task.h>
#include <rtos/io.h>


class rtos
{
public:
  /// Starts the init task
static  rtos_task::task_status bringup(rtos_task* init);

static ostream& log(const char* str, bool nr=true);
static ostream& err(const char* str, bool nr=true);


static rtos_scheduler scheduler;
// streams
static ostream stdout;
static size_t usart1_ostring(const char* s, size_t n);
static void usart1_ochar(const char c);


};



#endif
