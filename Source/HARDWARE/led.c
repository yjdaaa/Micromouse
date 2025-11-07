/***************************************************************************************************************

*** PA10 RED
*		PA11 GREEN
*** PA12 BLUE
**************************************************************************************************************/




#include "led.h"




/*	
		Name:LED_Init
		LED : PA10 
		Fuction :Set PC5 As OutPut
*/
void LED_Init(void)
{
	RCC->APB2ENR|=1<<0;     		 	//使能AFIO功能
	RCC->APB2ENR|=1<<2;    				//使能PORTA时钟
	GPIOA->CRH&=0XFFF000FF; 
	GPIOA->CRH|=0X00033300;				//PC.13 14 15  推挽输出
//	GPIOA->ODR|=1<<15;      			//PC.13 14 15 输出高


	RLED0 = 1;											//红灯
	GLED1 = 1;											//绿灯
	BLED2 = 1;											//蓝灯坏了							
}

/*	
		Name:LED_OutPutHigh
		LED :PC5
		Fuction :Set PC5 As High
*/
void LED_OutPutHigh()
{
	GPIOC->ODR |= 1<<5;      //PC.5 输出高
}
/*	
		Name:LED_OutPutLow
		LED :PC5
		Fuction :Set PC5 As High
*/
void LED_OutPutLow()
{
	GPIOC->ODR &= ~(1<<5);      //PC.5 输出低
}


/*	
		Name:LED_OutPutFlip
		LED :PC5
		Fuction :Set PC5 Flip
*/
void LED_OutPutFlip()
{
	static unsigned char Led_Flag = 0;
	if(Led_Flag == 0)
	{
		GPIOC->ODR &= ~(1<<5);      //PC.5 输出低
		Led_Flag = 1;
	}
	else 
	{
		GPIOC->ODR |= (1<<5);      //PC.5 输出高
		Led_Flag = 0;
	}
	
}







