//////////////////////////////////////////////////////////////////////////////////////////////////
//		██╗  ██╗  ██╗      ██╗    ██╗██╗ ██████╗ ██████╗ █████╗ ██████╗       ██╗  ██╗  ██╗		//
//		╚██╗ ╚██╗ ╚██╗     ██║    ██║██║██╔════╝██╔════╝██╔══██╗██╔══██╗     ██╔╝ ██╔╝ ██╔╝		//
//		 ╚██╗ ╚██╗ ╚██╗    ██║ █╗ ██║██║██║     ██║     ███████║██║  ██║    ██╔╝ ██╔╝ ██╔╝		//
//		 ██╔╝ ██╔╝ ██╔╝    ██║███╗██║██║██║     ██║     ██╔══██║██║  ██║    ╚██╗ ╚██╗ ╚██╗		//
//		██╔╝ ██╔╝ ██╔╝     ╚███╔███╔╝██║╚██████╗╚██████╗██║  ██║██████╔╝     ╚██╗ ╚██╗ ╚██╗		//
//		╚═╝  ╚═╝  ╚═╝       ╚══╝╚══╝ ╚═╝ ╚═════╝ ╚═════╝╚═╝  ╚═╝╚═════╝       ╚═╝  ╚═╝  ╚═╝		//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "scheduler.h"




volatile SCHEDULER_task SCHEDULER_task_list[SCHEDULER_TASK_SLOTS];
volatile uint32_t SCHEDULER_time = 0;
volatile uint8_t SCHEDULER_triggered = 0;



/**
  * @brief  Dummy-Funktion
  * @param  None
  * @retval None
  */
void SCHEDULER_foo(){}



/**
  * @brief  Initialisiert den Scheduler.
  * @param  None
  * @retval None
  */
