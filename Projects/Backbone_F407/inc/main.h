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

#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_crc.h"
#include "stm32f4xx_cryp.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_dbgmcu.h"
#include "stm32f4xx_dcmi.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_fsmc.h"
#include "stm32f4xx_hash.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_sdio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_wwdg.h"
#include "misc.h"




#define CONCAT2(a,b)	a ## b
#define CONCAT(a,b)		CONCAT2(a,b)

#define		LED_PORT	GPIOD
#define		LED_RCC		RCC_AHB1Periph_GPIOD
#define		LED_green	GPIO_Pin_12
#define		LED_orange	GPIO_Pin_13
#define		LED_red		GPIO_Pin_14
#define		LED_blue	GPIO_Pin_15




#endif /* MAIN_H_ */
