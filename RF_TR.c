#include "RF_TR.h"
#include "SysSet.h"


void rf_init()
{
  //Ӳ��CRC�Լ�AUTO_ACKʹ��
  FRMCTRL0 |= (0x20 | 0x40);

  //����TX������������Ի�ú��ʵĴ���
  TXFILTCFG = 0x09;
  //����AGCĿ��ֵ
  AGCCTRL1 = 0x15;
  //�����ѵ�EVM
  FSCAL1 = 0x00;
  // RXPKTDONE �ж�λʹ��
  RFIRQM0 |= (1<<6);
  //  RF �ж�ʹ��
  IEN2 |= (1<<0);
  //���ж�
  EA = 1;
  //�ŵ�ѡ��ѡ��11�ŵ�
  FREQCTRL = 0x0b; 
  //Ŀ���ַ�����ڼ�ʹ�õĶ̵�ַ
  SHORT_ADDR0 = 0x05;
  SHORT_ADDR1 = 0x00;
  //Ŀ���ַ�����ڼ�ʹ�õ�PANID
  PAN_ID0 = 0x22; 
  PAN_ID1 = 0x00;
  //���RXFIFO����������λ�����  
  RFST = 0xed; 
  //ΪRXʹ�ܲ�У׼Ƶ�ʺϳ���
  RFST = 0xe3; 
  //��ֹ֡����
  FRMFILT0 &= ~(1<<0);
}

void tx(char *tx,int len)
{
  unsigned char i; 
  //ΪRXʹ�ܲ�У׼Ƶ�ʺϳ���
  RFST = 0xe3;  
  // TX_ACTIVE | SFD 
  while (FSMSTAT1 & ((1<<1) | (1<<5))); 
  //��ֹRXPKTDONE�ж�
  RFIRQM0 &= ~(1<<6); 
  //��ֹRF�ж�
  IEN2 &= ~(1<<0); 
  // ���TXFIFO���� 
  RFST = 0xee; 
  // ��� TXDONE �ж� 
  RFIRQF1 = ~(1<<1);
  // ���͵ĵ�һ���ֽ��Ǵ����֡���� 
  RFD = len+2;//tx�а�����13���ַ�+2;��仺������������Ҫ����2�ֽڣ�CRCУ���Զ���� 
//��mac������д��RFD��
  for(i=0;i<len;i++)
  {
    RFD = tx[i];
  }
  // ��RX�ж� 
  RFIRQM0 |= (1<<6);
  //��RF�ж�
  IEN2 |= (1<<0);
  //У׼��ʹ��TX 
  RFST = 0xe9; 
  //�ȴ��������
  while (!(RFIRQF1 & (1<<1)));
  //��� TXDONE״̬  
  RFIRQF1 = ~(1<<1);
  Delay(20000);
}

void txC(char *value,int length)
{
  unsigned char i; 
  //ΪRXʹ�ܲ�У׼Ƶ�ʺϳ���
  RFST = 0xe3;  
  // TX_ACTIVE | SFD 
  while (FSMSTAT1 & ((1<<1) | (1<<5))); 
  //��ֹRXPKTDONE�ж�
  RFIRQM0 &= ~(1<<6); 
  //��ֹRF�ж�
  IEN2 &= ~(1<<0); 
  // ���TXFIFO���� 
  RFST = 0xee; 
  // ��� TXDONE �ж� 
  RFIRQF1 = ~(1<<1);
  // ���͵ĵ�һ���ֽ��Ǵ����֡����  
  RFD = length+2;//tx�а�����13���ַ�+2;��仺������������Ҫ����2�ֽڣ�CRCУ���Զ���� 
//��mac������д��RFD��
  for(i=0;i<length;i++)
  {
    RFD = value[i];
  }
  // ��RX�ж� 
  RFIRQM0 |= (1<<6);
  //��RF�ж�
  IEN2 |= (1<<0);
  //У׼��ʹ��TX 
  RFST = 0xe9; 
  //�ȴ��������
  while (!(RFIRQF1 & (1<<1)));
  //��� TXDONE״̬  
  RFIRQF1 = ~(1<<1);
  //LED1��״̬�ı�
  LED2=~LED2;
  //��ʱ
  Delay(20000);
}