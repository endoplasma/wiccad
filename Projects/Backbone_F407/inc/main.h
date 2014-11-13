//////////////////////////////////////////////////////////////////////////////////////////////////
//		██╗  ██╗  ██╗      ██╗    ██╗██╗ ██████╗ ██████╗ █████╗ ██████╗       ██╗  ██╗  ██╗		//
//		╚██╗ ╚██╗ ╚██╗     ██║    ██║██║██╔════╝██╔════╝██╔══██╗██╔══██╗     ██╔╝ ██╔╝ ██╔╝		//
//		 ╚██╗ ╚██╗ ╚██╗    ██║ █╗ ██║██║██║     ██║     ███████║██║  ██║    ██╔╝ ██╔╝ ██╔╝		//
//		 ██╔╝ ██╔╝ ██╔╝    ██║███╗██║██║██║     ██║     ██╔══██║██║  ██║    ╚██╗ ╚██╗ ╚██╗		//
//		██╔╝ ██╔╝ ██╔╝     ╚███╔███╔╝██║╚██████╗╚██████╗██║  ██║██████╔╝     ╚██╗ ╚██╗ ╚██╗		//
//		╚═╝  ╚═╝  ╚═╝       ╚══╝╚══╝ ╚═╝ ╚═════╝ ╚═════╝╚═╝  ╚═╝╚═════╝       ╚═╝  ╚═╝  ╚═╝		//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include "stm32f4xx_hal.h"

#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"


#define CONCAT2(a,b)	a ## b
#define CONCAT(a,b)		CONCAT2(a,b)


#define BIT_IS_SET(REG, BIT)		(((REG) & (BIT)) != RESET)
#define BIT_IS_CLR(REG, BIT)		(((REG) & (BIT)) == RESET)
#define BIT_SET(REG, BIT)			((REG) |= (BIT))
#define BIT_CLR(REG, BIT)			((REG) &= ~(BIT))


#define		LED_PORT	GPIOD
//#define		LED_RCC		RCC_AHB1Periph_GPIOD
#define		LED_green	GPIO_PIN_12
#define		LED_orange	GPIO_PIN_13
#define		LED_red		GPIO_PIN_14
#define		LED_blue	GPIO_PIN_15
#define		LED1		GPIO_PIN_12
#define		LED2		GPIO_PIN_13
#define		LED3		GPIO_PIN_14
#define		LED4		GPIO_PIN_15





//#define USE_DHCP

#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   67

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   51









extern PCD_HandleTypeDef hpcd;


void BSP_LED_Off(uint8_t led);
void BSP_LED_On(uint8_t led);


void Error_Handler(void);


#endif /* MAIN_H_ */
