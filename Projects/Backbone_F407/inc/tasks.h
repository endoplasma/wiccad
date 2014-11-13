#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>
#include "scheduler.h"

// Task: Handler
extern SCHEDULER_taskhandler TASK_alive_H;
extern SCHEDULER_taskhandler TASK_shell_H;
extern SCHEDULER_taskhandler TASK_shell_read_H;
extern SCHEDULER_taskhandler TASK_demo1_H;
extern SCHEDULER_taskhandler TASK_demo2_H;
extern SCHEDULER_taskhandler TASK_demo3_H;


// Task: Functions
void TASK_alive();
void TASK_shell();
void TASK_shell_read();
void TASK_demo1();
void TASK_demo2();
void TASK_demo3();




#endif /* TASKS_H_ */
