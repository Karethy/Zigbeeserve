#include "Uart.h"
#include "SysSet.h"

void InitUart()
{
    CLKCONCMD &= ~0x40;
    while(CLKCONSTA & 0x40);
    CLKCONCMD &= ~0x47;

    PERCFG = 0x00;
    P0SEL = 0x3c;
    P2DIR &= ~0XC0;

    U0CSR |= 0x80;
    U0GCR |= 11;
    U0BAUD |= 216;//��������Ϊ115200
    UTX0IF = 1;

    
    IEN0 |= 0x80;
    IEN2 |= 0x04;
    IEN0 |= 0x04;
    URX0IE = 1;
    U0CSR |= 0X40;
}

void UartTX_Send_String(char *Data,int len)
{
  int j;
  for(j=0;j<len;j++)
  { 
    U0DBUF = *Data++;
    while(UTX0IF == 0) //UTX0IF����1�������жϣ�����һֱѭ��
      ;
    UTX0IF = 0;
  }
}

void initUARTSEND(void)
{
    PERCFG = 0x00;        //λ�� 1 P0 ��
    P0SEL = 0x3c;        //P0_2,P0_3,P0_4,P0_5��������
    P2DIR &= ~0xc0;        //P0 ������ΪUART0
    U0CSR |= 0x80;        //ѡ�� UART ģʽ
    U0GCR |= 11;                // ����� U0GCR �� U0BAUD
    U0BAUD = 216;               // 115200
    UTX0IF = 0;        //UART0 TX �жϱ�־��ʼ��λ 0
}