#ifndef CDC_SHELL_H_
#define CDC_SHELL_H_

#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "ansi.h"




#define SHELL_BYTE "%d%d%d%d%d%d%d%d"
#define SHELL_BYTE2BIN(b) (b&0x80?1:0),(b&0x40?1:0),(b&0x20?1:0),(b&0x10?1:0),(b&0x08?1:0),(b&0x04?1:0),(b&0x02?1:0),(b&0x01?1:0)


typedef struct
{
    uint8_t Buffer[CDC_DATA_HS_OUT_PACKET_SIZE];
    int Position, Size;
    char ReadDone;
} usb_cdc_buffer;



extern USBD_HandleTypeDef  USBD_Device;
extern usb_cdc_buffer s_RxBuffer;
extern volatile char cdc_connected;
extern uint8_t shell_initiated;


void TASK_shell_read();
void TASK_shell();

void shell_info_out();
void shell_help_out();
void shell_logo_out();
void alive_animation ();
int shell_out(char* str);
void shell_out_header();

uint8_t shell_register_cmd(char* name, void(*fctn)(int,char*));
void shell_register_out(char* name, uint8_t refresh, void(*fctn)());
void shell_exit_program();
void shell_activate_program(int id, char* data);
void shell_init();
void shell_error_handler(char* str);


#endif /* CDC_SHELL_H_ */
