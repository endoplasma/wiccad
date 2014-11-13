#include "tasks.h"
#include "scheduler.h"
#include "cdc_shell.h"
#include "rng.h"
#include "adc.h"
#include "sd_card.h"




#define SHELL_CMDs_SLOTS			17
#define SHELL_OUTs_SLOTS			17


typedef struct {
	char cmd_name[42];
	uint8_t cmd_len;
	void (*cmd_function)(int,char*);
	uint8_t cmd_out;
} SHELL_CMD_DB;


typedef struct {
	uint8_t out_id;
	uint8_t out_active;
	uint8_t out_refresh;
	uint8_t out_exetuted;
	void (*out_function)(void);
} SHELL_OUT_DB;



volatile char cdc_connected = 0;
USBD_HandleTypeDef  USBD_Device;
usb_cdc_buffer s_RxBuffer;

SHELL_CMD_DB shell_cmds[SHELL_CMDs_SLOTS];
uint8_t shell_cmds_used = 0;

volatile SHELL_OUT_DB shell_outs[SHELL_OUTs_SLOTS];
uint8_t shell_outs_used = 0;


char shell_input[0xff] = {'\0'};
uint8_t shell_input_idx = 0;

uint8_t shell_initiated = RESET;
uint8_t shell_hidden_cmds = 0;






/**
  * @brief	Task zur Ausgabe der Shell.
  * @param  none
  * @retval None
  */
void TASK_shell()
{
	uint8_t program_executed = RESET;

	// Show current programm
	uint8_t i;
	for(i=0;i<shell_outs_used;i++)
	{
		// Programm gefunden zum AUsführen
		if(shell_outs[i].out_active != RESET)
		{
			// Programm soll auch wirklich ausgeführt werden (endlose Aktualisierung oder erstmaliges Ausführen)
			if(shell_outs[i].out_refresh != RESET || shell_outs[i].out_exetuted == RESET)
			{
				shell_out_header();

				shell_outs[i].out_exetuted = SET;
				shell_outs[i].out_function();

				shell_out("\r\n\r\n");
			}
			program_executed = SET;
			break;
		}
	}

	if(program_executed == RESET)
	{
		shell_out_header();
	}

	shell_out(ANSI_ERASE_LINE""ANSI_GOTO_COL1""ANSI_FG_DEFAULT"> "ANSI_FG_CYAN);
	shell_out(shell_input);
}



/**
  * @brief	Shell-Header-Ausgabe
  * @param  none
  * @retval None
  */
void shell_out_header()
{
	shell_out(ANSI_RESET);
	shell_out(ANSI_CLEAR);
	shell_out(ANSI_HOME);
	shell_logo_out();
}



/**
  * @brief	Task zur Eingabe der Shell
  * @param  none
  * @retval None
  */
void TASK_shell_read()
{
	// Neue Daten bereit
	if (s_RxBuffer.ReadDone == 1)
	{
		// Anzahl empfangener Bytes
		int remaining = s_RxBuffer.Size - s_RxBuffer.Position;
		if(remaining > 0)
		{
			// Dem shell_input Buffer hinzufügen und Befehl auswerten
			while(remaining-->0)
			{
				if(s_RxBuffer.Buffer[s_RxBuffer.Position] == 127)
				{
					if(shell_input_idx > 0)
						shell_input_idx--;
					shell_input[shell_input_idx] = '\0';
				}
				else if(s_RxBuffer.Buffer[s_RxBuffer.Position] == '\r')
				{
					// Auswertung bei erkanntem ENTER
					uint8_t i;
					for(i=0;i<shell_cmds_used;i++)
					{
						if(strncmp(shell_input,shell_cmds[i].cmd_name,shell_cmds[i].cmd_len) == 0)
						{
							shell_cmds[i].cmd_function(i, shell_input+shell_cmds[i].cmd_len);
							break;
						}
					}

					// Buffer leeren
					memset(shell_input, 0, sizeof(shell_input));
					shell_input_idx = 0;
				}
				else
				{
					shell_input[shell_input_idx++] = s_RxBuffer.Buffer[s_RxBuffer.Position];
				}

				s_RxBuffer.Position++;
			}
			s_RxBuffer.ReadDone = 0;
			USBD_CDC_ReceivePacket(&USBD_Device);



			// Bereit, neue Daten zu empfangen
			if (s_RxBuffer.Position >= s_RxBuffer.Size)
			{
				s_RxBuffer.ReadDone = 0;
				USBD_CDC_ReceivePacket(&USBD_Device);
			}
		}
	}
}





/**
  * @brief	Gibt Status-Informationen auf der Shell aus.
  * @param  none
  * @retval None
  */
