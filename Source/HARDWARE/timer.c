#include "timer.h"
#include "led.h"




//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timer3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//TIM3时钟使能    
 	TIM3->ARR=arr;  //设定计数器自动重装值//刚好1ms    
	TIM3->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->DIER|=1<<6;   //允许触发中断	  							    
	TIM3->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2									 
}





//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器6!
void Timer6_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;//TIM6时钟使能    
 	TIM6->ARR=arr;  //设定计数器自动重装值//刚好625us    
	TIM6->PSC=psc;  //预分频器72,得到10Mhz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM6->DIER|=1<<0;   //允许更新中断				
	TIM6->DIER|=1<<6;   //允许触发中断	  							    
	TIM6->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,2,TIM6_IRQn,2);//抢占1，子优先级1，组2	 最高优先级								 
}

// void TIM6_IRQHandler(void)
// { 
// 	static unsigned int i = 0;
// 	
// 	if(TIM6->SR&0X0001)//溢出中断
// 	{
// 		i++;
// 		if(i == 1000)
// 		{
// 			i = 0;
// 			//LED0 = !LED0;
// 			//LED_OutPutFlip();
// 		}			
// 	}				   
// 	TIM6->SR&=~(1<<0);//清除中断标志位
// }

//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器6!
void Timer7_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;//TIM7时钟使能    
 	TIM7->ARR=arr;  //设定计数器自动重装值//刚好   
	TIM7->PSC=psc;  //预分频器72,得到10Mhz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM7->DIER|=1<<0;   //允许更新中断				
	TIM7->DIER|=1<<6;   //允许触发中断	  							    
	TIM7->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,1,TIM7_IRQn,2);//抢占1，子优先级1，组2	 最高优先级								 
}

// void TIM7_IRQHandler(void)
// { 
// 	static unsigned int i = 0;
// 	
// 	if(TIM7->SR&0X0001)//溢出中断
// 	{
// 		SpeedRight = 32768 - TIM4->CNT;
// 		SpeedLeft  = TIM1->CNT -32768;
// 		TIM4->CNT = 32768;
// 		TIM1->CNT = 32768;
// 		i++;
// 		if(i == 1000)
// 		{
// 			i = 0;
// 			LED0 = !LED0;
// 			//LED_OutPutFlip();
// 		}			
// 	}				   
// 	TIM7->SR&=~(1<<0);//清除中断标志位
// }




void PWM_Init(void)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<1;       //TIM3时钟使能    
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  	
	GPIOA->CRL&=0X0FFFFFFF;//PA7输出
	GPIOA->CRL|=0XB0000000;//复用功能输出 	  

	TIM3->CCMR1|=6<<12;  //CH2 PWM1模式		 
	TIM3->CCMR1|=1<<11; //CH2预装载使能	   

	TIM3->CCER|=1<<4;   //OC2 输出使能	   

	TIM3->CR1=0x8000;   //ARPE使能 
	TIM3->CCR2 = 0;
	TIM3->CR1|=0x01;    //使能定时器3 										  
} 