void SCHEDULER_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(SCHEDULER_RCC, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 1;
	TIM_TimeBaseStructure.TIM_Prescaler = SCHEDULER_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(SCHEDULER_TIM, &TIM_TimeBaseStructure);
	TIM_ITConfig(SCHEDULER_TIM, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = SCHEDULER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	uint8_t i;
	for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
	{
		SCHEDULER_task_list[i].task_ID = i;
		SCHEDULER_task_list[i].task_flags = 0;
	}
}





/**
  * @brief  Interrupthandler des Scheudlers.
  * 		Prüft alle aktiven Tasks auf Auslösung.
  * @param  None
  * @retval None
  */
void SCHEDULER_IRQHandler(void)
{
	if((SCHEDULER_TIM->SR&TIM_IT_Update) != RESET)
	{
		SCHEDULER_TIM->SR = ~TIM_IT_Update;
		SCHEDULER_time++;

		// Prüfue Tasks auf Auslösung
		uint8_t i;
		for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
		{
			if(
					((SCHEDULER_task_list[i].task_flags>>FLAG_enabled)&1) != 0
					&& SCHEDULER_task_list[i].task_exetime == SCHEDULER_time)
			{
				SCHEDULER_task_list[i].task_flags |= (1<<FLAG_triggered);
				SCHEDULER_triggered = 1;
			}
		}
	}
}



/**
  * @brief  Erstellt einen neuen Task
  * @param  hanlder:	Handler (ID) des Tasks
  * @param  interval:	Ausführungsinterval des Tasks (oder Verzögerung bei einmaliger Ausführung)
  * @param  repeat:		true: Tasks wird regelmäßig wiederholt
  * @param  fctn:	Zeiger auf auszuführene Funktion
  * @retval None
  */
void SCHEDULER_set_task(uint8_t* handler, uint32_t interval, uint8_t repeat, void(*fctn)() )
{
	uint8_t task_ID = 255;
	// Suche ersten freien Slot
	uint8_t i;
	for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
	{
		if(((SCHEDULER_task_list[i].task_flags>>FLAG_active)&1) == RESET)
		{
			task_ID = i;
			break;
		}
	}

	// Freien Slot gefunden
	if(task_ID != 255)
	{
		// Zuweisung für Handler
		*handler = task_ID;

		SCHEDULER_task_list[task_ID].task_flags |= (1<<FLAG_active);
		SCHEDULER_task_list[task_ID].task_flags |= (1<<FLAG_enabled);
		SCHEDULER_task_list[task_ID].task_flags &= ~(1<<FLAG_triggered);

		// Einmaliges Auslösen?
		if(repeat != 0)
			SCHEDULER_task_list[task_ID].task_flags |= (1<<FLAG_repeat);
		else
			SCHEDULER_task_list[task_ID].task_flags &= ~(1<<FLAG_repeat);

		// Automatischer Funktionsaufruf?
		if(fctn == 0)
		{
			SCHEDULER_task_list[task_ID].task_function = SCHEDULER_foo;
			SCHEDULER_task_list[task_ID].task_flags &= ~(1<<FLAG_call_function);
		}
		else
		{
			SCHEDULER_task_list[task_ID].task_function = fctn;
			SCHEDULER_task_list[task_ID].task_flags |= (1<<FLAG_call_function);
		}

		SCHEDULER_task_list[task_ID].task_interval = interval;
		SCHEDULER_task_list[task_ID].task_exetime = SCHEDULER_time + interval;
	}
	else
	{
		while(1)
		{
			// Give me more slots please :'(
			// And money!
			// Dont forget the money :)
		}
	}
}


/**
  * @brief  Löscht einen bestehenden Task und gibt den Slot für andere Tasks frei
  * @param  hanlder:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_delete_task(uint8_t* handler)
{
	if(*handler < SCHEDULER_TASK_SLOTS)
	{
		SCHEDULER_task_list[*handler].task_flags &= ~(1<<FLAG_active);
		SCHEDULER_task_list[*handler].task_flags &= ~(1<<FLAG_enabled);
		*handler = 255;
	}
}


/**
  * @brief  Deaktiviert einen bestehenden Task
  * @param  hanlder:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_disable_task(uint8_t* handler)
{
	if(*handler < SCHEDULER_TASK_SLOTS)
	{
		SCHEDULER_task_list[*handler].task_flags &= ~(1<<FLAG_enabled);
	}
}


/**
  * @brief  Aktiviert einen bestehenden Task
  * @param  hanlder:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_enable_task(uint8_t* handler)
{
	if(*handler < SCHEDULER_TASK_SLOTS)
	{
		SCHEDULER_task_list[*handler].task_flags |= (1<<FLAG_enabled);
		SCHEDULER_task_list[*handler].task_flags &= ~(1<<FLAG_triggered);
		SCHEDULER_task_list[*handler].task_exetime = SCHEDULER_time + SCHEDULER_task_list[*handler].task_interval;
	}
}


/**
  * @brief  Ändert das Interval eines bestehenden Tasks
  * @param  hanlder:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_set_interval(uint8_t* handler,uint32_t interval)
{
	if(*handler < SCHEDULER_TASK_SLOTS)
	{
		SCHEDULER_task_list[*handler].task_interval = interval;
	}
}


/**
  * @brief  Startet den Scheduler
  * @param  None
  * @retval None
  */
void SCHEDULER_start()
{
	TIM_Cmd(SCHEDULER_TIM, ENABLE);
	while(1) // The one and only ...
	{
		if(SCHEDULER_triggered != 0)
		{
			SCHEDULER_triggered = 0;

			uint8_t i;
			for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
			{
				if(
						((SCHEDULER_task_list[i].task_flags>>FLAG_enabled)&1) != RESET
						&&((SCHEDULER_task_list[i].task_flags>>FLAG_triggered)&1) != RESET
						)
				{
					SCHEDULER_task_list[i].task_flags &= ~(1<<FLAG_triggered);
					SCHEDULER_task_list[i].task_function();

					if(((SCHEDULER_task_list[i].task_flags>>FLAG_repeat)&1) != RESET)
					{
						SCHEDULER_task_list[i].task_exetime = SCHEDULER_time + SCHEDULER_task_list[i].task_interval;
					}
					else
					{
						SCHEDULER_task_list[i].task_flags = RESET;
					}
				}
			}
		}
	}
}
