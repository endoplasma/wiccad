//////////////////////////////////////////////////////////////////////////////////////////////////
//		██╗  ██╗  ██╗      ██╗    ██╗██╗ ██████╗ ██████╗ █████╗ ██████╗       ██╗  ██╗  ██╗		//
//		╚██╗ ╚██╗ ╚██╗     ██║    ██║██║██╔════╝██╔════╝██╔══██╗██╔══██╗     ██╔╝ ██╔╝ ██╔╝		//
//		 ╚██╗ ╚██╗ ╚██╗    ██║ █╗ ██║██║██║     ██║     ███████║██║  ██║    ██╔╝ ██╔╝ ██╔╝		//
//		 ██╔╝ ██╔╝ ██╔╝    ██║███╗██║██║██║     ██║     ██╔══██║██║  ██║    ╚██╗ ╚██╗ ╚██╗		//
//		██╔╝ ██╔╝ ██╔╝     ╚███╔███╔╝██║╚██████╗╚██████╗██║  ██║██████╔╝     ╚██╗ ╚██╗ ╚██╗		//
//		╚═╝  ╚═╝  ╚═╝       ╚══╝╚══╝ ╚═╝ ╚═════╝ ╚═════╝╚═╝  ╚═╝╚═════╝       ╚═╝  ╚═╝  ╚═╝		//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "main.h"
#include "tasks.h"
#include "scheduler.h"



// Taskhandler
uint8_t TASK_alive_H;
uint8_t TASK_demo1_H;
uint8_t TASK_demo2_H;
uint8_t TASK_demo3_H;




void LED_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = LED_blue | LED_green | LED_orange | LED_red;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	GPIO_WriteBit(LED_PORT, LED_blue, SET);
	GPIO_WriteBit(LED_PORT, LED_green, SET);
	GPIO_WriteBit(LED_PORT, LED_orange, SET);
	GPIO_WriteBit(LED_PORT, LED_red, SET);
}



void TASK_alive()
{
	LED_PORT->ODR ^= LED_green;
}
void TASK_demo1()
{
	LED_PORT->ODR ^= LED_red;
}
void TASK_demo2()
{
	LED_PORT->ODR ^= LED_blue;
}
void TASK_demo3()
{
	LED_PORT->ODR ^= LED_orange;
}



int main(void)
{
	SystemInit();

	// Initialize hardware
	LED_init();

	// Initialize scheduler
	SCHEDULER_init();

	// Initialize tasks
	SCHEDULER_set_task(&TASK_alive_H, 50 _ms,1,TASK_alive);
	SCHEDULER_set_task(&TASK_demo1_H, 666 _ms,1,TASK_demo1);
	SCHEDULER_set_task(&TASK_demo2_H, 222 _ms,1,TASK_demo2);
	SCHEDULER_set_task(&TASK_demo3_H, 777 _ms,1,TASK_demo3);

	// Starting scheduler...
	SCHEDULER_start();
}

