#ifndef _ZP_GPIO_H_
#define _ZP_GPIO_H_

#include "sys.h"



/*
	PA6   detect1 前方左
	PA5   detect2 左方
	PA4   detect3 斜45度左
	PA7   detect4 斜45度右
	PC4   detect5 右方
	PC5   detect6 前方右
*/
//(改完)
#define FrontLeftIrState   PAin(6)
#define LeftIrState        PAin(5)
#define X_LeftIrState      PAin(4)
#define X_RightIrState     PAin(7)
#define RightIrState       PCin(4)
#define FrontRightIrState  PCin(5)




void IrRecieveGpioInilization(void);









#endif
//End Of The File
