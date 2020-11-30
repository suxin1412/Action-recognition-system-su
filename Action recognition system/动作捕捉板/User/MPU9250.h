#ifndef _MPU6050_H_
#define _MPU6050_H_
#include "TypeDefine.h"
//****************************************
// ֨ӥMPU6050Śҿַ֘
//****************************************
#define	SMPLRT_DIV		0x19	//ΓÝӇӉҹÊì֤эֵú0x07(125Hz)
#define	CONFIG			0x1A	//֍ͨËҨƵÊì֤эֵú0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//ΓÝӇؔݬܰӢ׶Χì֤эֵú0x18(һؔݬì2000deg/s)
#define	ACCEL_CONFIG	0x1C	//ݓ̙݆ؔݬbӢ׶ΧٟܰͨËҨƵÊì֤эֵú0x01(һؔݬì2Gì5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//֧Դڜmì֤эֵú0x00(ֽӣǴԃ)
#define	WHO_AM_I		0x75	//IICַ݄֘զǷ(Ĭɏ˽ֵ0x68ìֻׁ)
#define	SlaveAddress	0xD0	//IICдɫʱքַ֘ؖޚ˽ߝì+1Ϊׁȡ
#define	ACCEL_CONFIG_2                      0x1D 
#define INT_PIN_CFG                         0x37 //中断配置
#define USER_CTRL                           0x6a
#define I2C_MST_CTRL                        0x24
#define I2C_MST_DELAY_CTRL                  0x67
#define EXT_SENS_DATA_00    0x49  //MPU9250 IIC外挂器件读取返回寄存器00
//--------------------i2c slv0-------------------------------//
#define I2C_SLV0_ADDR                       0x25  
#define I2C_SLV0_REG                        0x26
#define I2C_SLV0_CTRL                       0x27 
#define I2C_SLV0_DO                         0x63 //output reg
//--------------------AK8963 reg addr------------------------//
#define MPU9250_AK8963_ADDR                 0x18  //AKM addr
#define AK8963_WHOAMI_REG                   0x00  //AKM ID addr
#define AK8963_WHOAMI_ID                    0x48  //ID
#define AK8963_ST1_REG                      0x02  //Data Status1
#define AK8963_ST2_REG                      0x09  //Data reading end register & check Magnetic sensor overflow occurred
#define AK8963_ST1_DOR                      0x02  //MASK
#define AK8963_ST1_DRDY                     0x01 	//MASK Data Ready
#define AK8963_ST2_BITM                     0x10
#define AK8963_ST2_HOFL                     0x08 // Magnetic sensor overflow 
#define AK8963_CNTL1_REG                    0x0A
#define AK8963_CNTL2_REG                    0x0B
#define AK8963_CNTL2_SRST                   0x01 //soft Reset
#define AK8963_ASAX                         0x10 //X-axis sensitivity adjustment value 
#define AK8963_ASAY                         0x11 //Y-axis sensitivity adjustment value
#define AK8963_ASAZ                         0x12 //Z-axis sensitivity adjustment value
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


#define	NOP_MPU _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_

sbit MPU_SCL = P1^3;
sbit MPU_SDA = P1^4;
// sbit SCL = P1^3;
// sbit SDA = P1^4;
// sbit INT = P1^5;
// sbit FSYNC = P5^5;
// sbit NCS = P5^4;
extern uint16_t accelX;
extern uint16_t accelY;
extern uint16_t accelZ;
extern uint16_t gyroX;
extern uint16_t gyroY;
extern uint16_t gyroZ;
extern uint16_t magX;
extern uint16_t magY;
extern uint16_t magZ;
extern uint8_t  ASAX;
extern uint8_t  ASAY;
extern uint8_t  ASAZ;


//****************************************
//MPU6050گ˽
//****************************************
void MPU_Init(void);
void MPU_Read6Axis(void);
unsigned char MPU_ReadRegister(unsigned char addr);
unsigned char AK8963_ReadRegister(unsigned char addr);
void AK8963_WriteReg(unsigned char Address,unsigned char dat);
void MPU_ReadMag(void);
#endif






