/*
Name :PWM.c 
Function:用于对红外发射PWM的初始化和电机控制的初始化
		红外发射：前方红外左IR1 PB4Time3CH1 IR6 PB5Time3 CH2需要部分重映射
							左方红外 ：IR2 PA3 time2CH4 右方红外：IR5 PA2 Time2CH3
							斜45度红外左：IR3 PA1Time5CH2       斜45度红外右：IR4 PA0 Time5CH1
							前左接收：PA6 				前右接收：PB8
							左方接收：PA5					右方接收：PB9
							斜左接收：PA4  				斜右接收：PC3
*/
#include "pwm.h"



/*
	Name :TIM3_PWM_Init()
	arr 周期 psc预分频器
	前方红外发射
	因为为共阳极接法，PWM2模式
*/
void TIM3_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<1; 	//TIM3时钟使能    
	RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	
	GPIOB->CRL&=0XFF00FFFF;	//PB4/5输出
	GPIOB->CRL|=0X00BB0000;	//复用功能输出 	  	 
	   
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[11:10]
	AFIO->MAPR|=1<<11;      //部分重映像,TIM3_CH2->PB5

	TIM3->ARR=arr;			//设定计数器自动重装值 
	TIM3->PSC=psc;			//预分频器不分频
	
	TIM3->CCMR1|=7<<4;  	//CH1 PWM2模式		 
	TIM3->CCMR1|=1<<3; 	  //CH1预装载使能	 
	TIM3->CCMR1|=7<<12;  	//CH2 PWM2模式		 
	TIM3->CCMR1|=1<<11; 	//CH2预装载使能	 
	TIM3->CCER|=1<<0;   	//OC1 输出使能	
	TIM3->CCER|=1<<4;   	//OC2 输出使能	   
	TIM3->CR1=0x0080;   	//ARPE使能 
	TIM3->CR1|=0x01;    	//使能定时器3 											  
} 
void Time3Freqset(unsigned short int Fre)
{
	TIM3->ARR = 72000000/Fre;	//自动加载值，从0~ARR
}

/*
	设置周期

*/
void Time3PeriodSet(unsigned short int PeriodSet)
{
	TIM3->ARR = PeriodSet;
}
//	/*设置占空比*/
void Time3WidthSet(unsigned short int Width)
{
	TIM3->CCR1 = Width;		//f_L		T3 CH1
	TIM3->CCR2 = Width;		//F_R		T3 CH2
}


 	 
// void TIM4_PWM_Init(u16 arr,u16 psc)	 //(风扇)	PB10
// {		 					 
// 	//此部分需手动修改IO口设置
// 	RCC->APB1ENR|=1<<2; 	//TIM4时钟使能    
// 	RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	
// 	GPIOB->CRH&=0XFFFFF0FF;	//PB10输出
// 	GPIOB->CRH|=0X00000B00;	//复用功能推挽输出  	 
// 	   
// 	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
// 	AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[11:10]
// //	AFIO->MAPR|=1<<11;      //部分重映像,TIM3_CH2->PB5

// 	TIM4->ARR=arr;			//设定计数器自动重装值 
// 	TIM4->PSC=psc;			//预分频器不分频 (不分频)
// 	
// 	TIM4->CCMR1|=7<<4;  	//CH1 PWM2模式		 
// 	TIM4->CCMR1|=1<<3; 	  //CH1预装载使能	 
// // 	TIM4->CCMR1|=7<<12;  	//CH2 PWM2模式		 
// // 	TIM4->CCMR1|=1<<11; 	//CH2预装载使能	 
// 	TIM4->CCER|=1<<0;   	//OC1 输出使能	
// // 	TIM4->CCER|=1<<4;   	//OC2 输出使能	   
// 	TIM4->CR1=0x0080;   	//ARPE使能 
// 	TIM4->CR1|=0x01;    	//使能定时器3 											  
// }  
// void Time4Freqset(unsigned short int Fre)//900
// {
// 	TIM4->ARR = 72000000/Fre;	//自动加载值，从0~ARR
// }
// void Time4PeriodSet(unsigned short int PeriodSet)
// {
// 	TIM4->ARR = PeriodSet;
// }	 
// void Time4WidthSet(unsigned short int Width)
// {
// 	TIM4->CCR1 = Width;		//f_L		T3 CH1
// //	TIM4->CCR2 = Width;		//F_R		T3 CH2
// }




