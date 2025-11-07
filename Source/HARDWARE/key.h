#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//Mini STM32开发板
//按键输入 驱动代码			 
//正点原子@ALIENTEK
//2010/5/27 

#define KEY_SET   PCin(12)	 //PC12
#define KEY_START PAin(15)	   //PD2
#define FAN_SET PBin(10)
	 
void KEY_Init(void);//IO初始化
unsigned char KEY_Scan(void);  //按键扫描函数		
unsigned char startCheck (void);
unsigned char keyCheck (void);

#endif
