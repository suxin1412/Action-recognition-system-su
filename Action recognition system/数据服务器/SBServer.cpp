#include <sys/socket.h> //socket
#include <sys/types.h>
#include <netinet/in.h> //ip地址结构体
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "SBServer.h"
#include <string>
#include "SuBand.h"
#include "TFXClient.h"
#include <string>
#include <iostream>
using namespace std;

int socketfd;
int SBServer_running = 1;
char buffer[1024*10];
string HtmlHandBegin("HTTP/1.1 200 OK\r\n\
Access-Control-Allow-Origin: *\r\n\
Content-Type: text/html\r\n\
Accept-Ranges: bytes\r\n\
Cache-Control: no-store, must-revalidate\r\n\
Content-Length: ");
string HtmlHandEnd("\r\n\
Server: SBServer/1.0\r\n\
Date: Thu, 12 Nov 2020 02:35:05 GMT\r\n\
ETag: \"3581539532\"\r\n\
Last-Modified: Sat, 31 Dec 2005 16:00:00 GMT\r\n\
\r\n");
string HtmlBody("hello World.\r\n   -SBServer.");
int client_fd;

int SBServer_init(){
    struct sockaddr_in server_addr, client_addr;
    bzero(&server_addr, sizeof(server_addr)); //清空内存
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2333);
    server_addr.sin_addr.s_addr = inet_addr("10.102.14.92");
    socketfd = socket(AF_INET, SOCK_STREAM, 0); //生成套接字
    int res = bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)); //绑定到socket
    if (res==-1){
        printf("Bind Error!\n");
        return -1;
    }
    listen(socketfd, 100); //监听
    return 0;
}

void SBServer_config(){
    //判断FC 略 12 13 14 15 16 17 18
    //         F  C  =  M  1  2  3
    if (buffer[12]=='F'&&buffer[13]=='C'){
        double fc = 0;
        fc = buffer[16] - '0';
        fc = fc*10 + buffer[17] - '0';
        if (buffer[18]!=' ')
            fc = fc*10 + buffer[18] - '0';
        switch (buffer[15])
        {
        case 'A':
            MPU_ACCFilter = fc/1000.0;
            break;
        case 'G':
            MPU_GYRFilter = fc/1000.0;
            break;
        case 'M':
            MPU_MAGFilter = fc/1000.0;
            break;
        default:
            break;
        }
        //printf("%s\n", buffer);
        HtmlBody = "ok";
    }else if (buffer[12]=='A'&&buffer[13]=='C'){ //录制动作
        int keyCode = buffer[15] - '0';
        keyCode = keyCode*10 + buffer[16] - '0';
        if (keyCode==81)
            recerdFlag = 0;
        else 
            recerdFlag = keyCode - 48;
        HtmlBody = "ok";
    }else if (buffer[12]=='A'&&buffer[13]=='R'){ //动作识别
        if (buffer[15]=='S'){ //开始识别
            TFX_status = 1;
            HtmlBody = "ok";
        }else if (buffer[15]=='E'){ //结束识别
            TFX_status = 4;
            while (TFX_status!=5);
            HtmlBody = strstr(TFX_EchoData, "\r\n\r\n");
            cout << "........." << HtmlBody;
            TFX_status = 0;
        }
    }
    string tmp = HtmlHandBegin+ to_string(HtmlBody.size()) +HtmlHandEnd+HtmlBody;
    write(client_fd, tmp.c_str(), tmp.size());
}

void *SBServer_runProces(void *args){
    char DataBuffer[]="{\"accX\":%d,\"accY\":%d,\"accZ\":%d,\
    \"gyroX\":%d,\"gyroY\":%d,\"gyroZ\":%d,\
    \"magX\":%d,\"magY\":%d,\"magZ\":%d}";
    int readLen;
    while (SBServer_running){
        client_fd = accept(socketfd, NULL, NULL); //阻塞，等待客户端连接
        readLen = read(client_fd, buffer, sizeof(buffer));
        buffer[readLen] = 0;
        if (strstr(buffer, "GET /config?"))
            SBServer_config();
        else{
            sprintf(DataBuffer, "{\"accX\":%d,\"accY\":%d,\"accZ\":%d,\"gyroX\":%d,\"gyroY\":%d,\"gyroZ\":%d,\"magX\":%d,\"magY\":%d,\"magZ\":%d}", 
                MPU_filtrationData.accX, MPU_filtrationData.accY, MPU_filtrationData.accZ, 
                MPU_filtrationData.gyroX,  MPU_filtrationData.gyroY,MPU_filtrationData.gyroZ,
                MPU_filtrationData.magX, MPU_filtrationData.magY, MPU_filtrationData.magZ);
            HtmlBody=DataBuffer;
            string tmp = HtmlHandBegin+ to_string(HtmlBody.size()) +HtmlHandEnd+HtmlBody;
            //printf("%s\n", tmp.c_str());
            write(client_fd, tmp.c_str(), tmp.size());
        }
        close(client_fd);
    }
    int sta = close(socketfd);
    printf("close server. %d", sta);
    fflush(stdout);
    return NULL;
}