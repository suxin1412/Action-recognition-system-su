#include <STC15F2K60S2.H>
#include "Bluetooth.h"
unsigned char  UartSendBusy = 0;
void Bluetooth_Init(void)		//115200bps@20.000MHz
{
	SCON = 0x50;	
	AUXR |= 0x01;	
	AUXR |= 0x04;	
	T2L = 0xD5;		
	T2H = 0xFF;		
	AUXR |= 0x10;		//Æô¶¯T2
	ES = 1;
	EA = 1;
}
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //??RI?
    }
    if (TI)
    {
        TI = 0;                 //??TI?
        UartSendBusy = 0;               //????
    }
}
void Bluetooth_SendByte(int8_t dat)
{
	while (UartSendBusy);
	SBUF = dat;
	UartSendBusy = 1;
}
void Bluetooth_SendString(char *str)
{
	while (*str){
		Bluetooth_SendByte(*str);
		str++;
	}
}
void Bluetooth_SendBuffer(uint8_t *buf, uint8_t length){
	uint8_t i = 0;
	while (i<length){
		Bluetooth_SendByte(buf[i++]);
	}
}