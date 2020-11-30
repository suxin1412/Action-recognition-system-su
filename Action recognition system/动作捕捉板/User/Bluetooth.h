#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "TypeDefine.h"
void Bluetooth_Init(void);		//115200bps@20.000MHz
void Bluetooth_SendString(char *str);
void Bluetooth_SendByte(int8_t dat);
void Bluetooth_SendBuffer(uint8_t *buf, uint8_t length);
#endif

