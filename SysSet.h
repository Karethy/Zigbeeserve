#include <ioCC2530.h>
#include <stdio.h>
#include <string.h>

#define LED1 P1_1	//����LED3ΪP11�ڿ���
#define LED2 P1_0
#define uchar unsigned char
#define uint unsigned int

void Delay(uint n);
void Delay1s(uint n);
void InitLed(void);
void initClock();