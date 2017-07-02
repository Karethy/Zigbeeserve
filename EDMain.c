#include "CollectTem.h"
#include "SysSet.h"
#include "RF_TR.h"

#define tid 'C'

float avgtemp;
char tempvalue[10]; 

static char buf[128]={"0"};
static int len=0;

void main(void)
{  
  EA = 0;
  initClock();
  InitLed();
  rf_init();
  EA =1 ;
  
  ATEST = 0x01;//�����¶ȴ�����
  TR0 = 0X01;  //���¶ȴ�������ADC��������
  while(1)
  {
    avgtemp = getTemperature();//ȡ���¶��� 
    avgtemp = avgtemp * 2.0;
    tempvalue[0] = 'W';
    tempvalue[1] = 'y';
    tempvalue[2] = 'Z';
    tempvalue[3] = (unsigned char)(avgtemp)/10 + 48;       //ʮλ
    tempvalue[4] = (unsigned char)(avgtemp)%10 + 48;      //��λ
    tempvalue[5] = '.';                                       //С���� 
    tempvalue[6] = (unsigned char)(avgtemp*10)%10+48;     //ʮ��λ
    tempvalue[7] = (unsigned char)(avgtemp*100)%10+48;     //�ٷ�λ
    tempvalue[8] = tid;
    tempvalue[9] = '\n';                                      //�ַ���������
    tx(tempvalue,10);
    Delay(50000);                      //��ʱ
    avgtemp = avgtemp / 2.0;
    if(avgtemp <= 26.00)
    {
      LED1 = 1;
    }
    else if(avgtemp >= 28.10)
    {
      LED1 = 0;
    }
    else
    {
      LED1 = ~LED1;
    }
    Delay1s(1);
  }
}




//�����жϴ���
#pragma vector=RF_VECTOR
__interrupt void rf_isr(void) 
{
  unsigned char  i; 
  int rssi=0;
  char crc_ok=0;
//  float avetemp=0;
  EA=0;
  //���ж�
  IEN2 &= ~0X01;
  //����֡����
  if (RFIRQF0 & (1<<6)) 
  {
    //����֡����
    len = RFD ;
    //printf("\nlen = %d\n***********\n",len);
    len &= 0x7f;
    //�����յ�����д��buf��
    for (i = 0; i < len - 2; i++) 
    {
      buf[i] = RFD;
      Delay(200);
    }
    rssi = RFD - 73;  //��ȡRSSI���  
    crc_ok = RFD;
    printf("[%d],crcResult=%d\n",rssi,(crc_ok&0x80));////crc_ok&0x80��ȡCRCУ���� BIT7      
    
    if((buf[0]=='1')&&(buf[1]=='1'))
    {
      LED1 = 0;
      Delay1s(5);
      
    }
    if((buf[0]=='0')&&(buf[1]=='0'))
    {
      LED1 = 1;
      
      Delay1s(5);
      
    }
    
    
    RFST = 0xED;
    // ��RF�ж�
    S1CON = 0;
    //�� RXPKTDONE�ж�  
    RFIRQF0 &= ~(1<<6);
  }
  IEN2 |= (1<<0);
  EA=1;
  
}