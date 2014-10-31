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
#define SCHEDULER_RCC				RCC_APB1Periph_TIM7
#define SCHEDULER_IRQn				TIM7_IRQn
#define SCHEDULER_IRQHandler		TIM7_IRQHandler
#define SCHEDULER_TIM_MAX_Mhz		42						/* Max Interface Clock: Datenblatt S. 31) */


// Frequenz-Einstellungen
#define	SCHEDULER_PERIOD_US		100
#define	SCHEDULER_PRESCALER		((SCHEDULER_TIM_MAX_Mhz*SCHEDULER_PERIOD_US)-1)
#define _ms						*1000/SCHEDULER_PERIOD_US
#define _s						*1000 _ms


// Anzahl Taskslots
#define SCHEDULER_TASK_SLOTS			17



// Status Flags
#define FLAG_active			0
#define FLAG_enabled		1
#define FLAG_repeat			2
#define FLAG_triggered		3
#define FLAG_call_function	4
#define FLAG_empty1	5
#define FLAG_empty2	6
#define FLAG_empty3	7



typedef struct {
	uint8_t task_ID;
	uint8_t task_flags;
	uint32_t task_exetime;
	uint32_t task_interval;
	void (*task_function)(void);
} SCHEDULER_task;





void SCHEDULER_init();
void SCHEDULER_start();
void SCHEDULER_set_task(uint8_t* handler, uint32_t interval, uint8_t repeat, void(*fctn)() );
void SCHEDULER_delete_task(uint8_t* handler);
void SCHEDULER_disable_task(uint8_t* handler);
void SCHEDULER_enable_task(uint8_t* handler);
void SCHEDULER_set_interval(uint8_t* handler, uint32_t interval);



#endif
