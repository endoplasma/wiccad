//////////////////////////////////////////////////////////////////////////////////////////////////
//		██╗  ██╗  ██╗      ██╗    ██╗██╗ ██████╗ ██████╗ █████╗ ██████╗       ██╗  ██╗  ██╗		//
//		╚██╗ ╚██╗ ╚██╗     ██║    ██║██║██╔════╝██╔════╝██╔══██╗██╔══██╗     ██╔╝ ██╔╝ ██╔╝		//
//		 ╚██╗ ╚██╗ ╚██╗    ██║ █╗ ██║██║██║     ██║     ███████║██║  ██║    ██╔╝ ██╔╝ ██╔╝		//
//		 ██╔╝ ██╔╝ ██╔╝    ██║███╗██║██║██║     ██║     ██╔══██║██║  ██║    ╚██╗ ╚██╗ ╚██╗		//
//		██╔╝ ██╔╝ ██╔╝     ╚███╔███╔╝██║╚██████╗╚██████╗██║  ██║██████╔╝     ╚██╗ ╚██╗ ╚██╗		//
//		╚═╝  ╚═╝  ╚═╝       ╚══╝╚══╝ ╚═╝ ╚═════╝ ╚═════╝╚═╝  ╚═╝╚═════╝       ╚═╝  ╚═╝  ╚═╝		//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "scheduler.h"
#include "cdc_shell.h"



volatile SCHEDULER_task SCHEDULER_task_list[SCHEDULER_TASK_SLOTS];
volatile uint32_t SCHEDULER_time = 0;
uint8_t shell_output_status = RESET;


TIM_HandleTypeDef    SCHEDULER_Handle;

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
	SCHEDULER_CLK_ENABLE();
	SCHEDULER_Handle.Instance = SCHEDULER_TIM;
	SCHEDULER_Handle.Init.Period = 1;
	SCHEDULER_Handle.Init.Prescaler = SCHEDULER_PRESCALER;
	SCHEDULER_Handle.Init.ClockDivision = 0;
	SCHEDULER_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&SCHEDULER_Handle);

	HAL_NVIC_SetPriority(SCHEDULER_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(SCHEDULER_IRQn);

	uint8_t i;
	for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
	{
		SCHEDULER_task_list[i].task_ID = i;
		SCHEDULER_task_list[i].task_flags = 0;
		SCHEDULER_task_list[i].task_executed = 0;
		SCHEDULER_task_list[i].task_exetime = 0;
		SCHEDULER_task_list[i].task_interval = 0;
		SCHEDULER_task_list[i].task_function = SCHEDULER_foo;
		SCHEDULER_task_list[i].task_name[0] = '\0';
	}



	HAL_TIM_Base_Start_IT(&SCHEDULER_Handle);
}



/**
  * @brief  Interrupthandler des Scheudlers.
  * 		Prüft alle aktiven Tasks auf Auslösung.
  * @param  None
  * @retval None
  */
void SCHEDULER_IRQHandler(void)
{
	if(__HAL_TIM_GET_ITSTATUS(&SCHEDULER_Handle, TIM_IT_UPDATE) !=RESET)
	{
		__HAL_TIM_CLEAR_IT(&SCHEDULER_Handle, TIM_IT_UPDATE);

		SCHEDULER_time++;

		// Prüfue Tasks auf Auslösung
		uint8_t i;
		for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
		{
			if(BIT_IS_SET(SCHEDULER_task_list[i].task_flags, FLAG_enabled))
			{
				if( SCHEDULER_task_list[i].task_exetime > 0)
				{
					SCHEDULER_task_list[i].task_exetime--;
				}
				else
				{
					// Normaler Task -> Flag zum Ausführen setzen
					if(BIT_IS_SET(SCHEDULER_task_list[i].task_flags, FLAG_auto_delete))
					{
						BIT_SET(SCHEDULER_task_list[i].task_flags, FLAG_triggered);
					}

					// StatusTask
					else
					{
						BIT_SET(SCHEDULER_task_list[i].task_flags, FLAG_done);
					}
				}
			}
		}
	}
}



