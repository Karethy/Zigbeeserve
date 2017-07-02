#include "RF_TR.h"
#include "SysSet.h"


void rf_init()
{
  //硬件CRC以及AUTO_ACK使能
  FRMCTRL0 |= (0x20 | 0x40);

  //设置TX抗混叠过滤器以获得合适的带宽
  TXFILTCFG = 0x09;
  //调整AGC目标值
  AGCCTRL1 = 0x15;
  //获得最佳的EVM
  FSCAL1 = 0x00;
  // RXPKTDONE 中断位使能
  RFIRQM0 |= (1<<6);
  //  RF 中断使能
  IEN2 |= (1<<0);
  //开中断
  EA = 1;
  //信道选择，选择11信道
  FREQCTRL = 0x0b; 
  //目标地址过滤期间使用的短地址
  SHORT_ADDR0 = 0x05;
  SHORT_ADDR1 = 0x00;
  //目标地址过滤期间使用的PANID
  PAN_ID0 = 0x22; 
  PAN_ID1 = 0x00;
  //清除RXFIFO缓冲区并复位解调器  
  RFST = 0xed; 
  //为RX使能并校准频率合成器
  RFST = 0xe3; 
  //禁止帧过滤
  FRMFILT0 &= ~(1<<0);
}

void tx(char *tx,int len)
{
  unsigned char i; 
  //为RX使能并校准频率合成器
  RFST = 0xe3;  
  // TX_ACTIVE | SFD 
  while (FSMSTAT1 & ((1<<1) | (1<<5))); 
  //禁止RXPKTDONE中断
  RFIRQM0 &= ~(1<<6); 
  //禁止RF中断
  IEN2 &= ~(1<<0); 
  // 清除TXFIFO缓存 
  RFST = 0xee; 
  // 清除 TXDONE 中断 
  RFIRQF1 = ~(1<<1);
  // 发送的第一个字节是传输的帧长度 
  RFD = len+2;//tx中包含的13个字符+2;填充缓冲区填充过程需要增加2字节，CRC校验自动填充 
//将mac的内容写到RFD中
  for(i=0;i<len;i++)
  {
    RFD = tx[i];
  }
  // 打开RX中断 
  RFIRQM0 |= (1<<6);
  //打开RF中断
  IEN2 |= (1<<0);
  //校准后使能TX 
  RFST = 0xe9; 
  //等待传输结束
  while (!(RFIRQF1 & (1<<1)));
  //清除 TXDONE状态  
  RFIRQF1 = ~(1<<1);
  Delay(20000);
}

void txC(char *value,int length)
{
  unsigned char i; 
  //为RX使能并校准频率合成器
  RFST = 0xe3;  
  // TX_ACTIVE | SFD 
  while (FSMSTAT1 & ((1<<1) | (1<<5))); 
  //禁止RXPKTDONE中断
  RFIRQM0 &= ~(1<<6); 
  //禁止RF中断
  IEN2 &= ~(1<<0); 
  // 清除TXFIFO缓存 
  RFST = 0xee; 
  // 清除 TXDONE 中断 
  RFIRQF1 = ~(1<<1);
  // 发送的第一个字节是传输的帧长度  
  RFD = length+2;//tx中包含的13个字符+2;填充缓冲区填充过程需要增加2字节，CRC校验自动填充 
//将mac的内容写到RFD中
  for(i=0;i<length;i++)
  {
    RFD = value[i];
  }
  // 打开RX中断 
  RFIRQM0 |= (1<<6);
  //打开RF中断
  IEN2 |= (1<<0);
  //校准后使能TX 
  RFST = 0xe9; 
  //等待传输结束
  while (!(RFIRQF1 & (1<<1)));
  //清除 TXDONE状态  
  RFIRQF1 = ~(1<<1);
  //LED1灯状态改变
  LED2=~LED2;
  //延时
  Delay(20000);
}