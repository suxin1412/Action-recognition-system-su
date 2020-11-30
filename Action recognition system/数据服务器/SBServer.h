#ifndef _SBServer_H_
#define _SBServer_H_

extern int SBServer_running;
int SBServer_init();
void *SBServer_runProces(void *args);
#endif