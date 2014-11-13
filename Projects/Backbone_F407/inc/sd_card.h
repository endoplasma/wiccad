#ifndef SPI_H_
#define SPI_H_

#include "main.h"
// fatfs
//#include "defines.h"
//#include "tm_stm32f4_fatfs.h"
#include "scheduler.h"
#include "ff.h"


#define    FATFS_USE_SDIO                0


void __init_spi();
void __init_sd_cmds();
uint8_t TM_SPI_Send(uint8_t tx);
void TM_SPI_SendMulti(uint8_t* dataOut, uint8_t* dataIn, uint16_t count);
void TM_SPI_WriteMulti(uint8_t* dataOut, uint16_t count);
void TM_SPI_ReadMulti(uint8_t* dataIn, uint8_t dummy, uint16_t count);


void sd_cmd_ls();



FRESULT TM_FATFS_DriveSize(uint32_t* total, uint32_t* free);
FRESULT TM_FATFS_USBDriveSize(uint32_t* total, uint32_t* free);

#endif /* SPI_H_ */
