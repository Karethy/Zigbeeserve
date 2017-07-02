#include "Time.h"

void timer1_init()
{
    T1CTL = 0x0C;         // @DIV分频系数 128 @MODE暂停运行
    T1CCTL0 = 0x44;       // @IM通道0中断使能 @MODE比较匹配模式
    T1STAT = 0x00;        // 清除所有中断标志

    T1IE = 1;             // IEN1@BIT1 使能定时器1中断

    T1CC0L = 0xA8;         // 溢出周期为200ms
    T1CC0H = 0x00;
}
void timer1_disbale()
{
    T1CTL &= ~( 1<< 1);   // 恢复为停止模式
}
void timer1_enable()
{
    T1CTL |= ( 1 << 1 );  // 改变模式为比较匹配模式 MODE = 0x10;
    T1STAT = 0x00;        // 清除中断标志位
    T1CNTH = 0;           // 重新开始计数
    T1CNTL = 0;
}
