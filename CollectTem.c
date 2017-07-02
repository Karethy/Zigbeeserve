#include "CollectTem.h"
#include "SysSet.h"


float getTemperature()
{
  uint value;
  ADCCON3 = 0x3e; //选择1.25V为参考电压；14位分辨率；对片内温度传感器采样
  ADCCON1 |= 0x30;//ADC启动方式选择为ADCCON1.ST=1事件
  ADCCON1 |= 0x40;//ADC启动转换
  
  while(!(ADCCON1 & 0x80))//等待AD转换完成
    ;
  value = ADCL >>4;/*ADCL寄存器低4位无效，ADC最高只能达到12位有效位。网络上很多代码这里都是右移两位，那是不对的*/
  value |= (((uint)ADCH)<<4);//连接ADCH和ADCL，并赋值给value
  return (value - 1367.5)/4.5 - 15;
}