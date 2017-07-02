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
  //关闭总中断
  EA = 0;
  InitLed();
  initClock();
  initUARTSEND();
  InitUart();
  //初始化RF
  rf_init();
  //中断使能
  EA = 1; 
  //发送或等待接收中断
  
  

  while(1) 
  {
        if(RT_flag == 1)			//接收
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
    
        if(RT_flag == 3)			 //led状态设置
        {
            U0CSR &= ~0x40;		         //禁止接收
            receive_buf[2] = '\0'; 
            
            txC(receive_buf,3);
            
            U0CSR |= 0x40;		        //允许接收
            RT_flag = 1;		        //恢复到接收状态
            counter = 0;		//指针归0
        }
  } 
}


#pragma vector = URX0_VECTOR
 __interrupt void UART0_R_ISR(void)
{
    URX0IF = 0;				//清中断标志
    temp = U0DBUF;
    //inInterrupt++;
}

#pragma vector = UTX0_VECTOR
 __interrupt void UART0_T_ISR(void)
{
  IEN2 &=~ 0x04;//发送中断使能
  //U0CSR |= 0x02;
  //UTX0IF = 0;				//清中断标志 
  IEN2 |= 0x04;//发送中断使能
}

//接收中断处理
#pragma vector=RF_VECTOR
__interrupt void rf_isr(void) 
{
  unsigned char  i; 
  int rssi=0;
  char crc_ok=0;
//  float avetemp=0;
  EA=0;
  //关中断
  IEN2 &= ~0X01; 
  //接收帧结束
  if (RFIRQF0 & (1<<6)) 
  {
    //接收帧长度
    len = RFD ;
    //printf("\nlen = %d\n***********\n",len);
    len &= 0x7f;
    //将接收的数据写入buf中
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
      rssi = RFD - 73;  //读取RSSI结果  
      crc_ok = RFD;
      printf("[%d],crcResult=%d\n",rssi,(crc_ok&0x80));//crc_ok&0x80读取CRC校验结果 BIT7      
      for(i = 0;i < (len - (2 + 3));i++)
      {
        buf[i]=buf[i+3];
      }
      //向串口发送接收到的数据
      UartTX_Send_String(buf,len-(2 + 3)); 
    }
      
    
    
    RFST = 0xED;
    // 清RF中断
    S1CON = 0; 
    //清 RXPKTDONE中断  
    RFIRQF0 &= ~(1<<6); 
  }
  IEN2 |= (1<<0);
  EA=1;
}
