#include "zlg7289.h"
#include "led.h"

/*********************************************************************************************************
** Function name:       ZLG7289_Delay
** Descriptions:        延时N个微秒
** input parameters:    x: 延时时间
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ZLG7289_Delay(uint16 x)
{	
	x += x;
	while(x--);
}

/*********************************************************************************************************
** Function name:       ZLG7289_DIO_OUT
** Descriptions:        将ZLG728_DI0设置成输出模式。
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ZLG7289_DIO_OUT(void)	 //(改完)
{
	//DIO=1;PB12
    GPIOB->CRL &= 0XFFFF0FFF; 
    GPIOB->CRL |= 0X000F3000;    //PB3推挽输出 

}
/*********************************************************************************************************
** Function name:       ZLG7289_DIO_IN
** Descriptions:        将ZLG728_DI0设置成输入模式。
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ZLG7289_DIO_IN(void)	//（改完）
{
    GPIOB->CRL &= 0XFFFF0FFF;
    GPIOB->CRL |= 0X00008000;    //上拉输入
	  GPIOB->ODR |= 1<<3;
}

/*********************************************************************************************************
** Function name:       __zlg7289SPIWrite
** Descriptions:        向SPI 总线写入1 个字节的数据。
** input parameters:    cDat：要写入的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __zlg7289SPIWrite (char cDat)
{
      char CT=8;
      ZLG7289_DIO_OUT();	 //DIO设置成输出模式
      do{
           if((cDat & 0x80) == 0x80)
            {
                 ZLG7289_DIO=1;	
            }
            else
            {
                 ZLG7289_DIO=0;
            }
            cDat <<= 1;
            ZLG7289_CLK=1;
            ZLG7289_Delay(50);
            ZLG7289_CLK=0;
            ZLG7289_Delay(50);
        }while(--CT!=0);
}
/*********************************************************************************************************
** Function name:       __zlg7289SPIRead
** Descriptions:        从SPI 总线读取1 个字节的数据。
** input parameters:    无
** output parameters:   无
** Returned value:      读取到的数据
*********************************************************************************************************/
 

char __zlg7289SPIRead (void)
{
    char cDat = 0;
    char cT   = 8;
    ZLG7289_DIO_IN();   
  
   do
	{
	    ZLG7289_CLK=1;
	    ZLG7289_Delay(50);
	    cDat<<=1;  
	    if(ZLG7289_DIOIN == 1)  cDat++;
	    ZLG7289_CLK=0;
	    ZLG7289_Delay(50);

	} while ( --cT != 0 );
	return 	cDat;
} 

/*********************************************************************************************************
** Function name:       zlg7289Cmd
** Descriptions:        执行ZLG7289 纯指令。
** input parameters:    cCmd：命令字
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void zlg7289Cmd (char  cCmd)
{
    ZLG7289_CS=0;
    ZLG7289_Delay(200);
    __zlg7289SPIWrite (cCmd);
    ZLG7289_CS=1;
    ZLG7289_Delay(50);	
} 
/*********************************************************************************************************
** Function name:       zlg7289CmdDat
** Descriptions:        执行ZLG7289 带数据指令。
** input parameters:    cCmd：命令字
**                      cDat：数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void zlg7289CmdDat (uint8  cCmd, char  cDat)
{
    ZLG7289_CS=0;
    ZLG7289_Delay(200);
    __zlg7289SPIWrite (cCmd);
    ZLG7289_Delay(80);
    __zlg7289SPIWrite (cDat);
    ZLG7289_CS=1;
    ZLG7289_Delay(50);
}
/*********************************************************************************************************
** Function name:       zlg7289Download
** Descriptions:        下载数据。
** input parameters:    ucMod=0： 下载数据且按方式0 译码
**                      ucMod=1： 下载数据且按方式1 译码
**                      ucMod=2： 下载数据但不译码
**                      cX：      数码管编号（横坐标），取值0～7
**                      cDp=0：   小数点不亮
**                      cDp=1：   小数点亮
**                      cDat：    要显示的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void zlg7289Download (u8  ucMod, char  cX, char  cDp, char  cDat)
{
    unsigned char ucModDat[3] = {0x80,0xC8,0x90};
    unsigned char ucD1;
    unsigned char ucD2;
    
    if (ucMod > 2) {
        ucMod = 2;
    }
    
    ucD1  = ucModDat[ucMod];
    cX   &= 0x07;
    ucD1 |= cX;
    ucD2  = cDat & 0x7F;
    
    if (cDp  == 1) {
        ucD2 |= 0x80;
    }
    zlg7289CmdDat(ucD1, ucD2);
}
/*********************************************************************************************************
** Function name:       zlg7289Key
** Descriptions:        执行ZLG7289 键盘命令。
** input parameters:    无
** output parameters:   无
** Returned value:      返回读到的按键值：0～63。如果返回0xFF 则表示没有键按下
*********************************************************************************************************/
char zlg7289Key (void)
{
   char key;
   
   ZLG7289_CS=0;
   ZLG7289_Delay(200);
   __zlg7289SPIWrite(0x15);
   ZLG7289_Delay(100);
   key=__zlg7289SPIRead();
   ZLG7289_CS=1;
   ZLG7289_Delay(50);

   return key;
}
/*********************************************************************************************************
** Function name:       __zlg7289KeyInit
** Descriptions:        对ZLG7289按键中断初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __zlg7289KeyInit (void)	//(改完)
{
	
	  GPIOD->CRL &= 0xFFFFF0FF;		//reset PA10 Key	  //改成PA10
	  GPIOD->CRL |= 0x00000800;		//set PA10 Key As input
	  GPIOD->ODR |= 1<<2;  
	  Ex_NVIC_Config(GPIO_D,2,FTIR); 	//a10 下降沿触发
	  MY_NVIC_Init(2,3,EXTI2_IRQn,2);	//抢占优先级2,子优先级3,组2
}


/*********************************************************************************************************
** Function name:       ZLG7289Init
** Descriptions:        对ZLG7289进行初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ZLG7289Init(void)     //（改完）
{
	JTAG_Set(SWD_ENABLE);
	//JTAG_Set(JTAG_SWD_DISABLE);
	RCC->APB2ENR|=1<<3;        //使能PORTB时钟 
	RCC->APB2ENR|=1<<5;        //使能PORTD时钟 
	GPIOB->CRL &= 0xFFFF0FFF;
	GPIOB->CRL |= 0x00003000;			//PB3 DIO
	
	GPIOB->CRH &= 0xFFFFFF00;
	GPIOB->CRH |= 0x00000033;			//PB8 CS PB9 CLK
    
	
  ZLG7289_DIO=1;
  ZLG7289_CLK=0;	
  ZLG7289_CS=1;
  __zlg7289KeyInit();
  zlg7289Reset();	
	  //zlg7289Reset();
}


//????4????
// void EXTI15_10_IRQHandler(void)
// {
// 	unsigned char key = 0;
// 	if((EXTI->PR & (1<<13)) == (1<<13))		//line 13
// 	//if(KEY0==0)	 	//??KEY0
// 	{
// 		key = zlg7289Key();
// 		if(key != 0xff)
// 		{
// 			zlg7289Download(1, 0, 0, key);
// 			//LED0=!LED0;
// 		//LED1=!LED1;
// 		}
// 	}		 
// 	EXTI->PR=1<<13;  //??LINE4???????  
// }