/**
  * @brief  Gibt alle aktuellen Tasks in der shell aus.
  * @param  none
  * @retval None
  */
void SCHEDULER_shell_out()
{
	shell_out(ANSI_BOLD"Scheduler-Informationen"ANSI_BOLD_OFF"\r\n");
	// Alle Slots durchlaufen
	uint8_t i;
	for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
	{
		char ansicolor[10];
		if(BIT_IS_SET(SCHEDULER_task_list[i].task_flags, FLAG_active))
		{
			if(BIT_IS_SET(SCHEDULER_task_list[i].task_flags, FLAG_enabled))
				strcpy(ansicolor,ANSI_FG_GREEN);
			else
				strcpy(ansicolor,ANSI_FG_GRAY);
		}
		else
		{
			strcpy(ansicolor,ANSI_FG_RED);
		}

		char out[150];
		sprintf(out,"%s#%d\t%s\t%d\t"SHELL_BYTE"\t%d\t%d\r\n",
				ansicolor,
				i,
				SCHEDULER_task_list[i].task_name,
				SCHEDULER_task_list[i].task_interval,
				SHELL_BYTE2BIN(SCHEDULER_task_list[i].task_flags),
				SCHEDULER_task_list[i].task_executed,
				SCHEDULER_task_list[i].task_exetime
				);
		shell_out(out);
	}
}



/**
  * @brief  Shell-Befehl: Löscht einen bestehenden Task und gibt den Slot für andere Tasks frei
  * @param  data:	Befehlszeichenkette
  * @retval None
  */
void SCHEDULER_shell_status_shell(int id, char* cmd)
{
	int a;
	sscanf(cmd,"%d",&a);

	if(a == 0)
		shell_output_status = RESET;
	else
		shell_output_status = SET;
}



/**
  * @brief  Erstellt einen neuen Task
  * @param  handler:	Handler (ID) des Tasks
  * @param  name:		Name des Tasks
  * @param  interval:	Ausführungsinterval des Tasks (oder Verzögerung bei einmaliger Ausführung)
  * @param  repeat:		true: Tasks wird regelmäßig wiederholt
  * @param  fctn:	Zeiger auf auszuführene Funktion
  * @retval None
  */