void shell_info_out()
{
	shell_out(ANSI_BOLD"Infos"ANSI_BOLD_OFF"\r\n");

	char out[300];
	sprintf(out,"Speed:\t\t%u\r\n",SystemCoreClock);
	shell_out(out);

	uint32_t *devID = (uint32_t *)0x1FFF7A10;
	sprintf(out,"Device-ID:\t%08X %08X %08X\r\n", devID[0], devID[1], devID[2]);
	shell_out(out);

	sprintf(out,"RNG:\t\t%u\r\n", rng());
	shell_out(out);

	sprintf(out,"Temp:\t\t%4.1f °C\r\n", adc_get_temp_in_celsius());
	shell_out(out);
}



/**
  * @brief  Zeigt alle registrierten Befehle auf der Shell an.
  * @param  none
  * @retval None
  */
void shell_help_out()
{
	shell_out(ANSI_BOLD"Verfügbare Kommandos"ANSI_BOLD_OFF"\r\n");
	shell_out(ANSI_FG_GREEN"Anzeigeprogramm \t"ANSI_FG_ORANGE"Befehl"ANSI_FG_DEFAULT"\r\n");

	// Auswertung bei erkanntem ENTER
	uint8_t i;
	for(i=shell_hidden_cmds;i<shell_cmds_used;i++)
	{
		char out[150];
		sprintf(out,"#%d\t%s%s"ANSI_FG_DEFAULT"\r\n",
				i,
				(shell_cmds[i].cmd_out != RESET)?ANSI_FG_GREEN:ANSI_FG_ORANGE,
				shell_cmds[i].cmd_name);
		shell_out(out);
	}
}






/**
  * @brief  Ausgabe des wunderschönen Logos auf der Shell.
  * @param  none
  * @retval None
  */
void shell_logo_out2()
{
	shell_out(ANSI_FG_RED);
	shell_out("__          _______ _____ _____          _____ \r\n");
	shell_out("\\ \\        / /_   _/ ____/ ____|   /\\   |  __ \\ \r\n");
	shell_out(" \\ \\  /\\  / /  | || |   | |       /  \\  | |  | |\r\n");
	shell_out("  \\ \\/  \\/ /   | || |   | |      / /\\ \\ | |  | |\r\n");
	shell_out("   \\  /\\  /   _| || |___| |____ / ____ \\| |__| |\r\n");
	shell_out("    \\/  \\/   |_____\\_____\\_____/_/    \\_\\_____/\r\n\r\n");
	shell_out(ANSI_FG_DEFAULT);
}
void shell_logo_out()
{
	shell_out(ANSI_BOLD""ANSI_FG_RED"__      _____ ___ ___   _   ___ \r\n");
	shell_out("\\ \\    / /_ _/ __/ __| /_\\ |   \\ \r\n");
	shell_out(" \\ \\/\\/ / | | (_| (__ / _ \\| |) |\r\n");
	shell_out("  \\_/\\_/ |___\\___\\___/_/ \\_\\___/\r\n\r\n"ANSI_BOLD_OFF""ANSI_FG_DEFAULT);
}




/**
  * @brief	Kleine Shell-Animation.
  * @param  none
  * @retval None
  */
#define ANIMATION_DELAY	2
void alive_animation()
{
	static uint8_t frame = 0;
	if(frame >= 4*ANIMATION_DELAY)			frame = 0;
	if(frame < ANIMATION_DELAY)				{shell_out("/");}
	else if(frame < 2*ANIMATION_DELAY)		{shell_out("-");}
	else if(frame < 3*ANIMATION_DELAY)		{shell_out("\\");}
	else if(frame < 4*ANIMATION_DELAY)		{shell_out("|");}
	frame++;
	shell_out("\r\n");
}


/**
  * @brief	Registriert einen einen Befehl für die Shell.
  * 		ACHTUNG: Es darf keine Ausgabe auf der Shell durch diesen Befehl erfolgen!
  * @param  none
  * @retval None
  */
uint8_t shell_register_cmd(char* name, void(*fctn)(int,char*))
{
	if(shell_cmds_used < SHELL_CMDs_SLOTS)
	{
		strcpy(shell_cmds[shell_cmds_used].cmd_name, name);
		shell_cmds[shell_cmds_used].cmd_len = strlen(name);
		shell_cmds[shell_cmds_used].cmd_function = fctn;
		shell_cmds[shell_cmds_used].cmd_out = RESET;

		return shell_cmds_used++;
	}
	else
	{
		shell_error_handler("SHELL_CMDs_SLOTS: Give me more slots please :'( And money! Don't forget the money :)");
	}
	return 0;
}



/**
  * @brief	Registriert einen Befehl, der eine Funktion zur Ausgabe auf der Shell aufruft.
  * @param  none
  * @retval None
  */
