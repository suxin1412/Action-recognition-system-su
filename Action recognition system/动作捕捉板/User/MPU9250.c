#include <STC15F2K60S2.H>
#include "intrins.h"
#include "MPU9250.h"
//****************************************
//		IICǰʼхۅ
//		עúǰʼхۅǰìSCLۍSDAΪٟ
//			  ǰʼӔ۳ìSCLۍSDAΪ֍
//****************************************
uint16_t accelX;
uint16_t accelY;
uint16_t accelZ;
uint16_t gyroX;
uint16_t gyroY;
uint16_t gyroZ;
uint16_t magX;
uint16_t magY;
uint16_t magZ;
uint8_t  ASAX;
uint8_t  ASAY;
uint8_t  ASAZ;


void iic_mpu_start(void) 
{
	MPU_SCL = 1;
	MPU_SDA = 1;
	NOP_MPU();
	MPU_SDA = 0;
	NOP_MPU();
	MPU_SCL = 0;
}

//****************************************
//		IICޡ˸хۅ
//		עúޡ˸хۅǰìѨҪѣ֤SCLۍSDAΪ֍
//			  ޡ˸Ӕ۳ìSCLۍSDAΪٟ
//****************************************
void iic_mpu_end(void)
{
	MPU_SCL = 0;
	MPU_SDA = 0;
	NOP_MPU();
	MPU_SDA = 1;
	NOP_MPU();
	MPU_SCL = 1;
}


//****************************************
//		ׁӦհ
//		עúǰìѨҪѣ֤SCLۍSDAΪ֍
//			  ۳ìSCLۍSDAΪ֍
//****************************************
bit iic_mpu_read_echo(void)
{
	NOP_MPU();
	MPU_SCL = 1;
	F0 = MPU_SDA;
	NOP_MPU();
	MPU_SCL = 0;
	return F0;
}
//****************************************
//		дӦհ
//		עúǰìѨҪѣ֤SCLۍSDAΪ֍
//			  ۳ìSCLۍSDAΪ֍
//****************************************
void iic_mpu_write_echo(bit ack)
{
	MPU_SDA = ack;
	NOP_MPU();
	MPU_SCL = 1;
	NOP_MPU();
	NOP_MPU();
	MPU_SCL = 0;
}
//****************************************
//		IICׁыλèһؖޚé
//		עúǰìѨҪѣ֤SCLۍSDAΪ֍
//			  ۳ìSCLۍSDAΪ֍
//****************************************
unsigned char iic_mpu_read_byte(void)
{
	unsigned char dat=0,i;
	MPU_SDA = 1;//ˍ؅˽ߝ؜П
	for (i=0;i<8;i++)
	{
		dat = dat << 1;
		NOP_MPU();										//ٸՓܺʱݤضԶ״Ӧ
		MPU_SCL = 1;
		dat = dat|MPU_SDA;
		NOP_MPU();
		MPU_SCL = 0;
	}
	return dat;
}
//****************************************
//		IICдыλèһؖޚé
//		עúǰìѨҪѣ֤SCLۍSDAΪ֍
//			  ۳ìSCLۍSDAΪ֍
//****************************************
void iic_mpu_write_byte(unsigned char dat)
{
	unsigned char i;
	for (i=0;i<8;i++)
	{
		dat<<=1;
		MPU_SDA = CY;
		NOP_MPU();
		MPU_SCL = 1;
		NOP_MPU();
		NOP_MPU();
		MPU_SCL = 0;
		NOP_MPU();
	}
	iic_mpu_read_echo();
}
//****************************************
//		IICָׁ֨λ׃èaddréք˽ߝ
//****************************************
unsigned char iic_mpu_read_data(unsigned char addr)
{
	unsigned char REG_data;
	iic_mpu_start();                   //ǰʼхۅ
	iic_mpu_write_byte(SlaveAddress);    //ע̍ʨѸַ֘+дхۅ
	iic_mpu_write_byte(addr);     //ע̍զԢեԪַ֘ìՓ0ߪʼ	
	iic_mpu_start();                   //ǰʼхۅ
	iic_mpu_write_byte(SlaveAddress+1);  //ע̍ʨѸַ֘+ׁхۅ
	REG_data=iic_mpu_read_byte();       //ׁԶ݄զǷ˽ߝ
	iic_mpu_write_echo(1);                //Ӧհхۅ
	iic_mpu_end();                    //ֹͣхۅ
	return REG_data;
}
//****************************************
//		IICд˽ߝսָ֨λ׃èaddré
//****************************************
void iic_mpu_write_data(unsigned char Address,unsigned char dat)
{
	  iic_mpu_start();                  	//ǰʼхۅ
    iic_mpu_write_byte(SlaveAddress+0); //ע̍ʨѸַ֘+дхۅ
    iic_mpu_write_byte(Address);    //Śҿ݄զǷַ֘ì
    iic_mpu_write_byte(dat);       //Śҿ݄զǷ˽ߝì
    iic_mpu_end();                   		//עֹ̍ͣхۅ
}