/*
	Name :TIM2_PWM_Init()
	arr 周期 psc预分频器
	左右红外发射左IR2 PA3 time2CH4 
	右IR2 PA2 time2CH3
	因为为共阳极接法，PWM2模式
*/
// void TIM2_PWM_Init(u16 arr,u16 psc)	 //arr预装载值		  psc预分频
// {		 					 
// 	//此部分需手动修改IO口设置
// 	RCC->APB1ENR|=1<<0; 	//TIM2时钟使能    
// 	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟	
// 	GPIOA->CRL&=0XFFFF00FF;	//PA2 3输出
// 	GPIOA->CRL|=0X0000BB00;	//复用功能输出 	  	 
// 	   
// 	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
// 	//AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[11:10]
// 	//AFIO->MAPR|=1<<11;      //部分重映像,TIM3_CH2->PB5

// 	TIM2->ARR=arr;			//设定计数器自动重装值 
// 	TIM2->PSC=psc;			//预分频器不分频
// 	
// 	TIM2->CCMR2|=7<<4;  	//CH3PWM2模式		 
// 	TIM2->CCMR2|=1<<3; 	  //CH3预装载使能	 
// 	TIM2->CCMR2|=7<<12;  	//CH4 PWM2模式		 
// 	TIM2->CCMR2|=1<<11; 	//CH4预装载使能	 
// 	TIM2->CCER|=1<<8;   	//OC3 输出使能	
// 	TIM2->CCER|=1<<12;   	//OC4 输出使能	   
// 	TIM2->CR1=0x0080;   	//ARPE使能 
// 	TIM2->CR1|=0x01;    	//使能定时器3 											  
// }  	 

// void Time2Freqset(unsigned short int Fre)
// {
// 	TIM2->ARR = 72000000/Fre;
// }
// /*
// 	设置周期

// */
// void Time2PeriodSet(unsigned short int PeriodSet)
// {
// 	TIM2->ARR = PeriodSet;
// }

/*
	设置占空比

*/
// void Time2WidthSet(unsigned short int Width)
// {
// 	TIM2->CCR3 = Width;		//PA2IR5 Right
// 	TIM2->CCR4 = Width;		//PA3IR2 Left
// }

/*
	Name :TIM5_PWM_Init()
	arr 周期 psc预分频器
	斜45红外发射左IR3 PA1 time5CH2
	右IR4 PA0 time5CH1
	因为为共阳极接法，PWM2模式

  左右红外发射左IR2 PA3 time5CH4 
	右IR2 PA2 time5CH3

*/
void TIM5_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<3; 	//TIM5时钟使能    
	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟	
	GPIOA->CRL&=0XFFFF0000;	//PA0 1 2 3输出
	GPIOA->CRL|=0X0000BBBB;	//复用功能输出 	  	 
	   
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	//AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[11:10]
	//AFIO->MAPR|=1<<11;      //部分重映像,TIM3_CH2->PB5

	TIM5->ARR=arr;			//设定计数器自动重装值 
	TIM5->PSC=psc;			//预分频器不分频
	
	//通道1和通道2用CCMR1，通道3和通道4用CCMR2
	TIM5->CCMR1|=7<<4;  	//CH1PWM2模式		 
	TIM5->CCMR1|=1<<3; 	  //CH1预装载使能	 
	TIM5->CCMR1|=7<<12;  	//CH2 PWM2模式		 
	TIM5->CCMR1|=1<<11; 	//CH2预装载使能	
	TIM5->CCMR2|=7<<4;  	//CH3PWM2模式		 
	TIM5->CCMR2|=1<<3; 	  //CH3预装载使能	 	
	TIM5->CCMR2|=7<<12;  	//CH4 PWM2模式		 
	TIM5->CCMR2|=1<<11; 	//CH4 预装载使能	 
	
	TIM5->CCER|=1<<0;   	//OC1 输出使能	
	TIM5->CCER|=1<<4;   	//OC2 输出使能
  TIM5->CCER|=1<<8;        // TIM5CH3
	TIM5->CCER|=1<<12;        // TIM5CH4	
	
	TIM5->CR1=0x0080;   	//ARPE使能 
	TIM5->CR1|=0x01;    	//使能定时器5 		
