#ifndef _ZP_USART_H_
#define _ZP_USART_H_


void zp_UART1_Init(unsigned long pclk2,unsigned long bound);
void zp_UART1_SendChar(unsigned char buffer);

void zp_UART2_Init(unsigned long pclk2,unsigned long bound);
void zp_UART2_SendChar(unsigned char buffer);

void zp_UART3_Init(unsigned long pclk2,unsigned long bound);
void zp_UART3_SendChar(unsigned char buffer);


void zp_UART4_Init(unsigned long pclk2,unsigned long bound);
void zp_UART4_SendChar(unsigned char buffer);
void zp_UART4_SendString(char *str);
int zp_UART4_Printf(char *fmt,...);

#endif
/* End Of The File */