void Delay200us()		//@20.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 195;
	k = 136;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
// 地磁计的iic专用函数
unsigned char AK8963_ReadReg(unsigned char addr)
{
	unsigned char REG_data;
	iic_mpu_write_data(INT_PIN_CFG ,0x02);
	iic_mpu_start();
	iic_mpu_write_byte(MPU9250_AK8963_ADDR);
	iic_mpu_write_byte(addr);
	iic_mpu_start();
	iic_mpu_write_byte(MPU9250_AK8963_ADDR+1);
	REG_data=iic_mpu_read_byte();
	iic_mpu_write_echo(1);
	iic_mpu_end();
	return REG_data;
}
void AK8963_WriteReg(unsigned char Address,unsigned char dat)
{
	iic_mpu_write_data(INT_PIN_CFG ,0x02);
	iic_mpu_start();
	iic_mpu_write_byte(MPU9250_AK8963_ADDR+0);
	iic_mpu_write_byte(Address);
	iic_mpu_write_byte(dat);
	iic_mpu_end();
}

void MPU_Init()
{
	unsigned char i = 8, j = 154, k = 122;
	iic_mpu_write_data(PWR_MGMT_1, 0x80);  		//复位-重置所有寄存器
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);	//延时保证正常
///*******************Init GYRO and ACCEL******************************/	
	iic_mpu_write_data(CONFIG, 0x07);      //低通滤波频率，典型值：0x07(3600Hz)此寄存器内决定Internal_Sample_Rate==8K
	iic_mpu_write_data(SMPLRT_DIV, 0x07);  //陀螺仪采样率，典型值：0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
	iic_mpu_write_data(GYRO_CONFIG, 0x18); //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	iic_mpu_write_data(ACCEL_CONFIG_2, 0x08);//加速计高通滤波频率 典型值 ：0x08  （1.13kHz）	
	iic_mpu_write_data(ACCEL_CONFIG, 0x10);//加速计自检、测量范围及高通滤波频率，典型值：0x00/+-2g. 0x08/+-4g. 0x10/+-8g. 0x18(不自检，16G)
  AK8963_WriteReg(AK8963_CNTL1_REG,0x16); //16bit 连续测量模式2
}
void MPU_Read6Axis(void)
{
	unsigned char H,L;
	H=iic_mpu_read_data(ACCEL_XOUT_H);
	L=iic_mpu_read_data(ACCEL_XOUT_L);
	accelX =  ((H<<8)+L);
	H=iic_mpu_read_data(ACCEL_YOUT_H);
	L=iic_mpu_read_data(ACCEL_YOUT_L);
	accelY =  ((H<<8)+L);
	H=iic_mpu_read_data(ACCEL_ZOUT_H);
	L=iic_mpu_read_data(ACCEL_ZOUT_L);
	accelZ =  ((H<<8)+L);
	H=iic_mpu_read_data(GYRO_XOUT_H);
	L=iic_mpu_read_data(GYRO_XOUT_L);
	gyroX =  ((H<<8)+L);
	H=iic_mpu_read_data(GYRO_YOUT_H);
	L=iic_mpu_read_data(GYRO_YOUT_L);
	gyroY =  ((H<<8)+L);
	H=iic_mpu_read_data(GYRO_ZOUT_H);
	L=iic_mpu_read_data(GYRO_ZOUT_L);
	gyroZ =  ((H<<8)+L);
}

unsigned char MPU_ReadRegister(unsigned char addr){
	return iic_mpu_read_data(addr);
}
unsigned char AK8963_ReadRegister(unsigned char addr){
	return AK8963_ReadReg(addr);
}

/**********************磁力计读取***************************/
//i2c_Mag_read(AK8963_ST2_REG) 此步读取不可省略???
//数据读取结束寄存器，reading this register means data reading end
//AK8963_ST2_REG 同时具有数据非正常溢出检测功能
//详情参考 MPU9250 PDF
/**********************************************************/
#define i2c_Mag_read AK8963_ReadReg
void MPU_ReadMag(void)
{
	uint8_t L,H;
	AK8963_WriteReg(AK8963_CNTL1_REG,0x16);
	Delay200us();
	L=i2c_Mag_read(MAG_XOUT_L);
	H=i2c_Mag_read(MAG_XOUT_H);
	magX = ((H<<8)+L);
	L=i2c_Mag_read(MAG_YOUT_L);
	H=i2c_Mag_read(MAG_YOUT_H);
	magY = ((H<<8)+L);
	L=i2c_Mag_read(MAG_ZOUT_L);
	H=i2c_Mag_read(MAG_ZOUT_H);
	magZ = ((H<<8)+L);
	ASAX=i2c_Mag_read(AK8963_ASAX);// X轴灵敏度调整值
	ASAY=i2c_Mag_read(AK8963_ASAY);
	ASAZ=i2c_Mag_read(AK8963_ASAZ);		       
}