void shell_register_out(char* name, uint8_t refresh, void(*fctn)())
{
	if(shell_outs_used < SHELL_OUTs_SLOTS)
	{
		uint8_t id = shell_register_cmd(name,shell_activate_program);
		shell_cmds[id].cmd_out = SET;
		shell_outs[shell_outs_used].out_id = id;
		shell_outs[shell_outs_used].out_active = 0;
		shell_outs[shell_outs_used].out_refresh = refresh;
		shell_outs[shell_outs_used].out_exetuted = RESET;
		shell_outs[shell_outs_used].out_function = fctn;
		shell_outs_used++;
	}
	else
	{
		shell_error_handler("SHELL_OUTs_SLOTS: Give me more slots please :'( And money! Don't forget the money :)");
	}
}



/**
  * @brief  Stoppt das aktuelle (genauer: alle) Ausgabe-Programm der Shell.
  * @param  id
  * @retval data
  */
void shell_exit_program()
{
	uint8_t i;
	for(i=0;i<shell_outs_used;i++)
	{
		if(shell_outs[i].out_active == 1)
		{
			shell_outs[i].out_active = 0;
			shell_outs[i].out_exetuted = RESET;
		}
	}

	shell_out_header();
}



/**
  * @brief  Aktiviert ein neues Ausgabe-Programm auf der Shell.
  * @param  id:
  * @retval data
  */
void shell_activate_program(int id, char* data)
{
	// Vorheriges Programm beenden
	shell_exit_program(id, data);

	// Neues Programm ausführen
	uint8_t i;
	for(i=0;i<shell_outs_used;i++)
	{
		if(shell_outs[i].out_id == id)
		{
			shell_outs[i].out_active = 1;
			break;
		}
	}
}





/**
  * @brief  Sinn des Lebens...
  * @param  none
  * @retval None
  */
void shell_hidden_42()
{
	shell_out("\r\n\r\n\r\n"ANSI_BOLD"Du kennst die Antwort.\r\nAber kennst du auch die Frage?"ANSI_BOLD_OFF"\r\n\r\n");
}


/**
  * @brief  Initialisiert die USB-CDC Verbindung und startet die Shell-Ausgabe.
  * @param  none
  * @retval None
  */
void shell_init()
{
	if(
			// Initialize USB
			USBD_Init(&USBD_Device, &VCP_Desc, 0) == USBD_OK
			// Add Supported Class
			&& USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS) == USBD_OK
			// Add CDC Interface Class
			&& USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops) == USBD_OK
			//* Start Device Process
			&& USBD_Start(&USBD_Device) == USBD_OK
		)
	{
		// Warte, bis USB-Verbindung erfolgreich aufgebaut
		uint8_t TASK_USB_check_connection;
		SCHEDULER_set_statustask(&TASK_USB_check_connection, "USB_CON", 10 _s);

		// Verusche USB-Verbindung herzustellen
		while(1)
		{
			// Verbindung erfolgreich hergestellt
			if(cdc_connected == 42)
			{
				shell_initiated = SET;

				// Tasks zur Shell-Aus-Eingabe erstellen
				SCHEDULER_set_task(&TASK_shell_read_H, "shell_r", 60 _ms,1,TASK_shell_read);
				SCHEDULER_set_task(&TASK_shell_H, "shell_o", 60 _ms,1,TASK_shell);

				// Grundlegende Befehle erstellen:
				shell_register_out("42",RESET,shell_hidden_42);
				shell_register_out("exit",RESET,shell_exit_program);
				shell_register_out("help",RESET,shell_help_out);
				shell_register_out("info",SET,shell_info_out);


				shell_hidden_cmds = 1;	// Wers findet, darfs behalten
				break;
			}
			else if(SCHEDULER_get_statustask(&TASK_USB_check_connection) != RESET)
			{
				shell_initiated = RESET;
				break;
			}
		}
	}
}





/**
  * @brief  Sendet Zeichenketten per USB_CDC.
  * @param  none
  * @retval None
  */
int shell_out(char* str)
{
	if(shell_initiated == SET)
	{
		int size = strlen(str);
		if (size >= CDC_DATA_HS_OUT_PACKET_SIZE)
		{
			int offset;
			for (offset = 0; offset < size; offset++)
			{
				int todo = MIN(CDC_DATA_HS_OUT_PACKET_SIZE,
							   size - offset);
				int done = shell_out(((char *)str) + offset);
				if (done != todo)
					return offset + done;
			}

			return size;
		}

		USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)USBD_Device.pClassData;
		while(pCDC->TxState) { } //Wait for previous transfer

		USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t *)str, size);
		if (USBD_CDC_TransmitPacket(&USBD_Device) != USBD_OK)
			return 0;

		while(pCDC->TxState) { } //Wait until transfer is done
		return size;
	}
	return 0;
}


/**
  * @brief  Error-Handler
  * @param  str:	Fehlerbeschreibung -> Im Debug-Modus anschauen!
  * @retval None
  */
void shell_error_handler(char* str)
{
	// Guckst du, was in "str" steht, dann weißt du, wo Probelm steckt
	while(1)
	{
		LED_PORT->ODR ^= LED_red;
	}
}