//TIM2->CCMR2|=7<<4;  	//CH3PWM2模式		 
// 	TIM2->CCMR2|=1<<3; 	  //CH3预装载使能	 
// 	TIM2->CCMR2|=7<<12;  	//CH4 PWM2模式		 
// 	TIM2->CCMR2|=1<<11; 	//CH4预装载使能	 
// 	TIM2->CCER|=1<<8;   	//OC3 输出使能	
// 	TIM2->CCER|=1<<12;   	//OC4 输出使能		
}  	 

void Time5Freqset(unsigned short int Fre)
{
	TIM5->ARR = 72000000/Fre;
}

void Time5PeriodSet(unsigned short int PeriodSet)
{
	TIM5->ARR = PeriodSet;
}

void Time5WidthSet(unsigned short int Width)
{
	TIM5->CCR1 = Width;		//X_R		T3 CH1
	TIM5->CCR2 = Width;		//X_L		T3 CH2
	TIM5->CCR3 = Width;		//PA2IR5 Right
	TIM5->CCR4 = Width;		//PA3IR2 Left
}

//设置前方左红外的占空比
void FrontLeftWidthSet(unsigned short int Width)
{
	 
	 TIM3->CCR1 = Width;		//f_L		T3 CH1
}
//设置前方右红外的占空比
void FrontRightWidthSet(unsigned short int Width)
{
	 TIM3->CCR2 = Width;		//F_R		T3 CH2
}
//右方占空比设置
void RightWidthSet(unsigned short int Width)
{
	TIM5->CCR3 = Width;		//PA2IR5 Right
}
//左方占空比设置
void LeftWidthSet(unsigned short int Width)
{
	TIM5->CCR4 = Width;		//PA3IR2 Left
}


//斜右方占空比设置
void XieRightWidthSet(unsigned short int Width)
{
	TIM5->CCR1 = Width;		//X_R		T3 CH1
}
//斜左方占空比设置
void XieLeftWidthSet(unsigned short int Width)
{
	TIM5->CCR2 = Width;		//X_L		T3 CH2
}

void IR_SendInitialization()
{
	TIM3_PWM_Init(2000,0);			//前方
//	TIM2_PWM_Init(1869,0);			//左右
	TIM5_PWM_Init(1869,0);			//xie  左右
	Time3Freqset(39000);				//前方
//	Time2Freqset(34000);				//左右
	Time5Freqset(36000);				//斜 45度
	
	TIM3->CCR1 = 0;		//f_L		T3 CH1
	TIM3->CCR2 = 0;		//F_R		T3 CH2
// 	TIM2->CCR3 = 0;		//PA2IR5 Right
// 	TIM2->CCR4 = 0;		//PA3IR2 Left
	TIM5->CCR1 = 0;		//X_R		T3 CH1
	TIM5->CCR2 = 0;		//X_L		T3 CH2
	TIM5->CCR3 = 0;		//ZUO
	TIM5->CCR4 = 0;		//YOU
}
// void Fan()		   //风扇
// {
// 	TIM4_PWM_Init(900,0);			//arr预装载值，不分频
// 	Time4Freqset(34000);				//频率设置
// 	TIM4->CCR1 = 0;		          //CCR从0开始
// }

/*


PC6 7 8 9Time8

*/

