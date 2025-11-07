

#include "sys.h"
#include "key.h"
#include "delay.h"
#include "Type.h"
#include "led.h"




//按键初始化函数
//
//
//PC12  SET  PA15 START 设置成输入
void KEY_Init(void)	//(改完)
{
	JTAG_Set(SWD_ENABLE);
	RCC->APB2ENR|=1<<4;         	//使能PORTC时钟
	RCC->APB2ENR|=1<<2;         	//使能PORTA时钟
	GPIOC->CRH &= 0XFFF0FFFF;			//PC12		 改成PC5
	GPIOC->CRH |= 0X00080000;	  
	GPIOC->ODR |= (1 << 12);			//上拉输入 PC12
	GPIOA->CRH &= 0X0FFFFFFF;    	//PA15 设置成输入 
	GPIOA->CRH |= 0x80000000;     
	GPIOA->ODR |= (1 << 15);			//PA15 上拉输入input
	
} 
//按键处理函数
//返回按键值
//0，没有任何按键按下
//1，SET按下
//2，START按下
//
//注意此函数有响应优先级,KEY0>KEY1>KEY2!!

unsigned char KEY_Scan(void)
{	 
	static u8 key_up=1;//按键按松开标志	
	if(key_up&&( (KEY_SET == 0) || (KEY_START == 0) ))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY_SET == 0)
		{
			return 1;
		}
		else if(KEY_START == 0)
		{
			return 2;
		}
	}
	else if((KEY_SET == 1)&&(KEY_START==1))key_up=1; 	    
	return 0;// 无按键按下
}

/*********************************************************************************************************
** Function name:       keyCheck
** Descriptions:        ????
** input parameters:    ?
** output parameters:   ?
** Returned value:      true:  按键按下
**                      false: 按键未按下
*********************************************************************************************************/
unsigned char startCheck (void)
{
    if (KEY_START == 0) 
		{
        delay_us(50);
        while(KEY_START == 0);
				GLED1 =!GLED1;
        return(KEY_TRUE);
    }
		else 
		{
        return(KEY_FALSE);
    }
}
unsigned char keyCheck (void)
{
    if (KEY_SET == 0) 
		{
        delay_us(50);
        while(KEY_SET == 0);
			  BLED2 =!BLED2;
        return(KEY_TRUE);
    }
		else 
		{
        return(KEY_FALSE);
    }
}




















