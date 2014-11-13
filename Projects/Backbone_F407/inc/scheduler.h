//////////////////////////////////////////////////////////////////////////////////////////////////
//		██╗  ██╗  ██╗      ██╗    ██╗██╗ ██████╗ ██████╗ █████╗ ██████╗       ██╗  ██╗  ██╗		//
//		╚██╗ ╚██╗ ╚██╗     ██║    ██║██║██╔════╝██╔════╝██╔══██╗██╔══██╗     ██╔╝ ██╔╝ ██╔╝		//
//		 ╚██╗ ╚██╗ ╚██╗    ██║ █╗ ██║██║██║     ██║     ███████║██║  ██║    ██╔╝ ██╔╝ ██╔╝		//
//		 ██╔╝ ██╔╝ ██╔╝    ██║███╗██║██║██║     ██║     ██╔══██║██║  ██║    ╚██╗ ╚██╗ ╚██╗		//
//		██╔╝ ██╔╝ ██╔╝     ╚███╔███╔╝██║╚██████╗╚██████╗██║  ██║██████╔╝     ╚██╗ ╚██╗ ╚██╗		//
//		╚═╝  ╚═╝  ╚═╝       ╚══╝╚══╝ ╚═╝ ╚═════╝ ╚═════╝╚═╝  ╚═╝╚═════╝       ╚═╝  ╚═╝  ╚═╝		//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "main.h"



// Timer-Einstellung
#define SCHEDULER_TIM				TIM7
#define SCHEDULER_CLK_ENABLE		__TIM7_CLK_ENABLE
#define SCHEDULER_IRQn				TIM7_IRQn
#define SCHEDULER_IRQHandler		TIM7_IRQHandler
#define SCHEDULER_TIM_MAX_Mhz		42						/* Max Interface Clock: Datenblatt S. 31) */



// Frequenz-Einstellungen
#define	SCHEDULER_PERIOD_US		100
#define	SCHEDULER_PRESCALER		((SCHEDULER_TIM_MAX_Mhz*SCHEDULER_PERIOD_US)-1)
#define _ms						*1000/SCHEDULER_PERIOD_US
#define _s						*1000 _ms


// Anzahl Taskslots
#define SCHEDULER_TASK_SLOTS			10



// Status Flags
#define FLAG_active			0x01
#define FLAG_enabled		0x02
#define FLAG_repeat			0x04
#define FLAG_triggered		0x08
#define FLAG_call_function	0x10
#define FLAG_auto_delete	0x20
#define FLAG_done			0x40
#define FLAG_nc				0x80	/* noch frei */



typedef struct {
	uint8_t task_ID;
	uint8_t task_flags;
	uint32_t task_exetime;
	uint32_t task_interval;
	uint32_t task_executed;
	char task_name[42];
	void (*task_function)(void);
} SCHEDULER_task;

typedef struct {
	uint8_t task_ID;
	uint8_t task_active;
} SCHEDULER_taskhandler;

extern volatile uint32_t SCHEDULER_time;


void delay_ms(uint32_t delay);
void SCHEDULER_init();
void SCHEDULER_main();
void SCHEDULER_set_task(SCHEDULER_taskhandler* handler, char* name, uint32_t interval, uint8_t repeat, void(*fctn)() );
void SCHEDULER_set_statustask(SCHEDULER_taskhandler* handler, char* name, uint32_t interval);
uint8_t SCHEDULER_get_statustask(SCHEDULER_taskhandler* handler);
void SCHEDULER_delete_task(SCHEDULER_taskhandler* handler);
void SCHEDULER_delete_task_shell(int id, char* cmd);
void SCHEDULER_disable_task(SCHEDULER_taskhandler* handler);
void SCHEDULER_enable_task(SCHEDULER_taskhandler* handler);
void SCHEDULER_task_status(SCHEDULER_taskhandler* handler, uint8_t status);
void SCHEDULER_task_status_shell(int id, char* cmd);
void SCHEDULER_set_interval(SCHEDULER_taskhandler* handler, uint32_t interval);
void SCHEDULER_set_interval_shell(int id, char* cmd);
void SCHEDULER_shell_status_shell(int id, char* cmd);
void SCHEDULER_shell_out();


#endif
