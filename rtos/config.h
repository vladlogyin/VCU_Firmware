#ifndef RTOS_CONFIG_H
#define RTOS_CONFIG_H




#define RTOS_VERSION "PseudOS v0.0.69"
// Comment this to disable printing timestamps during logging
#define RTOS_TIME_STAMPS

// Max number of "concurrent" tasks
#define RTOS_MAX_TASKS 10

// Comment this to disable performance profiler
#define RTOS_PROFILER
// Comment this to disable task names
#define RTOS_TASK_NAMES
#define RTOS_TASK_NAME_LENGTH 16

// Systime settings
#define SYSTIME_TICKRATE 1000
// Comment this to remove support for timer overflow detection
#define SYSTIME_HEATDEATH

#endif // CONFIG_H_INCLUDED
