/**********************************************
function:Encoder
Right Motor Encoder:Timer 4
Left  Motor Encoder:Timer 1

********************************************************/
#include "Encoder.h"
#include "sys.h"



/*
	//Name:MotorRightEncoderInit
	//FUnction :Initialization of Time4
	// Right Motor
*/
void MotorRightEncoderInit()
{
	RCC->APB1ENR|=1<<2;       //TIM4使能
  RCC->APB2ENR|=1<<3;       //PORTB使能
        
  GPIOB->CRL&=0X00FFFFFF;   //PB6?PB7 复位
  GPIOB->CRL|=0X44000000;   //PB6?PB7 浮空输入
            
  TIM4->ARR=65535-1;      //设定计数器自动重装值
  TIM4->PSC=0;            //预分频器

  TIM4->CCMR1 |= 1<<0;         //输入模式,IC2FP1映射到TI1上
  TIM4->CCMR1 |= 1<<8;         //输入模式,IC2FP2映射到TI2上
  TIM4->CCER &= ~(1<<1);          //IC1不反向
  TIM4->CCER &= ~(1<<5);          //IC2不反向
  TIM4->SMCR |= 3<<0;          //所有输入均在上升沿和下降沿有效
  TIM4->CR1 |= 1<<0;           //使能定时器
	TIM4->CNT = 32768;
}



/*
	//Name:MotorRightEncoderInit
	//FUnction :Initialization of Time4
	// Right Motor
*/
void MotorLeftEncoderInit()
{
	RCC->APB2ENR|=1<<11;       //TIM1使能
  RCC->APB2ENR|=1<<2;       //PORTA使能
        
  GPIOA->CRH&=0XFFFFFF00;   //PA8?PA9 复位
  GPIOA->CRH|=0X00000044;   //PA8?PA9 浮空输入
            
  TIM1->ARR=65535-1;      //设定计数器自动重装值
  TIM1->PSC=0;            //预分频器

  TIM1->CCMR1 |= 1<<0;         //输入模式,IC2FP1映射到TI1上
  TIM1->CCMR1 |= 1<<8;         //输入模式,IC2FP2映射到TI2上
  TIM1->CCER &= ~(1<<1);          //IC1不反向
  TIM1->CCER &= ~(1<<5);          //IC2不反向
  TIM1->SMCR |= 3<<0;          //所有输入均在上升沿和下降沿有效
  TIM1->CR1 |= 1<<0;           //使能定时器
	TIM1->CNT = 32768;
}































