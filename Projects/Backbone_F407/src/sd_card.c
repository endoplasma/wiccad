#include "cdc_shell.h"
#include "sd_card.h"



SPI_HandleTypeDef SpiHandle;



void __init_spi()
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	SpiHandle.Instance               = SPI2;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;
	SpiHandle.Init.Mode 			 = SPI_MODE_MASTER;

	// Clock enable
	__GPIOB_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__SPI2_CLK_ENABLE();

	// SPI-Config (SCK, MISO, MOSI, CS)
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	// SCK: 10		MISO: 14	MOSI: 15
	GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// CS
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_SPI_Init(&SpiHandle);
}






void sd_cmd_ls()
{
	char path[20] = "";
	char out[100] = "";


	shell_out("\r\n\r\nFiles:\r\n");
	FATFS FatFs;
	if (f_mount(&FatFs, "", 1) == FR_OK)
	{
		FRESULT res;
		FILINFO fno;
		DIR dir;
		int i;
		char *fn;   // This function assumes non-Unicode configuration

		res = f_opendir(&dir, path);                       // Open the directory
		if (res == FR_OK)
		{
			i = strlen(path);
			for (;;)
			{
				res = f_readdir(&dir, &fno);                   // Read a directory item
				if(res != FR_OK || fno.fname[0] == 0)
					break;  // Break on error or end of dir

				if(fno.fname[0] == '.')
					continue;             // Ignore dot entry

				fn = fno.fname;
				// It is a directory
				if(fno.fattrib & AM_DIR)
				{

					sprintf(out,ANSI_FG_MAGENTA"%s/%s/\r\n", path, fn);
				}
				else
				{
					sprintf(out,ANSI_FG_CYAN"%s/%s\r\n", path, fn);
				}
				shell_out(out);
			}
			f_closedir(&dir);
		}
	}
	else
	{
		shell_out(ANSI_FG_RED"Keine SD-Karte gefunden!\r\n");
	}
}




void sd_cmd_mkdir(int id, char* cmd)
{
	FATFS FatFs;
	if (f_mount(&FatFs, "", 1) == FR_OK)
	{
		f_mkdir ("blub");
	}
	else
	{
		shell_out(ANSI_FG_RED"Keine SD-Karte gefunden!\r\n");
	}
}




void __init_sd_cmds()
{
	if(shell_initiated != RESET)
	{
		shell_register_out("ls",RESET,sd_cmd_ls);
		shell_register_cmd("mkdir",sd_cmd_mkdir);
		//shell_register_cmd("cd",sd_cmd_mkdir);
		//shell_register_cmd("rm",sd_cmd_mkdir);
	}
}








uint8_t TM_SPI_Send(uint8_t tx)
{
	uint8_t rx;
	HAL_SPI_TransmitReceive(&SpiHandle, &tx, &rx, 1, 500);
	return rx;
}

void TM_SPI_SendMulti(uint8_t* dataOut, uint8_t* dataIn, uint16_t count)
{
	uint16_t i;
	for (i = 0; i < count; i++) {
		dataIn[i] = TM_SPI_Send(dataOut[i]);
	}
}

void TM_SPI_WriteMulti(uint8_t* dataOut, uint16_t count)
{
	uint16_t i;
	for (i = 0; i < count; i++) {
		TM_SPI_Send(dataOut[i]);
	}
}

void TM_SPI_ReadMulti(uint8_t* dataIn, uint8_t dummy, uint16_t count)
{
	uint16_t i;
	for (i = 0; i < count; i++) {
		dataIn[i] = TM_SPI_Send(dummy);
	}
}









FRESULT TM_FATFS_DriveSize(uint32_t* total, uint32_t* free)
{
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("0:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;

	/* Return OK */
	return FR_OK;
}

FRESULT TM_FATFS_USBDriveSize(uint32_t* total, uint32_t* free)
{
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("1:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;

	/* Return OK */
	return FR_OK;
}
