#include "Time.h"

void timer1_init()
{
    T1CTL = 0x0C;         // @DIV��Ƶϵ�� 128 @MODE��ͣ����
    T1CCTL0 = 0x44;       // @IMͨ��0�ж�ʹ�� @MODE�Ƚ�ƥ��ģʽ
    T1STAT = 0x00;        // ��������жϱ�־

    T1IE = 1;             // IEN1@BIT1 ʹ�ܶ�ʱ��1�ж�

    T1CC0L = 0xA8;         // �������Ϊ200ms
    T1CC0H = 0x00;
}
void timer1_disbale()
{
    T1CTL &= ~( 1<< 1);   // �ָ�Ϊֹͣģʽ
}
void timer1_enable()
{
    T1CTL |= ( 1 << 1 );  // �ı�ģʽΪ�Ƚ�ƥ��ģʽ MODE = 0x10;
    T1STAT = 0x00;        // ����жϱ�־λ
    T1CNTH = 0;           // ���¿�ʼ����
    T1CNTL = 0;
}
