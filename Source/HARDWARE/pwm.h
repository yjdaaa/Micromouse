#ifndef __PWM_H_
#define __PWM_H_

#include "sys.h"



void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
//void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM5_PWM_Init(u16 arr,u16 psc);
void IR_SendInitialization(void);

void TIM8_PWM_Init(u16 arr,u16 psc);										//Motor Control
void fan_init(u16 arr,u16 psc);




void Time3PeriodSet(unsigned short int PeriodSet);				//前方红外
//void Time2PeriodSet(unsigned short int PeriodSet);				//左右红外
void Time5PeriodSet(unsigned short int PeriodSet);				//xie 45

void Time3WidthSet(unsigned short int Width);
// void Time2WidthSet(unsigned short int Width);
void Time5WidthSet(unsigned short int Width);



void FrontLeftWidthSet(unsigned short int Width);
void FrontRightWidthSet(unsigned short int Width);
void RightWidthSet(unsigned short int Width);
void LeftWidthSet(unsigned short int Width);
void XieRightWidthSet(unsigned short int Width);
void XieLeftWidthSet(unsigned short int Width);


#endif
































