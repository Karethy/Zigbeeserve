#include "CollectTem.h"
#include "SysSet.h"


float getTemperature()
{
  uint value;
  ADCCON3 = 0x3e; //ѡ��1.25VΪ�ο���ѹ��14λ�ֱ��ʣ���Ƭ���¶ȴ���������
  ADCCON1 |= 0x30;//ADC������ʽѡ��ΪADCCON1.ST=1�¼�
  ADCCON1 |= 0x40;//ADC����ת��
  
  while(!(ADCCON1 & 0x80))//�ȴ�ADת�����
    ;
  value = ADCL >>4;/*ADCL�Ĵ�����4λ��Ч��ADC���ֻ�ܴﵽ12λ��Чλ�������Ϻܶ�������ﶼ��������λ�����ǲ��Ե�*/
  value |= (((uint)ADCH)<<4);//����ADCH��ADCL������ֵ��value
  return (value - 1367.5)/4.5 - 15;
}