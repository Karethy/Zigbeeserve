#include "SysSet.h"

void Delay(uint n)
{
    uint i,t;
    for(i = 0;i<5;i++)
    for(t = 0;t<n;t++);
}

void Delay1s(uint n)
{
  uint i;
  for(i = 0;i<n;i++)
  {
    Delay(50000);
    Delay(50000);
    Delay(50000);
    Delay(50000);
  }
}

void InitLed(void)
{
    P1DIR |= 0x03;   //P1_1定义为输出
    LED1 = 1;       //LED1灯熄灭
    LED2 = 1;
}

void initClock()
{
  CLKCONCMD &= ~0x40;
  while(CLKCONSTA & 0x40);
  CLKCONCMD  &=  ~0x47;
  
  SLEEPCMD |= 0X40;
}


