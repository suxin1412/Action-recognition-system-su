#ifndef _TFXCLIENT_H_
#define _TFXCLIENT_H_


extern volatile int TFX_status;
extern volatile double TFX_results[8];
extern volatile int TFX_running;
extern char TFX_EchoData[600];

void *TFXClient_collect(void *arg);

#endif