void TIM8_PWM_Init(u16 arr,u16 psc)	 //电机接口
{		 					 
	RCC->APB2ENR|=1<<13;              // Enable Time8 Clock
	RCC->APB2ENR|=1<<4;               //Enable IOC
	RCC->APB2ENR|=1<<0;               //Enable AFIO Clock

	GPIOC->CRL&=0X00FFFFFF;     // Clear the State of PC6,7
	GPIOC->CRL|=0XBB000000;     // Set   the State of PC6 C7
	GPIOC->CRH&=0XFFFFFF00;     // Clear the State of PC8,9
	GPIOC->CRH|=0X000000BB;     // Set   the State of PC8 C9



	TIM8->CCR1=0;                
	TIM8->CCR2=0;
	TIM8->CCR3=0;
	TIM8->CCR4=0;
	
	TIM8->ARR=arr;        //Load   
	TIM8->PSC=psc;        // 
	//TIM1->BDTR|=0xCD;        // Dead_Time

  //TIM8->CR1|=3<<5;       // Up Dowm 
//	TIM8->CR1|=1<<0;       // Enable the Count of Time1

// 	TIM8->CR2|=1<<8;	   //??????1(OC1??)
// 	TIM8->CR2|=1<<10;	   //??????2(OC2??)
// 	TIM8->CR2|=1<<12;	   //??????3(OC3??)
	

	TIM8->CCMR1|=6<<4;       // TIM1CH1 PWM2 Mode
	TIM8->CCMR1|=6<<12;      // TIM1CH2 PWM2 Mode
	TIM8->CCMR2|=6<<4;       // TIM1CH3 PWM2 Mode
	TIM8->CCMR2|=6<<12;       // TIM1CH3 PWM2 Mode

	//TIM1->CCER|=1<<1;		 //??????? ?????
	//TIM1->CCER|=1<<5;		 //??????? ?????
	//TIM1->CCER|=1<<9;		 //

//   TIM1->CCER|=1<<2;		 //??????? ?????
// 	TIM1->CCER|=1<<6;		 //??????? ?????
// 	TIM1->CCER|=1<<10;		 //

	TIM8->CCER|=1<<0;        // TIM1CH1 
	TIM8->CCER|=1<<4;        // TIM1CH2 
	TIM8->CCER|=1<<8;        // TIM1CH3
	TIM8->CCER|=1<<12;        // TIM1CH3


	TIM8->CR1=0x0080;   	//ARPE使能 
	TIM8->CR1|=0x01;    	//使能定时器8
	
	TIM8->BDTR|=1<<15;       // Open all channal	
	//zhongduan
	//TIM1->DIER|=1<<0;   //??????	 
	//MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);//??1,????3,?2	
}  
void fan_init(u16 arr,u16 psc)//定时器2
{
	RCC->APB1ENR|=1<<0; 	//TIM2时钟使能           
	RCC->APB2ENR|=1<<3;   //Enable IO PB
	RCC->APB2ENR|=1<<0;   //Enable AFIO Clock
// 	
// 	AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[9:10]
// 	AFIO->MAPR|=1<<11;      //部分重映像,TIM3_CH2->PB5
	
	AFIO->MAPR &= 0XFFFFFCFF;		//清除MAPR的[9:8]
	AFIO->MAPR |= 2 << 8;      		//部分重映像,TIM2_CH3->PB10
	
	TIM2->CCR3 = 0;  //通道3，初始化占空比  //电机3.7V CCR最高不得超过770	
	TIM2->ARR = arr;        //Load   
	TIM2->PSC = psc;     
	
	GPIOB->CRH&=0XFFFFF0FF;     // Clear the State of PB10
	GPIOB->CRH|=0X00000B00;     // Set   the State of PB10
	
	TIM2->CCMR2 |= 6<<4;   	// TIM2 CH3 PWM1 模式 高电平有效 
	TIM2->CCMR2 |= 1<<3; 	  //CH3预装载使能	  输出比较3预装载使能
	
	
	TIM2->CCER  |= 1<<8;   	//OC3 输出使能
	
	TIM2->CR1=0x0080;   	//ARPE使能 
	TIM2->CR1|=0x01;    	//使能定时器2
}
// void Time2WidthSet(unsigned short int Width)
// {
// 	TIM2->CCR3 = Width;		//f_L		T3 CH1
// // 	TIM2->CCR2 = Width;		//F_R		T3 CH2
// }






































