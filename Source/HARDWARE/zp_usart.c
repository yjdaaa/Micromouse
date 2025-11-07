/*

*/


#include "sys.h"
#include "zp_usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "led.h"


//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
void zp_UART1_Init(unsigned long pclk2,unsigned long bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH &= ~(0xff << 4);	  	//复位PA9 PA10
	GPIOA->CRH |= (0x0b << 4);		//PA9 复用推挽输出
	GPIOA->CRH |= (0x04 << 8);	 	//PA10 浮空输入 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.

	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 

}
/*
   name :  zp_UART1_SendChar
   buffer: 发送的数据
*/
void zp_UART1_SendChar(unsigned char buffer)
{
	 USART1->DR = buffer ;
	 while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
}

void zp_UART2_Init(unsigned long pclk2,unsigned long bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口2时钟 
	GPIOA->CRL &= ~(0xff << 8);	  	//复位PA2tx PA3rx
	GPIOA->CRL |= (0x0b << 8);		//PA2 复用推挽输出
	GPIOA->CRL |= (0x04 << 12);	 	//PA3 浮空输入 
	RCC->APB1RSTR |= 1<<17;   //复位串口2
	RCC->APB1RSTR &= ~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.

	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级 
}

void zp_UART2_SendChar(unsigned char buffer)
{
	 USART2->DR = buffer ;
	 while((USART2->SR&0X40)==0);//循环发送,直到发送完毕
}


void zp_UART3_Init(unsigned long pclk2,unsigned long bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口3时钟 
	GPIOB->CRH &= ~(0xff << 8);	  	//复位PB10tx PAB11rx
	GPIOB->CRH |= (0x0b << 8);		//PB10 复用推挽输出
	GPIOB->CRH |= (0x04 << 12);	 	//PB11 浮空输入 
	RCC->APB1RSTR |= 1<<18;   //复位串口2
	RCC->APB1RSTR &= ~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.

	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART3_IRQn,2);//组2，最低优先级 
}


void zp_UART3_SendChar(unsigned char buffer)
{
	 USART3->DR = buffer ;
	 while((USART3->SR&0X40)==0);//循环发送,直到发送完毕
}

void zp_UART4_Init(unsigned long pclk2,unsigned long bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<4;   //使能PORTC口时钟 
  RCC->APB2ENR|=1<<0;   //使能AFIO 	
	RCC->APB1ENR|=1<<19;  //使能串口4时钟 
	GPIOC->CRH &= 0xffff00ff;	  	//复位PC10tx PC11rx
	GPIOC->CRH |= (0x0b << 8);		//PC10 复用推挽输出
	GPIOC->CRH |= (0x08 << 12);	 	//PC11 浮空输入 
	RCC->APB1RSTR |= 1<<19;   //复位串口4
	RCC->APB1RSTR &= ~(1<<19);//停止复位	   	   
	//波特率设置
 	UART4->BRR=mantissa; // 波特率设置	 
	UART4->CR1|=0X200C;  //1位停止,无校验位.

	UART4->CR1|=1<<8;    //PE中断使能
	UART4->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,UART4_IRQn,2);//组2，最低优先级 
}


void zp_UART4_SendChar(unsigned char buffer)
{
	 while((UART4->SR&0X40)==0);      //循环发送,直到发送完毕
	 UART4->DR = buffer ;
	 
}
void zp_UART4_SendString( char *str)
{
	while(*str != 0)
	{
		zp_UART4_SendChar(*str);
		str++; 
	}
}

int zp_UART4_Printf(char *fmt,...)
{
	
	 va_list ap;			//??????????????         
		char string[256];	         
		va_start(ap,fmt);		//?????????????ap,?ap???????????         
		printf(string,fmt,ap);	//???fmt?ap??????????????????,?string???,????sprintf(),????????         
    zp_UART4_SendString(string); 	//????????????????         
		va_end(ap);    			//ap???0,???????,?????????
	 return 0;
}

void UART4_IRQHandler(void)
{
	u8 res;	
	if(UART4->SR&(1<<5))	//?????
	{	 
		res=UART4->DR; 
		zp_UART4_SendChar(res);
		LED_OutPutFlip();
	}

} 





