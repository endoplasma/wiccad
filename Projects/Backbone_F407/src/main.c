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
#include "cdc_shell.h"
#include "adc.h"
#include "sd_card.h"



#include "lwip/opt.h"
#include "lwip/init.h"
#include "netif/etharp.h"
#include "lwip/netif.h"
#include "lwip/lwip_timers.h"
#include "ethernetif.h"
#include "app_ethernet.h"
#include "httpd.h"


// Taskhandler
SCHEDULER_taskhandler TASK_alive_H = {0,RESET};
SCHEDULER_taskhandler TASK_shell_H = {0,RESET};
SCHEDULER_taskhandler TASK_shell_read_H = {0,RESET};
SCHEDULER_taskhandler TASK_demo1_H = {0,RESET};
SCHEDULER_taskhandler TASK_demo2_H = {0,RESET};
SCHEDULER_taskhandler TASK_demo3_H = {0,RESET};
SCHEDULER_taskhandler TASK_demo4_H = {0,RESET};


USBD_HandleTypeDef  USBD_Device;


struct netif gnetif;

static void Netif_Config(void);
static void SystemClock_Config(void);


/**
* @brief  Initializes the lwIP stack
* @param  None
* @retval None
*/
static void Netif_Config(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

  /* IP address default setting */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

  /* add the network interface */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernetif_update_config);
}





void BSP_LED_Off(uint8_t led)
{
	HAL_GPIO_WritePin(LED_PORT, led, RESET);
}
void BSP_LED_On(uint8_t led)
{
	HAL_GPIO_WritePin(LED_PORT, led, SET);
}




/**
  * @brief  Initialisiert die LEDs.
  * @param  None
  * @retval None
  */
void LED_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__GPIOD_CLK_ENABLE();

	//RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pin = LED_blue | LED_green | LED_orange | LED_red;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStructure);

	HAL_GPIO_WritePin(LED_PORT, LED_blue, SET);
	HAL_GPIO_WritePin(LED_PORT, LED_green, SET);
	HAL_GPIO_WritePin(LED_PORT, LED_orange, RESET);
	HAL_GPIO_WritePin(LED_PORT, LED_red, SET);
}



void tgl(int id, char* data)
{
	int a;
	sscanf(data,"%d",&a);

	if(a == 1)
		LED_PORT->ODR ^= LED_red;
	else if(a == 2)
		LED_PORT->ODR ^= LED_blue;
	else if(a == 3)
	{
		LED_PORT->ODR ^= LED_red;
		LED_PORT->ODR ^= LED_blue;
	}
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

	if(SCHEDULER_get_statustask(&TASK_demo4_H) != RESET)
	{
		SCHEDULER_disable_task(&TASK_demo3_H);
	}
}
void test_start(int id, char* data)
{
	SCHEDULER_enable_task(&TASK_demo3_H);
	SCHEDULER_set_statustask(&TASK_demo4_H, "TEEEST", 7 _s);
}




/**
  * @brief  Stellt die PLL auf die gewünschte Taktfrequenz ein.
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
                                   RCC_CLOCKTYPE_HCLK |
                                   RCC_CLOCKTYPE_PCLK1 |
                                   RCC_CLOCKTYPE_PCLK2);

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}





/**
  * @brief  Simply the main-function!
  * @param  None
  * @retval 0
  */
int main(void)
{
	HAL_Init();
	SystemClock_Config();

#define WICCAD_MACHT_ETHERNET	0

#if WICCAD_MACHT_ETHERNET
	  /* Initilaize the LwIP stack */
	  lwip_init();

	  /* Configure the Network interface */
	  Netif_Config();

	  /* Http webserver Init */
	  httpd_init();

	  /* Notify user about the netwoek interface config */
	  User_notification(&gnetif);

	  /* Infinite loop */
	  while (1)
	  {
	    /* Read a received packet from the Ethernet buffers and send it
	       to the lwIP for handling */
	    ethernetif_input(&gnetif);

	    /* Handle timeouts */
	    sys_check_timeouts();

	#ifdef USE_DHCP
	    /* handle periodic timers for LwIP */
	    DHCP_Periodic_Handle(&gnetif);
	#endif
	  }
#endif


	// Initialize scheduler
	SCHEDULER_init();
	// ------------------------------- //
	// DO NOT CHANGE ANYTHING ABOVE!!! //
	//     STEFFEN IS WARNING YOU :)   //
	// ------------------------------- //
		// Initialize hardware
			LED_init();
		// Initialize RNG
			rng_init();
		// Initialize shell
			shell_init();
			shell_register_cmd("tgl:",tgl);
			shell_register_cmd("test",test_start);
		// Initialize tasks
			SCHEDULER_set_task(&TASK_alive_H, "alive", 50 _ms,1,TASK_alive);
			SCHEDULER_set_task(&TASK_demo1_H, "demo1", 666 _ms,1,TASK_demo1);
			SCHEDULER_set_task(&TASK_demo2_H, "demo2", 222 _ms,1,TASK_demo2);
			SCHEDULER_set_task(&TASK_demo3_H, "demo3", 777 _ms,1,TASK_demo3);
		//
			adc_init();
		// Your well-documented and perfectly-working code comes here:
			//...


			FATFS FatFs;
			FIL fil;
			uint32_t total, free;
			//Mount drive
			if (f_mount(&FatFs, "", 1) == FR_OK)
			{
				//Try to open file
				if (f_open(&fil, "123.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
				{

					//If we put more than 0 characters (everything OK)
					if (f_puts("First string in my file\n", &fil) > 0)
					{
						if (TM_FATFS_DriveSize(&total, &free) == FR_OK)
						{
							//Data for drive size are valid
						}
					}

					//Close file, don't forget this!
					f_close(&fil);
				}

				//Unmount drive, don't forget this!
				f_mount(0, "", 1);
			}




	// ------------------------------- //
	// DO NOT CHANGE ANYTHING BELOW!!! //
	//     STEFFEN IS WARNING YOU :)   //
	// ------------------------------- //
	// Let the scheduler take control...
	SCHEDULER_main();
	return 0;
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
  }
}
