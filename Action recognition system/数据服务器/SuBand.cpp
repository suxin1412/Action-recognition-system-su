#include <sys/types.h>		//系统常用类型定义
#include <sys/stat.h>		//系统常用文件状态定义
#include <fcntl.h>			//文件控制相关
#include <unistd.h>			//POSIX常用系统调用
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>		//POSIX线程管理
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdint.h>
#include "SuBand.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

enum SuBandStateType{
    Disconnected,
    OpenFailed,
    Connected
};
enum SuBandStateType SuBandState = Disconnected;
int fd;
struct MPU_dataType MPU_data;
struct MPU_dataType MPU_filtrationData, MPU_filtrationData_;
double volatile MPU_ACCFilter = 1;
double volatile MPU_GYRFilter = 1;
double volatile MPU_MAGFilter = 1;
int volatile recerdFlag = 0;
int recerdFlag_ = 0;
volatile int SuBand_running = 1;

int SuBand_Connect(){
	fd = open("/dev/rfcomm0",O_RDWR|O_NOCTTY|O_NDELAY);
	if (fd<0) {
		SuBandState = OpenFailed;
		return 0;
	}
	struct termios Opt;
	memset(&Opt ,0 ,sizeof(Opt));
	tcgetattr(fd, &Opt);
	cfsetispeed(&Opt,B115200);     /*设置为115200Bps*/
	cfsetospeed(&Opt,B115200);
	Opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); //特殊字符不做特殊处理
	Opt.c_oflag &= ~OPOST;
	Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	Opt.c_cflag |= CLOCAL | CREAD;//保证程序不会成为端的占有者|使端口能读取输入的数据
	Opt.c_cflag &= ~CSIZE;
	Opt.c_cflag |= CS8;		//8数据位
	Opt.c_cflag &= ~PARENB; //无校验
	Opt.c_cflag &= ~CSTOPB; //一停止位
	Opt.c_cc[VTIME] = 10;/* 非规范模式读取时的超时时间*/
	Opt.c_cc[VMIN]  = 128; /* 非规范模式读取时的最小字符数*/
	tcflush(fd ,TCIFLUSH);/* tcflush清空终端未完成的输入/输出请求及数据*/
	tcsetattr(fd,TCSANOW,&Opt);
	SuBandState = Connected;
	
	return 1;
}
void HexViewer(uint8_t *buf, int rc){
    if (rc<=0) return;
	int i = 0;
	while (i<rc){
		printf("%02X ",buf[i++]);
	}
	printf("\n");
}
void *SuBand_runProcess(void *arg){
	//int fd_accX,fd_accY,fd_accZ,fd_gyroX,fd_gyroY,fd_gyroZ,fd_magX,fd_magY,fd_magZ,fd_flag;
	ofstream fd_accX,fd_accY,fd_accZ,fd_gyroX,fd_gyroY,fd_gyroZ,fd_magX,fd_magY,fd_magZ,fd_flag;
	int rdCount = 0;
	uint8_t bufTemp[24];
	uint8_t dis=0;
	cout << "SuBand Running." << endl;
	while (SuBand_running){
        rdCount = read(fd, bufTemp, 24);
		//数据帧校验
        if (rdCount < 24) continue;
		//HexViewer(bufTemp, 24);
		if (bufTemp[0]!=0xa5 || bufTemp[23]!=0x5a) continue;
		memcpy(&MPU_filtrationData_, &MPU_filtrationData, 24);
		memcpy(&MPU_filtrationData, bufTemp, 24);
		MPU_filtrationData.accX = MPU_ACCFilter * MPU_filtrationData.accX + (1-MPU_ACCFilter) * MPU_filtrationData_.accX;
		MPU_filtrationData.accY = MPU_ACCFilter * MPU_filtrationData.accY + (1-MPU_ACCFilter) * MPU_filtrationData_.accY;
		MPU_filtrationData.accZ = MPU_ACCFilter * MPU_filtrationData.accZ + (1-MPU_ACCFilter) * MPU_filtrationData_.accZ;
		MPU_filtrationData.gyroX = MPU_GYRFilter * MPU_filtrationData.gyroX + (1-MPU_GYRFilter) * MPU_filtrationData_.gyroX;
		MPU_filtrationData.gyroY = MPU_GYRFilter * MPU_filtrationData.gyroY + (1-MPU_GYRFilter) * MPU_filtrationData_.gyroY;
		MPU_filtrationData.gyroZ = MPU_GYRFilter * MPU_filtrationData.gyroZ + (1-MPU_GYRFilter) * MPU_filtrationData_.gyroZ;
		MPU_filtrationData.magX = MPU_MAGFilter * MPU_filtrationData.magX + (1-MPU_MAGFilter) * MPU_filtrationData_.magX;
		MPU_filtrationData.magY = MPU_MAGFilter * MPU_filtrationData.magY + (1-MPU_MAGFilter) * MPU_filtrationData_.magY;
		MPU_filtrationData.magZ = MPU_MAGFilter * MPU_filtrationData.magZ + (1-MPU_MAGFilter) * MPU_filtrationData_.magZ;
		//usleep(100000);
		if (dis<1)
			printf("%5d %5d %5d %5d %5d %5d %5d %5d %5d %0.4f %0.4f %0.4f\n\033[1A",MPU_filtrationData.accX,MPU_filtrationData.accY,MPU_filtrationData.accZ,
										MPU_filtrationData.gyroX,MPU_filtrationData.gyroY,MPU_filtrationData.gyroZ,
										MPU_filtrationData.magX,MPU_filtrationData.magY,MPU_filtrationData.magZ,
										MPU_ACCFilter,MPU_GYRFilter,MPU_MAGFilter);
		dis++;
		if (dis>7) dis=0;
		if (recerdFlag!=0&&recerdFlag_==0){ //开始录制，打开文件
			// fd_accX = open("./dataset/accX.txt",O_WRONLY|O_APPEND);
			// fd_flag = open("./dataset/flag.txt",O_WRONLY|O_APPEND);
			recerdFlag_ = recerdFlag;
			fd_accX.open("./dataset/accX.txt", ofstream::app|ofstream::out);
			fd_accY.open("./dataset/accY.txt", ofstream::app|ofstream::out);
			fd_accZ.open("./dataset/accZ.txt", ofstream::app|ofstream::out);
			fd_gyroX.open("./dataset/gyroX.txt", ofstream::app|ofstream::out);
			fd_gyroY.open("./dataset/gyroY.txt", ofstream::app|ofstream::out);
			fd_gyroZ.open("./dataset/gyroZ.txt", ofstream::app|ofstream::out);
			fd_magX.open("./dataset/magX.txt", ofstream::app|ofstream::out);
			fd_magY.open("./dataset/magY.txt", ofstream::app|ofstream::out);
			fd_magZ.open("./dataset/magZ.txt", ofstream::app|ofstream::out);
			fd_flag.open("./dataset/label.txt", ofstream::app|ofstream::out);
			fd_flag << recerdFlag;
		}else if (recerdFlag==0&&recerdFlag_!=0){//录制结束，关闭文件
			recerdFlag_ = 0;
			fd_accX << endl;
			fd_accY << endl;
			fd_accZ << endl;
			fd_gyroX << endl;
			fd_gyroY << endl;
			fd_gyroZ << endl;
			fd_magX << endl;
			fd_magY << endl;
			fd_magZ << endl;
			fd_flag << endl;
			fd_accX.close();
			fd_accY.close();
			fd_accZ.close();
			fd_gyroX.close();
			fd_gyroY.close();
			fd_gyroZ.close();
			fd_magX.close();
			fd_magY.close();
			fd_magZ.close();
			fd_flag.close();
		}else if (recerdFlag_ != recerdFlag){ // 录制动作改变，需要换行
			recerdFlag_ = recerdFlag;
			fd_accX << endl;
			fd_accY << endl;
			fd_accZ << endl;
			fd_gyroX << endl;
			fd_gyroY << endl;
			fd_gyroZ << endl;
			fd_magX << endl;
			fd_magY << endl;
			fd_magZ << endl;
			fd_flag << endl;
			fd_flag << recerdFlag;
		}else{//正常录制
			fd_accX << MPU_filtrationData.accX << " ";
			fd_accY << MPU_filtrationData.accY << " ";
			fd_accZ << MPU_filtrationData.accZ << " ";
			fd_gyroX << MPU_filtrationData.gyroX << " ";
			fd_gyroY << MPU_filtrationData.gyroY << " ";
			fd_gyroZ << MPU_filtrationData.gyroZ << " ";
			fd_magX << MPU_filtrationData.magX << " ";
			fd_magY << MPU_filtrationData.magY << " ";
			fd_magZ << MPU_filtrationData.magZ << " ";
		}
	}
	return NULL;
}

