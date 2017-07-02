#include "CollectTem.h"
#include "Uart.h"
#include "SysSet.h"
#include "RF_TR.h"

uchar temp;
int inInterrupt=0;

static  char buf[128]={"0"};
static  int len=0;

void main(void)
{
  char receive_buf[3];
  uchar counter =0;
  uchar RT_flag=1;
  //�ر����ж�
  EA = 0;
  InitLed();
  initClock();
  initUARTSEND();
  InitUart();
  //��ʼ��RF
  rf_init();
  //�ж�ʹ��
  EA = 1; 
  //���ͻ�ȴ������ж�
  
  

  while(1) 
  {
        if(RT_flag == 1)			//����
        {         
          if( temp != 0)
          {  
            if((temp!='#')&&(counter<3))
              {                                                     
                receive_buf[counter++] = temp;
              }
              else
              {
                RT_flag = 3;             
              }
              if(counter == 3)
              {
                RT_flag = 3;
                temp = 0;
              }
          }
        }
    
        if(RT_flag == 3)			 //led״̬����
        {
            U0CSR &= ~0x40;		         //��ֹ����
            receive_buf[2] = '\0'; 
            
            txC(receive_buf,3);
            
            U0CSR |= 0x40;		        //�������
            RT_flag = 1;		        //�ָ�������״̬
            counter = 0;		//ָ���0
        }
  } 
}


#pragma vector = URX0_VECTOR
 __interrupt void UART0_R_ISR(void)
{
    URX0IF = 0;				//���жϱ�־
    temp = U0DBUF;
    //inInterrupt++;
}

#pragma vector = UTX0_VECTOR
 __interrupt void UART0_T_ISR(void)
{
  IEN2 &=~ 0x04;//�����ж�ʹ��
  //U0CSR |= 0x02;
  //UTX0IF = 0;				//���жϱ�־ 
  IEN2 |= 0x04;//�����ж�ʹ��
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
//    if((buf[0]=='W')&&(buf[1]=='y')&&(buf[2]=='Z'))

    for (i = 0; i < 3; i++) 
    {
      buf[i] = RFD;
      Delay(200);
    }
    if((buf[0]=='W')&&(buf[1]=='y')&&(buf[2]=='Z'))
    {
      for (i = 3; i < (len - (2 + 3)); i++)
      {
        buf[i] = RFD;
        Delay(200);
      }
      rssi = RFD - 73;  //��ȡRSSI���  
      crc_ok = RFD;
      printf("[%d],crcResult=%d\n",rssi,(crc_ok&0x80));//crc_ok&0x80��ȡCRCУ���� BIT7      
      for(i = 0;i < (len - (2 + 3));i++)
      {
        buf[i]=buf[i+3];
      }
      //�򴮿ڷ��ͽ��յ�������
      UartTX_Send_String(buf,len-(2 + 3)); 
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
