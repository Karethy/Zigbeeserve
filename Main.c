#include "CollectTem.h"
#include "Uart.h"
#include "SysSet.h"

char tempvalue[10]; 

void main(void)
{
  
  initClock();
  InitUart();
  InitLed();
  
  ATEST = 0x01;//�����¶ȴ�����
  TR0 = 0X01;  //���¶ȴ�������ADC��������
  while(1)
  {
    
    float avgtemp = getTemperature();//ȡ���¶��� 
    tempvalue[0] = (unsigned char)(avgtemp)/10 + 48;          //ʮλ
    tempvalue[1] = (unsigned char)(avgtemp)%10 + 48;          //��λ
    tempvalue[2] = '.';                                       //С���� 
    tempvalue[3] = (unsigned char)(avgtemp*10)%10+48;         //ʮ��λ
    tempvalue[4] = (unsigned char)(avgtemp*100)%10+48;        //�ٷ�λ
    tempvalue[5] = '\n';
    UartTX_Send_String(tempvalue,6);
    Delay(50000);                      //��ʱ
    LED1=!LED1;                        //��־����״̬
    Delay(50000);
    Delay(50000);
  }
}