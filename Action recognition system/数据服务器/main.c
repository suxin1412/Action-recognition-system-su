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
#include "SBServer.h"
#include "TFXClient.h"

pthread_t pthread_Bluetooth, pthread_HttpServer, pthread_TFXServer;

void ExitServer(int sig){
	void *p;
	SBServer_running = 0;
	SuBand_running = 0;
	TFX_running = 0;
	pthread_join(pthread_HttpServer, &p);
	usleep(10000);
	exit(0);
}

int main(int argc, char *argv[]){
	if (SuBand_Connect()==0) {
		printf("Connect Error.");
		return -1;
	}
	if (SBServer_init()==-1){
		return -1;
	}
	printf("Running.");
	signal(SIGINT,ExitServer);
	pthread_create(&pthread_Bluetooth, NULL, SuBand_runProcess, NULL);
	pthread_create(&pthread_HttpServer, NULL, SBServer_runProces, NULL);
	pthread_create(&pthread_TFXServer, NULL, TFXClient_collect, NULL);
	while (1){
		// printf("%d %d %d %d %d %d %d %d %d \n",MPU_data.accX,MPU_data.accY,MPU_data.accZ,
		// 										MPU_data.gyroX,MPU_data.gyroY,MPU_data.gyroZ,
		// 										MPU_data.magX,MPU_data.magY,MPU_data.magZ);
		usleep(500*1000);
	}
}