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
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <sstream>
#include <iostream>
#include "TFXClient.h"

using namespace std;
using namespace rapidjson;

volatile int TFX_status = 0;
// 0 no status
// 1 start collect
// 2 collecting
// 3 collect over
// 4 Query
volatile double TFX_results[8];
volatile int TFX_running = 1;
Document doc;

int sockfd;
int arg_0_1 = 0;
char TFX_EchoData[600];
string TFX_FrameHand = "POST /v1/models/SuBandv2:predict HTTP/1.1\r\nContent-Length: ";
string TFX_FrameContentLen("0");
string TFX_Content("{\"inputs\": {\"args_0\": [");


ostringstream TFX_ContentInStream("");

void TFXClient_RequestRes(){
    struct sockaddr_in address;
    int result;
    string res;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(8501);
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    bzero(&(address.sin_zero), 8);
    result = connect(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr));
    if(result == -1) {
        perror("oops: client1");
        exit(1);
    }
    int slen = write(sockfd, TFX_FrameHand.c_str(), TFX_FrameHand.size());
    printf("write %d  Total%d  ConSize%s.\n", slen, TFX_FrameHand.size(), TFX_FrameContentLen.c_str());
    memset(TFX_EchoData, 0, 600);
    slen = read(sockfd, TFX_EchoData, 600);
    cout << "Readed Size:" << slen << endl;
    close(sockfd);
}

void *TFXClient_collect(void *arg){
    while (TFX_running){
        if (TFX_status==0) continue;
        else if (TFX_status==1){
            TFX_ContentInStream<<"{\"inputs\": {\"args_0\": [["
                <<MPU_filtrationData.accX<<","<<MPU_filtrationData.accY<<","<<MPU_filtrationData.accZ<<","
                <<MPU_filtrationData.gyroX<<","<<MPU_filtrationData.gyroY<<","<<MPU_filtrationData.gyroZ<<"]";
            arg_0_1 = 1;
            TFX_status = 3;
        }else if (TFX_status==3){
            TFX_ContentInStream<<",["
                <<MPU_filtrationData.accX<<","<<MPU_filtrationData.accY<<","<<MPU_filtrationData.accZ<<","
                <<MPU_filtrationData.gyroX<<","<<MPU_filtrationData.gyroY<<","<<MPU_filtrationData.gyroZ<<"]";
            usleep(1000*100);
            ++arg_0_1;
        }else if (TFX_status==4){
            TFX_ContentInStream<<"],\"args_0_1\":[0,"<< arg_0_1 <<"]}}";
            TFX_Content = TFX_ContentInStream.str();
            TFX_ContentInStream.str("");
            cout << TFX_Content;
            TFX_FrameContentLen = to_string(TFX_Content.size());
            TFX_FrameHand += TFX_FrameContentLen + "\r\n\r\n" + TFX_Content;
            TFXClient_RequestRes();
            TFX_FrameHand = "POST /v1/models/SuBandv2:predict HTTP/1.1\r\nContent-Length: ";
            //cout << TFX_EchoData;
            cout << "******\n" << strstr(TFX_EchoData, "\r\n\r\n") << endl;
            doc.Parse(strstr(TFX_EchoData, "\r\n\r\n"));
            const Value& outputs = doc["outputs"][0];
            for (SizeType i = 0; i < outputs.Size(); i++){ // 使用 SizeType 而不是 size_t
                printf("outputs[%d] = %f\n", i, outputs[i].GetDouble());
                TFX_results[i] = outputs[i].GetDouble();
            }
            TFX_status = 5;
        }
    }
    return NULL;
    // exit(0) ;
}



