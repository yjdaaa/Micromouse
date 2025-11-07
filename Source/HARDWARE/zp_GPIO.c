/***************************************************************************************************************
***Name   : LED.C

***GPIO.C 
*  初始化红外接收引脚
****************************************************************************************************/


#include "zp_GPIO.h"
/*
	初始化红外接手IO口
	配置为浮空输入 
	PA6   detect1 前方左
	PA5   detect2 左方
	PA4   detect3 斜45度左
	PA7   detect4 斜45度右
	PC4   detect5 左方
	PC5   detect6 前方右
*/
void IrRecieveGpioInilization()		  //红外接收	 (改完)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
//	RCC->APB2ENR|=1<<3;    //使能PORTB时钟
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟
	
	GPIOA->CRL &= 0X0000FFFF;		//PA4 5 6 7
	GPIOA->CRL |= 0X44440000;	 //上拉输入
	GPIOA->ODR |= (1<<4) 
						 | 	(1<<5)
						 | 	(1<<6)
						 | 	(1<<7);
	
	GPIOC->CRL &= 0XFF00FFFF;		//PC4 C5
	GPIOC->CRL |= 0X00440000;	  //上拉输入
	GPIOC->ODR |= (1<<4)
						 |  (1<<5);
	
	
//	GPIOC->CRL &= 0XFFFF0FFF;		//PC 3
//	GPIOC->CRL |= 0X00004000;	 //浮空输入

}








/*End Of The File */

