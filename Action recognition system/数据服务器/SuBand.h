#ifndef _SUBAND_H_
#define _SUBAND_H_
struct MPU_dataType{
    volatile uint8_t head;
    volatile int8_t asaX, asaY, asaZ;
    volatile int16_t accX, accY, accZ;
    volatile int16_t gyroX, gyroY, gyroZ;
    volatile int16_t magX, magY, magZ;
    volatile uint8_t crc, tail;
};
//uint8_t Buffer[24];
extern struct MPU_dataType MPU_data;
extern struct MPU_dataType MPU_filtrationData;
extern volatile double MPU_ACCFilter ;
extern volatile double MPU_GYRFilter ;
extern volatile double MPU_MAGFilter ;
extern volatile int recerdFlag;
extern volatile int SuBand_running;
int SuBand_Connect();
void *SuBand_runProcess(void *arg);




#endif