/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2013          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED_SD
#define _DISKIO_DEFINED_SD

#define _USE_WRITE	1	/* 1: Enable disk_write function */
#define _USE_IOCTL	1	/* 1: Enable disk_ioctl fucntion */

#include "diskio.h"
#include "integer.h"
#include "sd_card.h"

#ifndef FATFS_CS_PIN
#define FATFS_CS_PORT						GPIOD
#define FATFS_CS_PIN						GPIO_PIN_9
#endif


#define FATFS_CS_LOW						FATFS_CS_PORT->BSRRH = FATFS_CS_PIN
#define FATFS_CS_HIGH						FATFS_CS_PORT->BSRRL = FATFS_CS_PIN

/*---------------------------------------*/
/* Prototypes for disk control functions */
DSTATUS TM_FATFS_SD_disk_initialize(void);
DSTATUS TM_FATFS_SD_disk_status(void);
DRESULT TM_FATFS_SD_disk_read(BYTE* buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SD_disk_write(const BYTE* buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SD_disk_ioctl(BYTE cmd, void* buff);

#endif

