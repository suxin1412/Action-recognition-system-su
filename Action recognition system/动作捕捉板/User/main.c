#include <STC15F2K60S2.H>
#include "MPU9250.h"
#include "TypeDefine.h"
#include "Bluetooth.h"
#include <stdio.h>
#ifdef DEBUG_
char BT_Buf[7];
void Delay500ms();		//@20.000MHz
void Uint16ToString(uint16_t res);
#endif
//
uint8_t DataBuffer[24] = {0xa5, 
																0x00, 0x00, 0x00,	//asa 1 2 3 
																0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//acc 4-5 6-7 8-9 
																0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //gyro 10-11 12-13 14-15
																0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//mag 16-17 18-19 20-21
													0x00, 0x5a};	//crc 22
void BT_SendAxisData();
void main()
{
#ifdef DEBUG_
	BT_Buf[5] = ' ';
	BT_Buf[6] = 0;
#endif
	Bluetooth_Init();
	MPU_Init();
	while(1)
	{
		MPU_Read6Axis();
		MPU_ReadMag();
		BT_SendAxisData();
	}
}

#ifdef DEBUG_
void Delay500ms()		//@20.000MHz
{
	unsigned char i, j, k;
	i = 38;
	j = 0;
	k = 118;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Uint16ToString(uint16_t res){
	BT_Buf[0] = res/10000 + '0';
	BT_Buf[1] = (res%10000)/1000 + '0';
	BT_Buf[2] = (res%1000)/100 + '0';
	BT_Buf[3] = (res%100)/10 + '0';
	BT_Buf[4] = (res%10) + '0';
}
#endif
uint8_t CRC(){
	uint8_t i = 1;
	uint8_t res = 0;
	while (i<22){
		res += DataBuffer[i++];
	}
	return res;
}
void BT_SendAxisData(){
#ifdef DEBUG_
	Uint16ToString(accelX);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(accelY);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(accelZ);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(gyroX);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(gyroY);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(gyroZ);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(magX);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(magY);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(magZ);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(ASAX);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(ASAY);
	Bluetooth_SendString(BT_Buf);
	Uint16ToString(ASAZ);
	Bluetooth_SendString(BT_Buf);
	Bluetooth_SendString("\r\n"); 
#else
	DataBuffer[1] = ASAX;
	DataBuffer[2] = ASAY;
	DataBuffer[3] = ASAZ;
	DataBuffer[4] = accelX&0xff;
	DataBuffer[5] = (accelX>>8)&0xff;
	DataBuffer[6] = accelY&0xff;
	DataBuffer[7] = (accelY>>8)&0xff;
	DataBuffer[8] = accelZ&0xff;
	DataBuffer[9] = (accelZ>>8)&0xff;
	DataBuffer[10] = gyroX&0xff;
	DataBuffer[11] = (gyroX>>8)&0xff;
	DataBuffer[12] = gyroY&0xff;
	DataBuffer[13] = (gyroY>>8)&0xff;
	DataBuffer[14] = gyroZ&0xff;
	DataBuffer[15] = (gyroZ>>8)&0xff;
	DataBuffer[16] = magX&0xff;
	DataBuffer[17] = (magX>>8)&0xff;
	DataBuffer[18] = magY&0xff;
	DataBuffer[19] = (magY>>8)&0xff;
	DataBuffer[20] = magZ&0xff;(
	DataBuffer[21] = magZ>>8)&0xff;
	DataBuffer[22] = CRC();
	Bluetooth_SendBuffer(DataBuffer, 24);
#endif
}