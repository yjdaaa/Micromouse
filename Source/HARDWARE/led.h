#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


#define LED0 PCout(13)	// PC5
#define LED1 PCout(14)	// DS1	 
#define LED2 PCout(15)	// DS1	

#define RLED0 PAout(10)	// PC5
#define GLED1 PAout(11)	// DS1	 
#define BLED2 PAout(12)	// DS1	


void LED_Init(void);	//≥ı ºªØ	
void LED_OutPutHigh(void);	//OutPut High
void LED_OutPutLow(void);		//OutPut Low
void LED_OutPutFlip(void);	//OutPut Flip
#endif

















