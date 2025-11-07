
#include "test.h"




const u8 TEXT_Buffer[]={"天津工业大学 STM32F103 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)		//读取数组长度
#define FLASH_SAVE_ADDR  0X08070000		//??FLASH ????(?????,????????????FLASH???+0X08000000)

uint16 w=0;
signed short int SpeedRight = 0;
signed short int SpeedLeft = 0;
unsigned short int BatteryVoltage = 0;
int main(void)
{
	unsigned char key = 0;
	unsigned char t = 0;
	u8 datatemp[SIZE];
  
	Stm32_Clock_Init(9); //72M
	delay_init(72);//延时初始化
	//uart_init(72,9600);//设置串口1波特率
	zp_UART4_Init(36,9600);	//设置串口5波特率  在APB1总线上
	LED_Init();
	KEY_Init();
	IrRecieveGpioInilization();				//红外初始化
	
	Timer6_Init(72,625);		//625us 定时
	Timer7_Init(72,1000-1);			//	1ms
	IR_SendInitialization();
	TIM8_PWM_Init(3600,0);		//20K
// 	TIM8->CCR1=150;                
// 	TIM8->CCR2=0;						//正转 左
// 	TIM8->CCR3=150;
// 	TIM8->CCR4=0;						//正转 右
	
	IIC_Init();
	MPU6050_Init();
	ZLG7289Init();
	MotorRightEncoderInit();		//右电机编码器初始化Time4
	MotorLeftEncoderInit();			//左电机编码器初始化Time1
	Adc_Init();
	
	
	delay_ms(1000);
	
	zlg7289Download(1, 5, 0, 5);
	zlg7289Download(1, 4, 0, 5);
	zlg7289Download(1, 1, 0, 1);
	while(1)
	{
		BatteryVoltage = Get_Adc(9)*3.4;
		if(startCheck() == KEY_TRUE)	//Press Start key
		{
			LED_OutPutHigh();
			t++;
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);
			zp_UART4_Printf("print: %s\r\n", "写入成功");  
		}
		if(keyCheck() == KEY_TRUE)		//Press SET Key
		{
			LED_OutPutLow();
			STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);
			zp_UART4_Printf("print: %s\r\n", datatemp);
		}
		//key = KEY_Scan();
		//zp_UART4_SendChar(t);
		w=GetData(0x47);
		//w = 65535;
		zlg7289Download(1, 3, 0, w/10000);
		zlg7289Download(1, 4, 0, w/1000%10);
		zlg7289Download(1, 5, 0, w/100%10);
		zlg7289Download(1, 6, 0, w/10%10);
		zlg7289Download(1, 7, 0, w%10);
		zp_UART4_Printf("右电机速度 %d\r\n", SpeedRight);
		zp_UART4_Printf("左电机速度 %d\r\n", SpeedLeft);
		zp_UART4_Printf("角速度w： %d\r\n", w);
// 		zp_UART4_Printf("print: %f\r\n", 1234567.1234567);
// 		zp_UART4_Printf("print: %s\r\n", "string test");
// 		zp_UART4_Printf("print: %b\r\n", 0x12345ff);
// 		zp_UART4_Printf("print: %x\r\n", 0xabcdef);
// 		zp_UART4_Printf("print: %%\r\n");
		//zp_UART4_SendString("天津工业大学\r\n");
		delay_ms(1000);
	}
}