void SCHEDULER_set_task(SCHEDULER_taskhandler* handler, char* name, uint32_t interval, uint8_t repeat, void(*fctn)() )
{
	uint8_t task_ID;

	// Task existiert bereits, wird nun aktualisiert
	if(handler->task_active != RESET)
	{
		task_ID = handler->task_ID;
	}
	// Task wird neu erstellt
	else
	{
		task_ID = 255;
		// Suche ersten freien Slot
		uint8_t i;
		for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
		{
			if(BIT_IS_CLR(SCHEDULER_task_list[i].task_flags, FLAG_active))
			{
				task_ID = i;
				break;
			}
		}
	}

	// Slot gefunden
	if(task_ID != 255)
	{
		// Zuweisung für Handler
		handler->task_ID = task_ID;
		handler->task_active = SET;

		// Einstellungen speichern
		strcpy(SCHEDULER_task_list[task_ID].task_name,name);
		SCHEDULER_task_list[task_ID].task_executed = 0;
		SCHEDULER_task_list[task_ID].task_interval = interval;
		SCHEDULER_task_list[task_ID].task_exetime = interval;

		// Flags setzen/löschen
		BIT_SET(SCHEDULER_task_list[task_ID].task_flags, FLAG_active);
		BIT_SET(SCHEDULER_task_list[task_ID].task_flags, FLAG_enabled);
		BIT_CLR(SCHEDULER_task_list[task_ID].task_flags, FLAG_triggered);
		BIT_SET(SCHEDULER_task_list[task_ID].task_flags, FLAG_auto_delete);
		BIT_CLR(SCHEDULER_task_list[task_ID].task_flags, FLAG_done);

		// Einmaliges Auslösen?
		if(repeat != 0)
		{
			BIT_SET(SCHEDULER_task_list[task_ID].task_flags, FLAG_repeat);
		}
		else
		{
			BIT_CLR(SCHEDULER_task_list[task_ID].task_flags, FLAG_repeat);
		}

		// Automatischer Funktionsaufruf?
		if(fctn == 0)
		{
			SCHEDULER_task_list[task_ID].task_function = SCHEDULER_foo;
			BIT_CLR(SCHEDULER_task_list[task_ID].task_flags, FLAG_call_function);
		}
		else
		{
			SCHEDULER_task_list[task_ID].task_function = fctn;
			BIT_SET(SCHEDULER_task_list[task_ID].task_flags, FLAG_call_function);
		}
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
  * @brief  Erstellt einen neuen StatusTask
  * 		=> Erzeugt lediglich ein gesetztes Flag nach eingestellter Verzögerung.
  * @param  handler:	Handler (ID) des Tasks
  * @param  name:		Name des Tasks
  * @param  interval:	Verzögerung des Tasks
  * @retval None
  */
void SCHEDULER_set_statustask(SCHEDULER_taskhandler* handler, char* name, uint32_t interval)
{
	SCHEDULER_set_task(handler, name, interval, 0, 0);
	BIT_CLR(SCHEDULER_task_list[handler->task_ID].task_flags, FLAG_auto_delete);
}


/**
  * @brief  Prüft einen Task auf Auslösung
  * @param  handler:	Handler (ID) des Tasks
  * @retval None
  */
uint8_t SCHEDULER_get_statustask(SCHEDULER_taskhandler* handler)
{
	if(
			handler->task_active != RESET
			//&& handler->task_ID < SCHEDULER_TASK_SLOTS
			&& BIT_IS_SET(SCHEDULER_task_list[handler->task_ID].task_flags, FLAG_done)
		)
	{
		SCHEDULER_delete_task(handler);
		return SET;
	}
	return RESET;
}


/**
  * @brief  Löscht einen bestehenden Task und gibt den Slot für andere Tasks frei
  * @param  handler:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_delete_task(SCHEDULER_taskhandler* handler)
{
	//if(*handler < SCHEDULER_TASK_SLOTS)
	if(handler->task_active != RESET)
	{
		SCHEDULER_task_list[handler->task_ID].task_interval = 0;
		SCHEDULER_task_list[handler->task_ID].task_name[0] = '\0';
		SCHEDULER_task_list[handler->task_ID].task_flags = 0;
		SCHEDULER_task_list[handler->task_ID].task_function = SCHEDULER_foo;
		SCHEDULER_task_list[handler->task_ID].task_executed = 0;
		SCHEDULER_task_list[handler->task_ID].task_exetime = 0;
		handler->task_ID = 255;
		handler->task_active = RESET;
	}
}


/**
  * @brief  Shell-Befehl: Löscht einen bestehenden Task und gibt den Slot für andere Tasks frei
  * @param  data:	Befehlszeichenkette
  * @retval None
  */
void SCHEDULER_delete_task_shell(int id, char* cmd)
{
	int a;
	sscanf(cmd,"%d",&a);

	SCHEDULER_taskhandler tmp;
	tmp.task_ID = a;
	tmp.task_active = SET;
	SCHEDULER_delete_task(&tmp);
}


/**
  * @brief  Deaktiviert einen bestehenden Task
  * @param  hanlder:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_disable_task(SCHEDULER_taskhandler* handler)
{
	//if(*handler < SCHEDULER_TASK_SLOTS)
	if(handler->task_active != RESET)
	{
		BIT_CLR(SCHEDULER_task_list[handler->task_ID].task_flags, FLAG_enabled);
	}
}


/**
  * @brief  Aktiviert einen bestehenden Task
  * @param  handler:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_enable_task(SCHEDULER_taskhandler* handler)
{
	//if(*handler < SCHEDULER_TASK_SLOTS)
	if(handler->task_active != RESET)
	{
		BIT_SET(SCHEDULER_task_list[handler->task_ID].task_flags, FLAG_enabled);
		BIT_CLR(SCHEDULER_task_list[handler->task_ID].task_flags, FLAG_triggered);
		SCHEDULER_task_list[handler->task_ID].task_exetime = SCHEDULER_task_list[handler->task_ID].task_interval;
	}
}


/**
  * @brief  Ändert den Zustand eines bestehenden Tasks
  * @param  handler:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_task_status(SCHEDULER_taskhandler* handler, uint8_t status)
{
	if(status == RESET)
		SCHEDULER_disable_task(handler);
	else
		SCHEDULER_enable_task(handler);
}


/**
  * @brief  Shell-Befehl: Ändert den Zustand eines bestehenden Tasks
  * @param  data:	Befehlszeichenkette
  * @retval None
  */
void SCHEDULER_task_status_shell(int id, char* cmd)
{
	int a,b;
	sscanf(cmd,"%d,%d",&a,&b);

	SCHEDULER_taskhandler tmp;
	tmp.task_ID = a;
	tmp.task_active = SET;
	SCHEDULER_task_status(&tmp,b);
}


/**
  * @brief  Ändert das Interval eines bestehenden Tasks
  * @param  handler:	Handler (ID) des Tasks
  * @retval None
  */
void SCHEDULER_set_interval(SCHEDULER_taskhandler* handler,uint32_t interval)
{
	//if(*handler < SCHEDULER_TASK_SLOTS)
	if(handler->task_active != RESET)
	{
		SCHEDULER_task_list[handler->task_ID].task_interval = interval;
	}
}


/**
  * @brief  Shell-Befehl: Ändert das Interval eines bestehenden Tasks
  * @param  data:	Befehlszeichenkette
  * @retval None
  */
void SCHEDULER_set_interval_shell(int id, char* cmd)
{
	int a,b;
	sscanf(cmd,"%d,%d",&a,&b);

	SCHEDULER_set_interval(&a,b);
}


/**
  * @brief  Warte angegebene Zeit in Millisekunden.
  * @param  delay:	Wartezeit
  * @retval None
  */
void delay_ms(uint32_t delay)
{
	uint32_t start = SCHEDULER_time;
	uint32_t stop = start + delay _ms;
	while(stop > SCHEDULER_time);
}


/**
  * @brief  Startet den Scheduler
  * @param  None
  * @retval None
  */
void SCHEDULER_main()
{
	// Falls die shell aktiv ist, erstelle entsprechende Kommandos
	if(shell_initiated != RESET)
	{
		shell_register_cmd("scheduler_interval:",SCHEDULER_set_interval_shell);
		shell_register_cmd("scheduler_delete:",SCHEDULER_delete_task_shell);
		shell_register_cmd("scheduler_task:",SCHEDULER_task_status_shell);
		shell_register_out("scheduler",SET,SCHEDULER_shell_out);
	}


	while(1) // The one and only ...
	{
		uint8_t i;
		for(i=0;i<SCHEDULER_TASK_SLOTS;i++)
		{
			// Task ausführen
			if(
					BIT_IS_SET(SCHEDULER_task_list[i].task_flags, FLAG_enabled)
					&& BIT_IS_SET(SCHEDULER_task_list[i].task_flags, FLAG_triggered)
					)
			{
				BIT_CLR(SCHEDULER_task_list[i].task_flags, FLAG_triggered);

				SCHEDULER_task_list[i].task_function();
				SCHEDULER_task_list[i].task_executed++;

				// Task erneut ausführen...
				if(BIT_IS_SET(SCHEDULER_task_list[i].task_flags, FLAG_repeat))
				{
					SCHEDULER_task_list[i].task_exetime = SCHEDULER_task_list[i].task_interval;
				}
				// Task löschen
				else if(BIT_IS_SET(SCHEDULER_task_list[i].task_flags, FLAG_auto_delete))
				{
					//SCHEDULER_delete_task(&i);
				}
			}
		}
	}
}