// /*串口初始化函数
//  *@method Usart_Init
//  *@param{void}
//  *@return {void}
// */
// void Usart_Init()
// {
// 	struct termios Opt;
// 	memset(&Opt ,0 ,sizeof(Opt));
// 	tcgetattr(fd, &Opt);
// 	cfsetispeed(&Opt,B115200);     /*设置为115200Bps*/
// 	cfsetospeed(&Opt,B115200);
// 	Opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); //特殊字符不做特殊处理
// 	Opt.c_oflag &= ~OPOST;
// 	Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
// 	Opt.c_cflag |= CLOCAL | CREAD;//保证程序不会成为端的占有者|使端口能读取输入的数据
// 	Opt.c_cflag &= ~CSIZE;
// 	Opt.c_cflag |= CS8;		//8数据位
// 	Opt.c_cflag &= ~PARENB; //无校验
// 	Opt.c_cflag &= ~CSTOPB; //一停止位
// 	Opt.c_cc[VTIME] = 10;/* 非规范模式读取时的超时时间*/
// 	Opt.c_cc[VMIN]  = 128; /* 非规范模式读取时的最小字符数*/
// 	tcflush(fd ,TCIFLUSH);/* tcflush清空终端未完成的输入/输出请求及数据*/
// 	tcsetattr(fd,TCSANOW,&Opt);
// }

// void HexViewer(uint8_t *buf, int rc){
//     if (rc<=0) return;
// 	int i = 0;
// 	while (i<rc){
// 		printf("%02X ",buf[i++]);
// 	}
// 	printf("\n");
// }

// int main(int argc, char *argv[]){
// 	fd = open("/dev/rfcomm0",O_RDWR|O_NOCTTY|O_NDELAY);
// 	if (fd<0) return fd;
// 	Usart_Init();
//     int cn = 0;
//     uint8_t buf[24];
//     while (1){
//         cn = read(fd, buf, 24);
//         if (cn < 24) continue;
//         printf("Read Count:%d\n", cn);
//         HexViewer(buf, cn);
//     }
// }