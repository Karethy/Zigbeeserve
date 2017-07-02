#include "CollectTem.h"
#include "Uart.h"
#include "SysSet.h"

char tempvalue[10]; 

void main(void)
{
  
  initClock();
  InitUart();
  InitLed();
  
  ATEST = 0x01;//开启温度传感器
  TR0 = 0X01;  //将温度传感器与ADC连接起来
  while(1)
  {
    
    float avgtemp = getTemperature();//取得温度数 
    tempvalue[0] = (unsigned char)(avgtemp)/10 + 48;          //十位
    tempvalue[1] = (unsigned char)(avgtemp)%10 + 48;          //个位
    tempvalue[2] = '.';                                       //小数点 
    tempvalue[3] = (unsigned char)(avgtemp*10)%10+48;         //十分位
    tempvalue[4] = (unsigned char)(avgtemp*100)%10+48;        //百分位
    tempvalue[5] = '\n';
    UartTX_Send_String(tempvalue,6);
    Delay(50000);                      //延时
    LED1=!LED1;                        //标志发送状态
    Delay(50000);
    Delay(50000);
  }
}