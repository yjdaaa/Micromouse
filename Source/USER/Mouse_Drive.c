/****************************************Copyright (c)****************************************************
**                                       �������ΰҵ���޹�˾
**
*********************************************************************************************************/


/*********************************************************************************************************
  ����
	
*********************************************************************************************************/
#include "Mouse_Drive.h"
/*********************************************************************************************************
  ����ȫ�ֱ���
*********************************************************************************************************/
MAZECOOR          GmcMouse                        = {0,0};              /*  ���������ǰλ������      */
uint8             GucMouseDir                     = UP;                 /*  ���������ǰ����          */
uint8             GucMapBlock[MAZETYPE][MAZETYPE] = {0};                /*  GucMapBlock[x][y]���ķ����·�������´洢��ǽ������  */
                                                                        /*  x,������;y,������;          */
uint8             GucMapBlock0[MAZETYPE][MAZETYPE] = {0};               /*  �洢��������̽������ǽ����Ϣ�ͽ���Ԥ���ݵ�ǽ������ */
                                                                   /*  bit3~bit0�ֱ������������   */
                                                                        /*  0:�÷�����·��1:�÷�����·  */
uint8             GucMapBlock1[MAZETYPE][MAZETYPE]= {0x0f};            //�ں�ˮ�㷨��·�������´洢��ǽ������
uint8             GucMouseStart                    = 0;                 /* ����������        */
uint8             GucFrontJinju                    = 0;                 /* ǰ���������������ʱ�ȸ�ͼ��������   */
uint8             GucCrossroad                     = 0;                 /* ʮ��·���������ʱ�ã���ʮ��·�ڶཱུ����߳���ٶ�   */
uint8             F =0;
static uint32     SGW_R;
static uint32     SGW_L;
static uint32     UL;
static uint32     UR;
static uint32     GW;                                                       /*С��ת���Ƕ�*/
static uint32     GW_R=0;                                                       /*С����ת�������Ƕ�*/
static uint32     GW_L=0;                                                       /*С����ת�������Ƕ�*/
static __MOTOR  __GmLeft                          = {0, 0, 0, 0, 0, 0};    /*  ���岢��ʼ������״̬      */
static __MOTOR  __GmRight                         = {0, 0, 0, 0, 0, 0};    /*  ���岢��ʼ���ҵ��״̬      */
static __PID    __GmLPID;                                                 /*  ��������PID      */
static __PID    __GmRPID;                                                 /*  �����ҵ��PID     */
static __PID    __GmSPID;                                                 /*  ֱ��PID     */
static __PID    __GmWPID;                                                 /*  ��תPID     */
static uint8    __GucMouseState                   = __STOP;             /*  ���������ǰ����״̬      */
static int32    __GiMaxSpeed                      = SEARCHSPEED;        /*  �����������е�����ٶ�      */
static uint8    __GucDistance[5]                  = {0};                /*  ��¼������״̬              */
uint16   GusFreq_F                         = 36200;   //33.8,33,327        /*  ǰ������Ƶ��              */
uint16   GusFreq_FJ                        = 19200;   //26.3,266,275              /*  ǰ���������Ƶ��              */
uint16   GusFreq_X                         = 30000;   //35,33.8          /*  б45�Ⱥ���Ƶ��              */
uint16   GusFreq_LF                        = 31700;   //34000           /*  ���Һ���Զ��Ƶ��              */
uint16   GusFreq_L                         = 18300;              /*  ���Һ������Ƶ��              */

unsigned short int GusWidthFrontLeftNear  = 1894/2;						//ǰ�������
unsigned short int GusWidthFrontLeftFar   = 1894/2;						//ǰ����Զ��

unsigned short int GusWidthFrontRightNear  = 1894/2;						//ǰ���ҽ���
unsigned short int GusWidthFrontRightFar   = 1894/2;						//ǰ����Զ��


unsigned short int GusWidthRightNear  = 1894/2;								//�ҽ���
unsigned short int GusWidthRightFar   = 1894/2;								//��Զ��

unsigned short int GusWidthLeftNear  = 1894/2;									//�����
unsigned short int GusWidthLeftFar   = 1894/2;									//��Զ��


unsigned short int GusWidthXieLeft   = 1894/2;									//б45����
unsigned short int GusWidthXieRight  = 1894/2;									//б45����

//short int StaticAngleSpeed = 0;
short int MaxStaticAngleSpeed = 0;
short int MinStaticAngleSpeed = 0;

unsigned char LedMouseTast = 0;

static  int16   GsTpusle_T                       = 0;                  /*  ����У�����ٵ��ٶ�ֵ              */
static uint8    GuiSpeedCtr                       = 0;
static uint16   GuiTpusle_LR                      = 0;
static uint16   GuiTpusle_LR_R                      = 0;
static uint16   GuiTpusle_LR_L                      = 0;
static uint16   GuiTpusle_S_L                       = 0;
static uint16   GuiTpusle_S_R                       = 0;
static uint8    GucFrontNear                      = 0;
uint8    GucFangXiang                      = 0;
uint8    GucDirTemp                        = 0;
uint8    kang,hang ;
// uint8    SEARCHMETHOD;
uint8 Tab=0;//�������ѡ��
unsigned short int SystemVolt = 0;

signed  int SpeedRight;
signed  int SpeedLeft;
unsigned int debug_1maichongyizouL = 0;
unsigned int debug_1maichongyizouR = 0;


void __delay (uint32  uiD)
{
    for (; uiD; uiD--);
}
/*********************************************************************************************************
** Function name:       PIDInit
** Descriptions:        PID��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void PIDInit(void) 
{  
    __GmLPID.usEncoder_new = 0;
    __GmLPID.usEncoder_last = 32768;
    __GmLPID.usFeedBack = 0 ;  //�ٶȷ���ֵ
    __GmLPID.sFeedBack = 0 ;
    
    __GmRPID.usEncoder_new = 0;
    __GmRPID.usEncoder_last = 32768;
    __GmRPID.usFeedBack = 0 ;  //�ٶȷ���ֵ
    __GmRPID.sFeedBack = 0 ;
    
    __GmSPID.sRef = 0 ;        //�ٶ��趨ֵ 
    __GmSPID.sFeedBack = 0 ;        
    __GmSPID.sPreError = 0 ;   //ǰһ�Σ��ٶ����,,vi_Ref - vi_FeedBack 
    __GmSPID.sPreDerror = 0 ;   //ǰһ�Σ��ٶ����֮�d_error-PreDerror; 
        
    __GmSPID.fKp = __KP; 
    __GmSPID.fKi = __KI;
    __GmSPID.fKd = __KD; 
       
    __GmSPID.iPreU = 0 ;      //����������ֵ 
    
    __GmWPID.sRef = 0 ;        //�ٶ��趨ֵ 
    __GmWPID.sFeedBack = 0 ;       
    __GmWPID.sPreError = 0 ;   //ǰһ�Σ��ٶ����,,vi_Ref - vi_FeedBack 
    __GmWPID.sPreDerror = 0 ;   //ǰһ�Σ��ٶ����֮�d_error-PreDerror; 
    
    __GmWPID.fKp = __KP;  //30
    __GmWPID.fKi = __KI;  //0.1,0.01
    __GmWPID.fKd = __KD; 
       
    __GmWPID.iPreU = 0 ;      //����������ֵ 
    
}
/*********************************************************************************************************
** Function name:       __Encoder
** Descriptions:        �ɼ����������������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __Encoder(void)                                          
{
     __GmLPID.usEncoder_new = TIM1->CNT;						//������ȡ��ǰֵ
		 //SpeedLeft = __GmLPID.usEncoder_new;
		 TIM1->CNT = 32768;															//�����������ֵ��λ
	   __GmRPID.usEncoder_new = TIM4->CNT;						//��ȡ�ҵ����ǰ����ֵ
	   //SpeedRight = __GmRPID.usEncoder_new ;
	   TIM4->CNT = 32768;															//�ҵ���������ֵ��λ
	
		 __GmLPID.sFeedBack = (__GmLPID.usEncoder_new - 32768 + __GmLPID.usEncoder_last)/4;  //   			//�ٶ�ֵ ��Ϊ����ת Ϊ����ת
	   __GmLPID.usEncoder_last = (__GmLPID.usEncoder_new - 32768)%4;    //%4  Ϊ�������ֵ
	   //TIM1->CNT +=  (__GmLPID.usEncoder_new - 32768)%4;
		 SpeedLeft = __GmLPID.sFeedBack;
	   if(__GmLPID.sFeedBack >= 0)
		 {
			 __GmLeft.cRealDir = __MOTORGOAHEAD;						//����״̬Ϊ��ת
			 __GmLeft.uiPulseCtr += __GmLPID.sFeedBack;		//������������
			}
			else
			{
				__GmLeft.cRealDir = __MOTORGOBACK;						//����״̬Ϊ��ת
				__GmLeft.uiPulseCtr -= __GmLPID.sFeedBack;		//������������ Ϊ��
			}
			__GmRPID.sFeedBack = (32768 - __GmRPID.usEncoder_new + __GmRPID.usEncoder_last)/4;				//�ҵ���ٶ�ֵ Ϊ����ת Ϊ����ת
			__GmRPID.usEncoder_last = (32768 - __GmRPID.usEncoder_new)%4;
			SpeedRight = __GmRPID.sFeedBack;
			if(__GmRPID.sFeedBack >= 0)
		 {
			 __GmRight.cRealDir = __MOTORGOAHEAD;						//�ҵ��״̬Ϊ��ת
			 __GmRight.uiPulseCtr += __GmRPID.sFeedBack;		//������������
			}
			else
			{
				__GmRight.cRealDir = __MOTORGOBACK;						//�ҵ��״̬Ϊ��ת
				__GmRight.uiPulseCtr -= __GmRPID.sFeedBack;		//������������ Ϊ��
			}
			debug_1maichongyizouL = __GmLeft.uiPulseCtr;
			debug_1maichongyizouR = __GmRight.uiPulseCtr;
		__GmSPID.sFeedBack = (__GmRPID.sFeedBack + __GmLPID.sFeedBack)/2;			//Ϊ�˱���ͳһ ����������ɵ�ֵΪ�ı�
    __GmWPID.sFeedBack = (__GmRPID.sFeedBack - __GmLPID.sFeedBack)/2; 
	  
	   
}
//void __Encoder(void)                                          
//{
//     __GmLPID.usEncoder_new = TIM1->CNT;						//������ȡ��ǰֵ
//		 TIM1->CNT = 32768;															//�����������ֵ��λ
//	   __GmRPID.usEncoder_new = TIM4->CNT;						//��ȡ�ҵ����ǰ����ֵ
//	   TIM4->CNT = 32768;															//�ҵ���������ֵ��λ
//	
//		 __GmLPID.sFeedBack = (__GmLPID.usEncoder_new - 32768)/2;    //  /4 			//�ٶ�ֵ ��Ϊ����ת Ϊ����ת
//	   TIM1->CNT +=  (__GmLPID.usEncoder_new - 32768)%2;
//		 SpeedLeft = __GmLPID.sFeedBack;
//	   if(__GmLPID.sFeedBack >= 0)
//		 {
//			 __GmLeft.cRealDir = __MOTORGOAHEAD;						//����״̬Ϊ��ת
//			 __GmLeft.uiPulseCtr += __GmLPID.sFeedBack;		//������������
//			}
//			else
//			{
//				__GmLeft.cRealDir = __MOTORGOBACK;						//����״̬Ϊ��ת
//				__GmLeft.uiPulseCtr -= __GmLPID.sFeedBack;		//������������ Ϊ��
//			}
//			__GmRPID.sFeedBack = (32768 - __GmRPID.usEncoder_new)/2;				//�ҵ���ٶ�ֵ Ϊ����ת Ϊ����ת
//			TIM4->CNT -= (32768 - __GmRPID.usEncoder_new)%2;
//			SpeedRight = __GmRPID.sFeedBack;
//			if(__GmRPID.sFeedBack >= 0)
//		 {
//			 __GmRight.cRealDir = __MOTORGOAHEAD;						//�ҵ��״̬Ϊ��ת
//			 __GmRight.uiPulseCtr += __GmRPID.sFeedBack;		//������������
//			}
//			else
//			{
//				__GmRight.cRealDir = __MOTORGOBACK;						//�ҵ��״̬Ϊ��ת
//				__GmRight.uiPulseCtr -= __GmRPID.sFeedBack;		//������������ Ϊ��
//			}
//			debug_1maichongyizouL = __GmLeft.uiPulseCtr;
//			debug_1maichongyizouR = __GmRight.uiPulseCtr;
//		__GmSPID.sFeedBack = (__GmRPID.sFeedBack + __GmLPID.sFeedBack)/2;			//Ϊ�˱���ͳһ ����������ɵ�ֵΪ�ı�
//    __GmWPID.sFeedBack = (__GmRPID.sFeedBack - __GmLPID.sFeedBack)/2; 
//	  
//	   
//}
/*********************************************************************************************************
** Function name:       __SPIDContr
** Descriptions:        ֱ��PID����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __SPIDContr(void) 
{ 
    int16  error,d_error,dd_error;
    static uint8   K_I=1;
    error = __GmSPID.sRef - __GmSPID.sFeedBack; // ƫ�����
    d_error = error - __GmSPID.sPreError; 
    dd_error = d_error - __GmSPID.sPreDerror;
    if(error> Deadband)
      error -= Deadband;
    else if(error < -Deadband)
      error += Deadband;
    else
      error = 0;
    if((error > error_IMAX)||(error < -error_IMAX))
      K_I=0;
    else
      K_I=1;
    
    __GmSPID.sPreError = error; //�洢��ǰƫ�� 
    __GmSPID.sPreDerror = d_error;
    
    __GmSPID.iPreU += (int16)(  __GmSPID.fKp * d_error + K_I*__GmSPID.fKi * error  + __GmSPID.fKd*dd_error); 
}
/*********************************************************************************************************
** Function name:       __WPIDContr
** Descriptions:        ��ת����PID����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __WPIDContr(void) 
{ 
    int16  error,d_error,dd_error; 
    static uint8   K_I=1;
    error = __GmWPID.sRef + GsTpusle_T- __GmWPID.sFeedBack; // ƫ����� 
    d_error = error - __GmWPID.sPreError; 
    dd_error = d_error - __GmWPID.sPreDerror;
    if(error> Deadband)
      error -= Deadband;
    else if(error < -Deadband)
      error += Deadband;
    else
      error = 0;
    if((error > error_IMAX)||(error < -error_IMAX))
      K_I=0;
    else
      K_I=1;
    
    __GmWPID.sPreError = error; //�洢��ǰƫ�� 
    __GmWPID.sPreDerror = d_error;
    __GmWPID.iPreU += (int16)(  __GmWPID.fKp * d_error + K_I*__GmWPID.fKi * error  + __GmWPID.fKd*dd_error);
        
}
/*********************************************************************************************************
** Function name:      __PIDContr
** Descriptions:        PID���ƣ�ͨ�����������Ƶ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __PIDContr(void)
{
    __SPIDContr();
    __WPIDContr();
    __GmLeft.sSpeed = __GmSPID.iPreU - __GmWPID.iPreU ;
    if(__GmLeft.sSpeed>=0){
     __GmLeft.cDir=__MOTORGOAHEAD; 
    if( __GmLeft.sSpeed >= U_MAX )   //�ٶ�PID����ֹ���������� 
       __GmLeft.sSpeed = U_MAX;      
    if( __GmLeft.sSpeed <= U_MIN ) //�ٶ�PID����ֹ����������  
       __GmLeft.sSpeed = U_MIN;
    }
    else{
      __GmLeft.cDir=__MOTORGOBACK;
      __GmLeft.sSpeed *=-1; 
    if( __GmLeft.sSpeed >= U_MAX )   //�ٶ�PID����ֹ���������� 
       __GmLeft.sSpeed = U_MAX;      
    if( __GmLeft.sSpeed <= U_MIN ) //�ٶ�PID����ֹ����������  
       __GmLeft.sSpeed = U_MIN;
    }
      
    __GmRight.sSpeed = __GmSPID.iPreU + __GmWPID.iPreU ;
    if(__GmRight.sSpeed>=0){
     __GmRight.cDir=__MOTORGOAHEAD; 
    if( __GmRight.sSpeed >= U_MAX )   //�ٶ�PID����ֹ���������� 
       __GmRight.sSpeed = U_MAX;      
    if( __GmRight.sSpeed <= U_MIN ) //�ٶ�PID����ֹ����������  
       __GmRight.sSpeed = U_MIN;
    }
    else{
      __GmRight.cDir=__MOTORGOBACK;
      __GmRight.sSpeed *=-1; 
    if( __GmRight.sSpeed >= U_MAX )   //�ٶ�PID����ֹ���������� 
       __GmRight.sSpeed = U_MAX;      
    if( __GmRight.sSpeed <= U_MIN ) //�ٶ�PID����ֹ����������  
       __GmRight.sSpeed = U_MIN;
    }
    __rightMotorContr();
    __leftMotorContr();
    
}
/*********************************************************************************************************
** Function name:       __rightMotorContr
** Descriptions:        ��ֱ���������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __rightMotorContr(void)
{
	
// 	TIM8->CCR3 = 200;
//  	TIM8->CCR4=0;						//��ת ��
	
	
	  //SpeedRight  = __GmRight.sSpeed; 
    switch (__GmRight.cDir) 
    {
    case __MOTORGOAHEAD:                                                /*  ��ǰ����                    */
			 	TIM8->CCR3=0;
				TIM8->CCR4=__GmRight.sSpeed;						//��ת ��
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_5, 0);                         /*  ����PWM5������������      */
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_4, __GmRight.sSpeed);          /*  ����PWM4������������      */
        break;

    case __MOTORGOBACK:                                               	  /*  ��󲽽�                    */
				TIM8->CCR3 = __GmRight.sSpeed;
				TIM8->CCR4 = 0;																			//��ת ��
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_4, 0);                         /*  ����PWM4������������      */
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_5, __GmRight.sSpeed);          /*  ����PWM5������������      */
        break;
    case __MOTORGOSTOP:                                                  /*  �����ƶ�                   */
				TIM8->CCR3 = 0;
				TIM8->CCR4 = 0;																									//��ת ��
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_5, 0);                     /*  ����PWM5������������      */
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_4, 0);                     /*  ����PWM4������������      */
        break;

    default:
        break;
    }
    //PWMGenEnable(PWM_BASE, PWM_GEN_2);
}
/*********************************************************************************************************
** Function name:       __leftMotorContr
** Descriptions:        ��ֱ���������
** input parameters:    __GmLeft.cDir :������з���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __leftMotorContr(void)
{
    // SpeedLeft  = __GmLeft.sSpeed; 
	 switch (__GmLeft.cDir) 
    {
    case __MOTORGOAHEAD:                                                /*  ��ǰ����                    */
				TIM8->CCR1 = 0;                
				TIM8->CCR2 = __GmLeft.sSpeed;																									//��ת ��
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 0);                         /*  ����PWM0������������      */
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, __GmLeft.sSpeed);          /*  ����PWM1������������      */
        break;

    case __MOTORGOBACK:                                                 /*  ��󲽽�                    */
				TIM8->CCR1 = __GmLeft.sSpeed;                
				TIM8->CCR2 = 0;																									//��ת �� 
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 0);                         /*  ����PWM1������������      */
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, __GmLeft.sSpeed);          /*  ����PWM0������������      */
        break;
    case __MOTORGOSTOP:                                                  /*  �����ƶ�                   */
			TIM8->CCR1 = 0;                
			TIM8->CCR2 = 0;		
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 0);                     /*  ����PWM0������������      */
//       PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 0);                     /*  ����PWM1������������      */
        break;

    default:
        break;
    }
//     PWMGenEnable(PWM_BASE, PWM_GEN_0);
}
/*********************************************************************************************************
** Function name:       __SpeedUp
** Descriptions:        ��������ٳ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __SpeedUp (void)
{
    uint16 Speed;
    Speed=__GmSPID.sFeedBack;
    if(__GmSPID.sRef<__GiMaxSpeed){
      if(Speed >=__GmSPID.sRef)
      {
        __GmSPID.sRef=__GmSPID.sRef+2;
      }
    }   
}
/*********************************************************************************************************
** Function name:       __SpeedDown
** Descriptions:        ��������ٳ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __SpeedDown (void)
{
    uint16 Speed;
    Speed=__GmSPID.sFeedBack;
    if(__GmSPID.sRef>=MINSPEED){
      if(Speed <=__GmSPID.sRef+2)
      {
       __GmSPID.sRef=__GmSPID.sRef-2;
      }
    }
}




/*********************************************************************************************************
** Function name:       mazeSearch
** Descriptions:        ǰ��N��
** input parameters:    iNblock: ǰ���ĸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseGoahead_C (int8  cNBlock)                                    //����ת����
{
	
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
		
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1600;
           GuiTpusle_S_R  = 0;
					GuiTpusle_S_L=0;
        }
        else
        {
           GuiTpusle_LR = 1600;
          GuiTpusle_S_R  = 0;
					GuiTpusle_S_L=300;
        }
        __GiMaxSpeed = 55;
    }
    else{
        GuiTpusle_LR = 0;
       GuiTpusle_S_R= 0;
			GuiTpusle_S_L=0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
      GuiTpusle_S_R  = 0;
						 GuiTpusle_S_L=0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7200;       //1182(34mm)
        __GmRight.uiPulseCtr = 7200;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    if(cNBlock >7)
    {
      __GiMaxSpeed=120;        //75//120
			TIM2->CCR3 =240;
    }
    else if(cNBlock == 7)
    {
      __GiMaxSpeed=110;						//75//120
			TIM2->CCR3 =210;
    } 	
    else if(cNBlock ==6)
    {
      __GiMaxSpeed=100;						//70//100
		 	TIM2->CCR3 =180;
    }    
    else if(cNBlock == 5)
    {
      __GiMaxSpeed=90;						//64//80
		 	TIM2->CCR3 =150;
    }
    else if(cNBlock == 4)
    {
      __GiMaxSpeed=80;						//56//70
			TIM2->CCR3 =120;
    }
    else if(cNBlock == 3)
    {
      __GiMaxSpeed=70;						//48//50
			TIM2->CCR3 =100;
    }
    else if(cNBlock == 2)
    {
      __GiMaxSpeed=55;
			TIM2->CCR3 =100;
    }
    else
     ;
	
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB-1)//������һ��ʱ��
                  GuiSpeedCtr=__SPEEDUP;
								if(cNBlock<2)
                   TIM2->CCR3 =250;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
				if(cNBlock < 3)
				{
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=55;
						}
				}
        if (cNBlock < 2) 
					{
          if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=55;
						}  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr  + 2000- GuiTpusle_LR;    //3094(89mm)///2800
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr   + 2000- GuiTpusle_LR;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 1820- GuiTpusle_LR;
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr + 1820- GuiTpusle_LR;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
End:     ;
}

void mouseGoahead_C_Llow (int8  cNBlock)                                    //����ת����,����ʮ��·�ڵ��١�������
{
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
     TIM2->CCR3 =250;
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1500;//1500
           GuiTpusle_S_R = 0;
					 GuiTpusle_S_L = 0;
						if( GucDirTemp==1){GW_R=700;;}     //3400
						if( GucDirTemp==3){GW_L=2000;}
				 	 //mouseStop();
           //while(1);
        }
        else
        {
           GuiTpusle_LR = 2000;//2000
            GuiTpusle_S_R = 400;
					 GuiTpusle_S_L = 600;
					if( GucDirTemp==1){GW_R=1600;GuiTpusle_LR_R= 0;}   //3500
						if( GucDirTemp==3){GW_L=2000;GuiTpusle_LR_L=200;}
					
        }
        __GiMaxSpeed = 55;
    }
    else{
        GuiTpusle_LR = 0;
         GuiTpusle_S_R = 0;
			 GuiTpusle_S_L = 0;
        __GiMaxSpeed = 70;
			GuiTpusle_LR_R= 0;
			GuiTpusle_LR_L= 0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
       GuiTpusle_S_R = 0;
						  GuiTpusle_S_L = 0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 9000;       //1182(34mm)
        __GmRight.uiPulseCtr = 9000;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB)
                  GuiSpeedCtr=__SPEEDUP;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
              __GmSPID.sRef=55;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 4300- GuiTpusle_LR + GuiTpusle_LR_L;    //3094(89mm)
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 4300- GuiTpusle_LR + GuiTpusle_LR_L;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 4500- GuiTpusle_LR+GuiTpusle_LR_R;
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 4500- GuiTpusle_LR+GuiTpusle_LR_R;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
    /*
     *  �趨���������õ������ߵ�֧·������λ��
     */
End:    ;
        
}
/*********************************************************************************************************
** Function name:       mazeSearch
** Descriptions:        ǰ��N��
** input parameters:    iNblock: ǰ���ĸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseGoahead_C2 (int8  cNBlock)                                    //����ת����
{
	
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
		
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1600;
           GuiTpusle_S_R  = 0;
					GuiTpusle_S_L=0;
        }
        else
        {
           GuiTpusle_LR = 1600;
          GuiTpusle_S_R  = 300;
					GuiTpusle_S_L=300;
        }
        __GiMaxSpeed = 55;
    }
    else{
        GuiTpusle_LR = 0;
       GuiTpusle_S_R= 0;
			GuiTpusle_S_L=0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
      GuiTpusle_S_R  = 0;
						 GuiTpusle_S_L=0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7200;       //1182(34mm)
        __GmRight.uiPulseCtr = 7200;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    if(cNBlock >7)
    {
      __GiMaxSpeed=130;        //75//120
	
    }
    else if(cNBlock == 7)
    {
      __GiMaxSpeed=120;						//75//120
	
    } 	
    else if(cNBlock ==6)
    {
      __GiMaxSpeed=110;						//70//100
		
    }    
    else if(cNBlock == 5)
    {
      __GiMaxSpeed=100;						//64//80
		
    }
    else if(cNBlock == 4)
    {
      __GiMaxSpeed=100;						//56//70
		
    }
    else if(cNBlock == 3)
    {
      __GiMaxSpeed=90;						//48//50
		
    }
    else if(cNBlock == 2)
    {
      __GiMaxSpeed=80;
	
    }
    else
     ;
	
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB-1)//������һ��ʱ��
                  GuiSpeedCtr=__SPEEDUP;
								if(cNBlock<2)
									;
             //      TIM2->CCR3 =350;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
if(cNBlock < 9)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=140;
						}
				 }
			  if(cNBlock < 3)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=120;
						}
				 }
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
             __GmSPID.sRef=70;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {        
									
                __GmRight.uiPulse = __GmRight.uiPulseCtr  + 3500- GuiTpusle_LR;    //3094(89mm)///2800
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr   + 3500- GuiTpusle_LR;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {      
						
							//	mouseStop();while(1);							
                __GmRight.uiPulse = __GmRight.uiPulseCtr +3500- GuiTpusle_LR;
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr +3500- GuiTpusle_LR;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
End:     ;
}
void mouseGoahead_C2_Llow (int8  cNBlock)                                    //����ת����,����ʮ��·�ڵ��١�������
{
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
    // TIM2->CCR3 =300;
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1500;//1500
           GuiTpusle_S_R = 0;
					 GuiTpusle_S_L = 0;

        }
        else
        {
           GuiTpusle_LR = 1900;//100
            GuiTpusle_S_R = 300;//400
					
        }
        __GiMaxSpeed = 55;
    }
    else{
        GuiTpusle_LR = 0;
         GuiTpusle_S_R = 0;
			 GuiTpusle_S_L = 0;
        __GiMaxSpeed = 70;
			GuiTpusle_LR_R= 0;
			GuiTpusle_LR_L= 0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
       GuiTpusle_S_R = 0;
						  GuiTpusle_S_L = 0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7000;       //1182(34mm)
        __GmRight.uiPulseCtr = 7000;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB)
                  GuiSpeedCtr=__SPEEDUP;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }if(cNBlock < 9)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=140;
						}
				 }if(cNBlock < 5)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=100;
						}
				 }
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
              __GmSPID.sRef=60;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 4500- GuiTpusle_LR + GuiTpusle_LR_L+GuiTpusle_S_R;    //3094(89mm)
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 4500- GuiTpusle_LR + GuiTpusle_LR_L+GuiTpusle_S_R;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {  
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 4500- GuiTpusle_LR+GuiTpusle_LR_R;
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 4500- GuiTpusle_LR+GuiTpusle_LR_R;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
    /*
     *  �趨���������õ������ߵ�֧·������λ��
     */
End:    ;
        
}
void mouseGoahead_C1 (int8  cNBlock)                                    //����ת����
{
	
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
		
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1600;
           GuiTpusle_S_R  = 0;
					GuiTpusle_S_L=0;
        }
        else
        {
           GuiTpusle_LR = 1600;
          GuiTpusle_S_R  = 300;
					GuiTpusle_S_L=300;
        }
        __GiMaxSpeed = 55;
    }
    else{
        GuiTpusle_LR = 0;
       GuiTpusle_S_R= 0;
			GuiTpusle_S_L=0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
      GuiTpusle_S_R  = 0;
						 GuiTpusle_S_L=0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7200;       //1182(34mm)
        __GmRight.uiPulseCtr = 7200;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    if(cNBlock >7)
    {
      __GiMaxSpeed=130;        //75//120
	
    }
    else if(cNBlock == 7)
    {
      __GiMaxSpeed=120;						//75//120
	
    } 	
    else if(cNBlock ==6)
    {
      __GiMaxSpeed=110;						//70//100
		
    }    
    else if(cNBlock == 5)
    {
      __GiMaxSpeed=100;						//64//80
		
    }
    else if(cNBlock == 4)
    {
      __GiMaxSpeed=100;						//56//70
		
    }
    else if(cNBlock == 3)
    {
      __GiMaxSpeed=90;						//48//50
		
    }
    else if(cNBlock == 2)
    {
      __GiMaxSpeed=80;
	
    }
    else
     ;
	
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB-1)//������һ��ʱ��
                  GuiSpeedCtr=__SPEEDUP;
								if(cNBlock<2)
									;
             //      TIM2->CCR3 =350;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
if(cNBlock < 9)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=150;
						}
				 }
			  if(cNBlock < 3)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=120;
						}
				 }
        if (cNBlock == 1) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
             __GmSPID.sRef=70;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {        
									
                __GmRight.uiPulse = __GmRight.uiPulseCtr  + 1850- GuiTpusle_LR;    //3094(89mm)///2800
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr   + 1850- GuiTpusle_LR;           //2350
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {      
						
							//	mouseStop();while(1);							
                __GmRight.uiPulse = __GmRight.uiPulseCtr +1100- GuiTpusle_LR;             
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr +1100- GuiTpusle_LR;                //2250
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
End:     ;
}
void mouseGoahead_h (int8  cNBlock)                                    //����ת����
{
	
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
		
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1600;
           GuiTpusle_S_R  = 0;
					GuiTpusle_S_L=0;
        }
        else
        {
           GuiTpusle_LR = 1600;
          GuiTpusle_S_R  = 300;
					GuiTpusle_S_L=300;
        }
        __GiMaxSpeed = 30;
    }
    else{
        GuiTpusle_LR = 0;
       GuiTpusle_S_R= 0;
			GuiTpusle_S_L=0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
      GuiTpusle_S_R  = 0;
						 GuiTpusle_S_L=0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7200;       //1182(34mm)
        __GmRight.uiPulseCtr = 7200;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    if(cNBlock >7)
    {
      __GiMaxSpeed=95;        //75//120
		//	TIM2->CCR3 =240;
    }
    else if(cNBlock == 7)
    {
      __GiMaxSpeed=95;						//75//120
		//	TIM2->CCR3 =210;
    } 	
    else if(cNBlock ==6)
    {
      __GiMaxSpeed=85;						//70//100
		 //	TIM2->CCR3 =180;
    }    
    else if(cNBlock == 5)
    {
      __GiMaxSpeed=85;						//64//80
		// 	TIM2->CCR3 =150;
    }
    else if(cNBlock == 4)
    {
      __GiMaxSpeed=75;						//56//70
		//	TIM2->CCR3 =120;
    }
    else if(cNBlock == 3)
    {
      __GiMaxSpeed=75;						//48//50
		//	TIM2->CCR3 =100;
    }
    else if(cNBlock == 2)
    {
      __GiMaxSpeed=70;
		//	TIM2->CCR3 =100;
    }
    else
     ;
	
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB-1)//������һ��ʱ��
                  GuiSpeedCtr=__SPEEDUP;
								if(cNBlock<2)
             ;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
			  if(cNBlock < 3)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=55;
						}
				 }
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
             __GmSPID.sRef=55;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr  + 3600- GuiTpusle_LR;    //3094(89mm)///2800
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr   + 3600- GuiTpusle_LR;
							
							
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 500) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 3600- GuiTpusle_LR;
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr + 3600- GuiTpusle_LR;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 500) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
End:     ;
}



void mouseGoahead_C1_Llow (int8  cNBlock)                                    //����ת����,����ʮ��·�ڵ��١�������
{
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
    // TIM2->CCR3 =300;
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1500;//1500
           GuiTpusle_S_R = 0;
					 GuiTpusle_S_L = 0;

        }
        else
        {
           GuiTpusle_LR = 1900;//100
            GuiTpusle_S_R = 300;//400
					
        }
        __GiMaxSpeed = 55;
    }
    else{
        GuiTpusle_LR = 0;
         GuiTpusle_S_R = 0;
			 GuiTpusle_S_L = 0;
        __GiMaxSpeed = 70;
			GuiTpusle_LR_R= 0;
			GuiTpusle_LR_L= 0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
       GuiTpusle_S_R = 0;
						  GuiTpusle_S_L = 0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7000;       //1182(34mm)
        __GmRight.uiPulseCtr = 7000;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB)
                  GuiSpeedCtr=__SPEEDUP;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }if(cNBlock < 9)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=155;        //155
						}
				 }if(cNBlock < 5)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=110;          //110
						}
				 }				 if((GucCrossroad <= 1)&&(cNBlock>1))
				  {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=80;
						}
				 }
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
              __GmSPID.sRef=60;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 3700- GuiTpusle_LR + GuiTpusle_LR_L+GuiTpusle_S_R;    //3094(89mm)//4000
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 3700- GuiTpusle_LR + GuiTpusle_LR_L+GuiTpusle_S_R;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {  
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 3600- GuiTpusle_LR+GuiTpusle_LR_R;//4200//3500
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 3600- GuiTpusle_LR+GuiTpusle_LR_R;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
    /*
     *  �趨���������õ������ߵ�֧·������λ��
     */
End:    ;
        
}
void mouseGoahead_hLlow (int8  cNBlock)           //����ת����,����ʮ��·�ڵ��١�������
{
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
   //  TIM2->CCR3 =300;         //��������
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1500;//1500
           GuiTpusle_S_R = 0;
					 GuiTpusle_S_L = 0;
						if( GucDirTemp==1){GW_R=700;;}     //3400
						if( GucDirTemp==3){GW_L=2000;}

        }
        else
        {
           GuiTpusle_LR = 2000;//2000
            GuiTpusle_S_R = 400;
					 GuiTpusle_S_L = 600;
					if( GucDirTemp==1){GW_R=1600;GuiTpusle_LR_R= 0;}   //3500
						if( GucDirTemp==3){GW_L=2000;GuiTpusle_LR_L=200;}

        }
        __GiMaxSpeed = 30;    //�ص��ٶ�
    }
    else{
        GuiTpusle_LR = 0;
         GuiTpusle_S_R = 0;
			 GuiTpusle_S_L = 0;
        __GiMaxSpeed = 70;
			GuiTpusle_LR_R= 0;
			GuiTpusle_LR_L= 0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
       GuiTpusle_S_R = 0;
						  GuiTpusle_S_L = 0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7000;       //1182(34mm)
        __GmRight.uiPulseCtr = 7000;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB)
                  GuiSpeedCtr=__SPEEDUP;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
              __GmSPID.sRef=55;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 5600- GuiTpusle_LR + GuiTpusle_LR_L;    //3094(89mm)
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 5600- GuiTpusle_LR + GuiTpusle_LR_L;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 5600- GuiTpusle_LR+GuiTpusle_LR_R;
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 5600- GuiTpusle_LR+GuiTpusle_LR_R;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
    /*
     *  �趨���������õ������ߵ�֧·������λ��
     */
End:    ;
        
}



void mouseGoahead_C3 (int8  cNBlock) 
{
	
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
		
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1600;
           GuiTpusle_S_R  = 0;
					GuiTpusle_S_L=0;
        }
        else
        {
           GuiTpusle_LR = 1600;
          GuiTpusle_S_R  = 300;
					GuiTpusle_S_L=300;
        }
        __GiMaxSpeed = 60;
    }
    else{
        GuiTpusle_LR = 0;
       GuiTpusle_S_R= 0;
			GuiTpusle_S_L=0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
      GuiTpusle_S_R  = 0;
						 GuiTpusle_S_L=0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7200;       //1182(34mm)
        __GmRight.uiPulseCtr = 7200;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    if(cNBlock >7)
    {
      __GiMaxSpeed=150;        //75//120
		
    }
    else if(cNBlock == 7)
    {
      __GiMaxSpeed=140;						//75//120
	
    } 	
    else if(cNBlock ==6)
    {
      __GiMaxSpeed=120;						//70//100
		
    }    
    else if(cNBlock == 5)
    {
      __GiMaxSpeed=90;						//64//80
	
    }
    else if(cNBlock == 4)
    {
      __GiMaxSpeed=60;						//56//70
		
    }
    else if(cNBlock == 3)
    {
      __GiMaxSpeed=60;						//48//50
		
    }
    else if(cNBlock == 2)
    {
      __GiMaxSpeed=60;

    }
    else
     ;
	
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB-1)//������һ��ʱ��
                  GuiSpeedCtr=__SPEEDUP;
								if(cNBlock<4)
                ;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
if(cNBlock < 9)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=160;
						}
				 }
			  if(cNBlock < 3)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=100;
						}
				 }
        if (cNBlock == 1) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
             __GmSPID.sRef=70;
          }   
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr  +1200- GuiTpusle_LR;    //1200
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr   +1200- GuiTpusle_LR;     //1200
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 1250- GuiTpusle_LR;      //1250
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr + 1250- GuiTpusle_LR;       //1250
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
End:     ;
}

 void mouseGoahead_C3_Llow (int8  cNBlock)
{
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
     TIM2->CCR3 =250;
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1500;//1500
           GuiTpusle_S_R = 0;
					 GuiTpusle_S_L = 0;
						if( GucDirTemp==1){GW_R=0;;}     //3400
						if( GucDirTemp==3){GW_L=0;}
				 
        }
        else
        {
           GuiTpusle_LR = 2000;//2000
            GuiTpusle_S_R = 0;          //400
					 GuiTpusle_S_L = 0;           //600
					if( GucDirTemp==1){GW_R=0;GuiTpusle_LR_R= 0;}   //500
						if( GucDirTemp==3){GW_L=0;GuiTpusle_LR_L=0;}    //200

					
        }
        __GiMaxSpeed = 60;
    }
    else{
        GuiTpusle_LR = 0;
         GuiTpusle_S_R = 0;
			 GuiTpusle_S_L = 0;
        __GiMaxSpeed = 70;
			GuiTpusle_LR_R= 0;
			GuiTpusle_LR_L= 0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
       GuiTpusle_S_R = 0;
						  GuiTpusle_S_L = 0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 9000;       //1182(34mm)
        __GmRight.uiPulseCtr = 9000;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB)
                  GuiSpeedCtr=__SPEEDUP;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
if(cNBlock < 9)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=150;
						}
				 }if(cNBlock < 9)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=100;
						}
				 }
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
              __GmSPID.sRef=60;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 3600- GuiTpusle_LR + GuiTpusle_LR_L;    //3600(89mm)       
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 3600- GuiTpusle_LR + GuiTpusle_LR_L;    //3600
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 3800- GuiTpusle_LR+GuiTpusle_LR_R;     //3800
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 3800- GuiTpusle_LR+GuiTpusle_LR_R;     //3800
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
    /*
     *  �趨���������õ������ߵ�֧·������λ��
     */
End:    ;
        
}

/*********************************************************************************************************
** Function name:       mazeSearch
** Descriptions:        ǰ��N��
** input parameters:    iNblock: ǰ���ĸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mazeSearch(void)                  //��������ת��
{
    int8 cL = 0, cR = 0, cCoor = 1;
    //GusFreq_FJ = 17600;    //342
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    /*
     *  �趨��������
     */
		
     if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 8000;    
        __GmRight.uiPulseCtr = 8000;
        cL = 1;
        cR = 1;
        if((__GucDistance[__FRONT]!=0)||((__GucDistance[ __LEFT] & 0x01) == 0)||((__GucDistance[__RIGHT] & 0x01) == 0)){
           if((__GucMouseState==__TURNRIGHT)&&((__GucDistance[ __LEFT] & 0x01) == 0))
					 {GuiTpusle_LR =1000;
            SGW_L=0;UL=170;}    
					 else if ((__GucMouseState==__TURNLEFT)&&((__GucDistance[__RIGHT] & 0x01) == 0))
					 {GuiTpusle_LR = 1100;
            SGW_R=0;UR=200;}
          else if((__GucMouseState==__TURNLEFT)&&((__GucDistance[__LEFT] & 0x01) == 0))
					{GuiTpusle_LR = 1700;  
					}
					else if((__GucMouseState==__TURNRIGHT)&&((__GucDistance[__RIGHT] & 0x01) == 0))
					{ GuiTpusle_LR = 1460;
         }
          else 
					GuiTpusle_LR = 1500;    }
        else{
          GuiTpusle_LR =0; 
        }
    }
    else{
      GuiTpusle_LR =0;
    }
    __GucMouseState   = __GOAHEAD;
    __GiMaxSpeed      =   SEARCHSPEED;       
    __GmRight.uiPulse =   MAZETYPE * ONEBLOCK;
    __GmLeft.uiPulse  =   MAZETYPE * ONEBLOCK;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
     GuiSpeedCtr=__SPEEDUP;
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��  */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
              if((__GucDistance[__FRONT])&&(__GucDistance[ __LEFT] & 0x01)&&(__GucDistance[__RIGHT] & 0x01)){
                GucFrontNear=1;                       //ָ���ǽ���������ͬ��ǰ�������Ҷ��е���
                goto End;
              }
              __mouseCoorUpdate();                                    /*  ��������  */
            } else {
                cCoor = 1;
            }
        }
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��  */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
        
				
				/*
				��mazeSearch()�����У�__GmLeft.uiPulse��__GmRight.uiPulse��ʾ���ǵ�����ת��ǰ���еľ��롣
				*/
        if (cL) {                                                       /*  �Ƿ�����������    */
            if ((__GucDistance[ __LEFT] & 0x01) == 0) {                 /*  �����֧·����������    */
            /**/
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 3200 - GuiTpusle_LR-UL;      //1500///2300//2600
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 3200 - GuiTpusle_LR-UL;
							  UL=0;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) {
                 
                    if ((__GmLeft.uiPulseCtr + 550) > __GmLeft.uiPulse) {
                      GucFrontNear=0;
                        goto End;
                    }
                }
                __GmRight.uiPulse = MAZETYPE * ONEBLOCK;
                __GmLeft.uiPulse  = MAZETYPE * ONEBLOCK;
                GuiSpeedCtr=__SPEEDUP;
            }
        } else {                                                        /*  �����ǽʱ��ʼ����������  */
            if ( __GucDistance[ __LEFT] & 0x01) {
                cL = 1;
            }
						
        }
				
        if (cR) {                                                       /*  �Ƿ���������ұ�   */
            if ((__GucDistance[__RIGHT] & 0x01) == 0) {                 /*  �ұ���֧·����������  */
            
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 3000 - GuiTpusle_LR+UR;     //1600
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 3000 - GuiTpusle_LR+UR;
							  UR=0;
                while ((__GucDistance[ __RIGHT] & 0x01) == 0) {
                 
                    if ((__GmLeft.uiPulseCtr + 500) > __GmLeft.uiPulse) {
                                       
											GucFrontNear=0;
                
                        goto End;
                    }
                }
                __GmRight.uiPulse = MAZETYPE * ONEBLOCK;
                __GmLeft.uiPulse  = MAZETYPE * ONEBLOCK;
                GuiSpeedCtr=__SPEEDUP;
            }
        } else {
            if ( __GucDistance[__RIGHT] & 0x01) {                       /*  �ұ���ǽʱ��ʼ��������ұ�  */
                cR = 1;
            }
        }
    }
End:   
        __mouseCoorUpdate();                                            /*  ��������  */
    
}





/*********************************************************************************************************
** Function name:       __irSendFreq
** Descriptions:        ���ͺ����ߡ�
** input parameters:    __uiFreq:  �����ߵ���Ƶ��
**                      __cNumber: ѡ����Ҫ���õ�PWMģ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __irSendFreq (uint32  __uiFreq, int8  __cNumber)
{
		__uiFreq = 72000000/38000;
    switch (__cNumber) {

    case 1: 
                                                            /*ǰ������*/
						Time3PeriodSet(__uiFreq);																	/*Period*/
						Time3WidthSet( __uiFreq / 2);
                 
        break;

    case 2:                                                            /*���Һ���*/
			
				Time5PeriodSet(__uiFreq);																	/*Period*/
				Time5WidthSet( __uiFreq / 2);
        break;
    case 3:                                                            /*б45����*/
				Time5PeriodSet(__uiFreq);																	/*Period*/
				Time5WidthSet( __uiFreq / 2);
        break;
    default:
        break;
    }
}
/*
#define FrontLeftPosition  0
#define LeftPosition       1
#define XieLeftPosition    2
#define XieRightPosition   3
#define RightPosition      4
#define FrontRightPosition 5
*/
void __irSendWidth (unsigned short int  Width, int8  __cNumber)
{
	switch(__cNumber)
	{
		case FrontLeftPosition:
			FrontLeftWidthSet(Width);
		break;
		case LeftPosition:
			LeftWidthSet(Width);
		break;
		case XieLeftPosition:
			XieLeftWidthSet(Width);
		break;
		case XieRightPosition:
			XieRightWidthSet(Width);
		break;
		case RightPosition:
			RightWidthSet(Width);
		break;
		case FrontRightPosition:
			FrontRightWidthSet(Width);
		break;
		default :
			
		break;
	}
}


/*********************************************************************************************************
** Function name:       __irCheck
** Descriptions:        �����ߴ�������⡣
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __irCheck (void)
{
    static uint8 ucState = 0;

    switch (ucState) {

    case 0:
				__irSendWidth(GusWidthXieLeft,XieLeftPosition);						//б��ռ�ձ�����
				__irSendWidth(GusWidthXieRight,XieRightPosition);					//б�ҷ�ռ�ձ�����
				
				__irSendWidth(GusWidthRightNear,RightPosition);						//�ҷ�����
				__irSendWidth(GusWidthLeftNear,LeftPosition);						  //�󷽽���
        //__irSendFreq(GusFreq_X, 3);                                          /*  ����б45�Ƚ��ϵĴ��������      */
       // __irSendFreq(GusFreq_L, 2);                                         /*  ̽�������������    30000        */
        break;
        
    case 1:
        //ucIRCheck1 = GPIOPinRead(GPIO_PORTB_BASE, 0x30);                 /*  ��ȡб45�ȴ�����״̬         */
        //ucIRCheck = GPIOPinRead(GPIO_PORTD_BASE, 0x48);                   /*  ��ȡ���Ҵ�����״̬         */
				
		
				    
        if (X_RightIrState == 1) {																							/*б45����*/
            __GucDistance[__FRONTR]  = 0x00;
        } else {
            __GucDistance[__FRONTR]  = 0x01;
        }
        if (X_LeftIrState == 1) {																								/*б45����*/
            __GucDistance[__FRONTL]  = 0x00;
        } else {
            __GucDistance[__FRONTL]  = 0x01;
        }
        if (RightIrState == 1) {																								/*you��*/
            __GucDistance[__RIGHT]  &= 0xfd;
        } else {
            __GucDistance[__RIGHT]  |= 0x02;
        }
        if (LeftIrState == 1) {																										/*zuo��*/
            __GucDistance[__LEFT]   &= 0xfd;
        } else {
            __GucDistance[__LEFT]   |= 0x02;
        }
				Time5WidthSet(0);

        break;    
    case 2:
       // __irSendFreq(GusFreq_F, 1);                                       /*  �������ǰ����Զ�� 36000 */
		    __irSendWidth(GusWidthFrontLeftFar,FrontLeftPosition);						//ǰ����Զ��
				__irSendWidth(GusWidthFrontRightFar,FrontRightPosition);						//ǰ����Զ��
        break;
        
    case 3:
    
        if ((FrontRightIrState == 1)||(FrontLeftIrState == 1)) {
            __GucDistance[__FRONT] &= 0xfe;                               /*  ǰ���޵��� */
            GucMouseStart = 0;
        } else {
            __GucDistance[__FRONT] |= 0x01;                               /*  ǰ�����ڵ���  */
            GucMouseStart = 1;
        }
				Time3WidthSet(0);
        break;
        
    case 4:
        //__irSendFreq(GusFreq_LF, 2);                                         /*  ������ҷ���Զ��  34000*/
				__irSendWidth(GusWidthRightFar,RightPosition);						//�ҷ�Զ��
				__irSendWidth(GusWidthLeftFar,LeftPosition);						  //��Զ��
        break;
        
    case 5:
   
        if (RightIrState == 1) {
            __GucDistance[__RIGHT] &= 0xfe;
        } else {
            __GucDistance[__RIGHT] |= 0x01;
        }
        if (LeftIrState == 1) {
            __GucDistance[__LEFT]  &= 0xfe;
        } else {
            __GucDistance[__LEFT]  |= 0x01;
        }
				  Time5WidthSet(0);
		//		Time2WidthSet(0);
        break;
        
     case 6:
        //__irSendFreq(GusFreq_FJ, 1);                                       /*  �������ǰ�������  */
				__irSendWidth(GusWidthFrontLeftNear,FrontLeftPosition);						//ǰ�������
				__irSendWidth(GusWidthFrontRightNear,FrontRightPosition);						//ǰ���ҽ���
        break;
        
     case 7:

        if ((FrontRightIrState == 1)||(FrontLeftIrState == 1)) {
            __GucDistance[__FRONT] &= 0xfd;                               /*  ǰ���޵��� */
            GucFrontJinju = 0;
        } else {
            __GucDistance[__FRONT] |= 0x02;                               /*  ǰ�����ڵ���  */
            GucFrontJinju = 1;
        }
				Time3WidthSet(0);
        break;
    default:
        break;
    }
    ucState = (ucState + 1) % 8;                                        /*  ѭ�����                    */
}






void mouseStop(void)
{   
  __GmSPID.sRef=0;
  __GmWPID.sRef=0;
  GuiSpeedCtr=5;  
}
/**************************����ʱ�����ת**************************************************************
** Function name:       mouseTurnright
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��         ������ת��
*********************************************************************************************************/
void mouseTurnright(void)
{  

   while ((__GmRight.uiPulseCtr+150 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+150 ) <= __GmLeft.uiPulse);
// 	  mouseStop();
//    while(1);
   __GmLeft.uiPulse =2900;            // 135*32*1024/(12*25*3.14)  R_r=14mm  R_l=14+72=86 ;4697
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =2900;           // 22*32*1024/(12*25*3.14);
   __GmRight.uiPulseCtr=0;
   __GmSPID.sRef = 48;  //45
   __GmWPID.sRef = -46;   //35
   GW=0;
   __GucMouseState   = __TURNRIGHT; 
   __GmRight.cState =__MOTORRUN;        //???????
   __GmLeft.cState  =__MOTORRUN;  
   GucMouseDir     = (GucMouseDir + 1) % 4;                          /*  ????                    */
   while(1)
   {
       if(GW>33500+SGW_R)   //70200
       {
				 SGW_R=0;
         break;
       }                   
   } 
 
	
//      while(1)
//      {
//      
//           mouseStop();
//      }

   __mazeInfDebug();
   __GmWPID.sRef=0;   
   __GucMouseState   = __GOAHEAD;
   
   GuiSpeedCtr=3;
   __GmLeft.uiPulse = 4000;  // 4000       
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse = 4000;           
   __GmRight.uiPulseCtr=0;

   while ((__GmRight.uiPulseCtr+150 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+150 ) <= __GmLeft.uiPulse);
   __GucMouseState   = __TURNRIGHT;
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;	 	
   #ifdef DEBUG_TURN_STOP_R
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
#endif
}
/***************************����ʱ�����ת******************************************************************
** Function name:       mouseTurnleft
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnleft(void)
{
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);  
	
/*��__GmLeft.uiPulse��__GmRight.uiPulse��ʾ����
�����������ת�������壬�ڸú����У���ʾת��
֮����������еľ��롣  */
   __GmLeft.uiPulse =2950;  // 22*32*1024/(12*25*3.14)  R_l=14mm  R_r=14+72=86         3000
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =2950;           // 135*32*1024/(12*25*3.14)            3000
   __GmRight.uiPulseCtr=0;
	/*
	__GmSPIDԽ�������ٶ�Խ�죻
__GmWPIDԽ��ת��뾶ԽС�� W��ʾת��Ƕȣ�
84000�޸ĵ�Խ�󣬱�ʾת���ĽǶ�Խ��
	
	*/
   __GmSPID.sRef = 48;//45
   __GmWPID.sRef = 38;//35
   GW=0;
   __GucMouseState   = __TURNLEFT; 
   __GmRight.cState = __MOTORRUN;     
   __GmLeft.cState  = __MOTORRUN;
   GucMouseDir     = (GucMouseDir + 3) % 4;          /*  ����ת��              */
		
   while(1)
   {
       if(GW>37700+SGW_L)//53000
       {
				 SGW_L=0;
         break;
       }                      
   }

   
   
//      while(1)
//      {
//      
//           mouseStop();
//      }

   __mazeInfDebug();
  
   __GmWPID.sRef=0;
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse = 4000;      
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse = 4000;           
   __GmRight.uiPulseCtr=0;
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
   
   __GucMouseState   = __TURNLEFT;	// mouseStop();__delay(900000000);
  
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
   #ifdef DEBUG_TURN_STOP_L
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __rightMotorContr();     // ����ֹͣ�ҵ��PWM���
   __leftMotorContr();      // ����ֹͣ����PWM���
   while(1);                // ���ԣ���ת��ɺ�ֹͣ
    #endif

}


void mouseTurnright_Y(void)
{  
   //__delay(10000000);
   
  //__delay(2000000);
   __GmLeft.uiPulse =3200;            // 135*32*1024/(12*25*3.14)  R_r=14mm  R_l=14+72=86 ;4697
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =3200;           // 22*32*1024/(12*25*3.14);
   __GmRight.uiPulseCtr=0;
   __GmSPID.sRef=0;
   __GmWPID.sRef=-12;
   GW=0;
   __GucMouseState   = __TURNRIGHTY; 
   
   //__GmSPID.sRef=0;//
   //__GmWPID.sRef=0;//
   //while(1);//
  // __delay(2000000);
   __GmRight.cState =__MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  =__MOTORRUN;
   GucMouseDir     = (GucMouseDir + 1) % 4;                            /*  ������                    */
    //   mouseStop();//
    //while(1);   //
   
   while(1)
   {
       if(GW>72520)   //81500  
       {
         break;
       }                 
    
   }
	 
   __mazeInfDebug();
   //__delay(10000000);
   __GmWPID.sRef=0;
   __GmSPID.sRef=25;
    //mouseStop();//
   // while(1);   //
   
   
   
   //mouseStop();  //
   //while(1);       //
   //__delay(100000000);   //
   //__GmSPID.sRef=6;    //
    
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   //__GmRight.sSpeed = 0;
    __rightMotorContr();
   // __GmLeft.sSpeed = 0;
    __leftMotorContr();
   __delay(10000);
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
      #ifdef DEBUG_TURN_STOP_R
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
#endif
}

/*********************************************************************************************************
** Function name:       mouseTurnleft
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnleft_Y(void)
{  

   __GmLeft.uiPulse =3000;            // 22*32*1024/(12*25*3.14)  R_l=14mm  R_r=14+72=86
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =3000;           // 135*32*1024/(12*25*3.14)
   __GmRight.uiPulseCtr=0;
   __GmSPID.sRef=0;
   __GmWPID.sRef=12;//12
   GW=0;
   __GucMouseState   = __TURNLEFTY;
   __GmRight.cState = __MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  = __MOTORRUN;
   GucMouseDir     = (GucMouseDir + 3) % 4;                            /*  ������                    */
   while(1)
   {
       if(GW>73500)//64000
       {
         break;
       }                  
   }
   __mazeInfDebug();
   //__delay(10000000);
   __GmWPID.sRef=0;
   __GmSPID.sRef=25;
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   //__GmRight.sSpeed = 0;
    __rightMotorContr();
   // __GmLeft.sSpeed = 0;
    __leftMotorContr();
   __delay(10000);                 ///100000
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
   
#ifdef DEBUG_TURN_STOP_L
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
   #endif
}

/*********************************************************************************************************
** Function name:       mouseTurnleft
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnright_C(void)
{  

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);     
   __GmLeft.uiPulse =6000;            
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =6000;           
   __GmRight.uiPulseCtr=0;
   __GmSPID.sRef = 65;
   __GmWPID.sRef = -45;
   GW=0; 
   __GucMouseState   = __TURNRIGHT;   
   __GmRight.cState =__MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  =__MOTORRUN;
   GucMouseDir     = (GucMouseDir + 1) % 4;                            /*  ������                    */
   
   while(1)
   {
       if(GW>36000+GW_R)   //4300//30000
       {
         break;
       }
   }
//mouseStop();
   __mazeInfDebug();
   __GmWPID.sRef=0;  
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse =2180+ GuiTpusle_S_R;  //1200        
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =2180+GuiTpusle_S_R;           
   __GmRight.uiPulseCtr=0;

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <=__GmLeft.uiPulse);
   __GucMouseState   = __TURNRIGHT;   // mouseStop();__delay(900000000);
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
      #ifdef DEBUG_TURN_STOP_R
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
#endif
}
/*********************************************************************************************************
** Function name:       mouseTurnleft_H
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnright_H(void)
{  

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);     
   __GmLeft.uiPulse =2600;   //2600         
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =2600;           
   __GmRight.uiPulseCtr=0;
   __GmSPID.sRef = 46;//38   
   __GmWPID.sRef = -36;//32  //��תPID
   GW=0;
   __GucMouseState   = __TURNRIGHT;   
   __GmRight.cState =__MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  =__MOTORRUN;
   GucMouseDir     = (GucMouseDir + 1) % 4;                            /*  ������                    */
   
   while(1)
   {
       if(GW>45000+GW_R)   
       {
         break;
       }
   }

   __mazeInfDebug();
   __GmWPID.sRef=0;  
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse =1500+ GuiTpusle_S_R;  //1200        
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =1500+GuiTpusle_S_R;           
   __GmRight.uiPulseCtr=0;
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <=__GmLeft.uiPulse);

   __GucMouseState   = __TURNRIGHT;// mouseStop();__delay(900000000);
	 
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
      #ifdef DEBUG_TURN_STOP_R
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
#endif
}


void mouseTurnright_KC(void)
{  

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);     
   __GmLeft.uiPulse =3000;            
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =3000;           
   __GmRight.uiPulseCtr=0;
   __GmSPID.sRef = 50;
   __GmWPID.sRef = -40;
   GW=0; 
   __GucMouseState   = __TURNRIGHT;   
   __GmRight.cState =__MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  =__MOTORRUN;
   GucMouseDir     = (GucMouseDir + 1) % 4;                            /*  ������                    */
   
   while(1)
   {
       if(GW>33000+GW_R)   //4300
       {
         break;
       }
   }

   __mazeInfDebug();
   __GmWPID.sRef=0;  
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse =1200+ GuiTpusle_S_R;  //1200        
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =1200+GuiTpusle_S_R;           
   __GmRight.uiPulseCtr=0;
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <=__GmLeft.uiPulse);

   __GucMouseState   = __TURNRIGHT;   // mouseStop();__delay(900000000);
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
      #ifdef DEBUG_TURN_STOP_R
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
#endif
}
/*********************************************************************************************************
** Function name:       mouseTurnleft
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
// void mouseTurnright_KC(void)
// {  
// // 	 TIM2->CCR3 = 500;	//���ڷ���ռ�ձ�  
//      static int q=0;
// 	q++;
//    while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
//    while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);     
//    __GmLeft.uiPulse =2600;            
//    __GmLeft.uiPulseCtr=0;
//    __GmRight.uiPulse =2600;           
//    __GmRight.uiPulseCtr=0;
// 	 __GmSPID.sRef = 40;
//    __GmWPID.sRef=-20;
//    GW=0;
//    __GucMouseState   = __TURNRIGHT;   
//    __GmRight.cState =__MOTORRUN;        //�üӵ�����״̬
//    __GmLeft.cState  =__MOTORRUN;
//    GucMouseDir     = (GucMouseDir + 1) % 4;                            /*  ������                    */
//     mouseStop();
//     while(1);
//    while(1)
//    {
//        if(GW>61300+GW_R)   
//        {
//          break;
//        }
//    }
// 	 if(q==1)
// 		 { mouseStop();
//     while(1);}
//    __mazeInfDebug();
//    __GmWPID.sRef=0;  
//    __GucMouseState   = __GOAHEAD;
//    GuiSpeedCtr=3;
//    __GmLeft.uiPulse =1+GuiTpusle_S_R;          
//    __GmLeft.uiPulseCtr=0;
//    __GmRight.uiPulse =1+GuiTpusle_S_R;           
//    __GmRight.uiPulseCtr=0;
//    while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
//    while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
//      
//    __GucMouseState   = __TURNRIGHT;   
//    GuiSpeedCtr=__SPEEDUP;
//    __GmRight.cState = __MOTORSTOP;       
//    __GmLeft.cState  = __MOTORSTOP;
//    __GmRight.uiPulseCtr = 0;
//    __GmLeft.uiPulseCtr = 0;
// }



/*********************************************************************************************************
** Function name:       mouseTurnleft
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnleft_C(void)
{

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
     
   __GmLeft.uiPulse =4000;            
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =4000;          
   __GmRight.uiPulseCtr=0;
   
 __GmSPID.sRef = 65;
   __GmWPID.sRef = 45;
	GW=0;
   __GucMouseState   = __TURNLEFT; 
   __GmRight.cState = __MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  = __MOTORRUN;
   GucMouseDir     = (GucMouseDir + 3) % 4;                            /*  ������                    */
   while(1)
   {
       if(GW>29000+GW_L)//42000//33000//
       {
         break;
       }
     
   }
//mouseStop();
   __mazeInfDebug();
   __GmWPID.sRef=0; 
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse =1980+GuiTpusle_S_L;     //1500        
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =1980+GuiTpusle_S_L;           
   __GmRight.uiPulseCtr=0;
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
  
   __GucMouseState   = __TURNLEFT;	// mouseStop();__delay(90000000);
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
   
#ifdef DEBUG_TURN_STOP_L
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
#endif
}
/*********************************************************************************************************
** Function name:       mouseTurnleft_H
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnleft_H(void)
{

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
     
   __GmLeft.uiPulse =2600;            
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =2600;          
   __GmRight.uiPulseCtr=0;
   
   __GmSPID.sRef = 46;  //38
   __GmWPID.sRef = 36;   //32
	GW=0;
   __GucMouseState   = __TURNLEFT; 
   __GmRight.cState = __MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  = __MOTORRUN;
   GucMouseDir     = (GucMouseDir + 3) % 4;                            /*  ������                    */
	
	

	
   while(1)
   {
       if(GW>46000+GW_L)//85000
       {
         break;
       }
     
   }
//	  mouseStop();__delay(900000000);
	 
   __mazeInfDebug();
   __GmWPID.sRef=0; 
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse =1500+GuiTpusle_S_L;             
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =1500+GuiTpusle_S_L;           
   __GmRight.uiPulseCtr=0;
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
  
   __GucMouseState   = __TURNLEFT;
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
   
#ifdef DEBUG_TURN_STOP_L
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
#endif
}

/*********************************************************************************************************
** Function name:       mouseTurnleft
** Descriptions:        ��ת
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnleft_KC(void)
{

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
     
   __GmLeft.uiPulse =3000;            
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =3000;          
   __GmRight.uiPulseCtr=0;
   
 __GmSPID.sRef = 50;
   __GmWPID.sRef = 40;
	GW=0;
   __GucMouseState   = __TURNLEFT; 
   __GmRight.cState = __MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  = __MOTORRUN;
   GucMouseDir     = (GucMouseDir + 3) % 4;                            /*  ������                    */
   while(1)
   {
       if(GW>33000+GW_L)//42000
       {
         break;
       }
     
   }

   __mazeInfDebug();
   __GmWPID.sRef=0; 
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse =1200+GuiTpusle_S_L;     //1500        
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =1200+GuiTpusle_S_L;           
   __GmRight.uiPulseCtr=0;
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
  
   __GucMouseState   = __TURNLEFT;	// mouseStop();__delay(90000000);
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
   
#ifdef DEBUG_TURN_STOP_L
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.cDir = __MOTORGOSTOP;
   __GmLeft.cDir = __MOTORGOSTOP;
   __GmRight.sSpeed = 0;       // ��ʽ�����ٶ�Ϊ0
   __GmLeft.sSpeed = 0;        // ��ʽ�����ٶ�Ϊ0
   __rightMotorContr();
   __leftMotorContr();
   while(1);
#endif
}
/*********************************************************************************************************
** Function name:       mouseTurnback
** Descriptions:        ����ǰ�����࣬��ת180�ȣ������˺�תǰ�ж�һ�����Ҿ൲��ľ���������
������˳ʱ�뻹����ʱ����ת,liang
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnback(void)
{
    uint8 w=0;
    uint16 q=0;
   if(GucFrontNear)
   {
     while(1)
     {
       if(GucFrontJinju){
         while(GucFrontJinju){
           q++;
           if(q>8){
             w=1;
             break;
           }          
         }
         q=0;
       }
      if(w)
        break;
     }
   }
   else
   {
     __GmRight.uiPulse +=1200;//////gaile5000
     __GmLeft.uiPulse +=1200;/////5000
    while ((__GmRight.uiPulseCtr+150 ) <= __GmRight.uiPulse);
    while ((__GmLeft.uiPulseCtr+150 ) <= __GmLeft.uiPulse);
     
   }
   
    __GmSPID.sRef=0; 
    __GucMouseState   = __TURNBACK; 
   /////gaile __delay(1500000); 
    __GmRight.uiPulse =5000;            
    __GmRight.uiPulseCtr=0;
    __GmLeft.uiPulse =5000; 
    __GmLeft.uiPulseCtr=0;
 
    GW=0;
	 if(((__GucDistance[__RIGHT]==0x03)&&(__GucDistance[__LEFT]==0x01))||
		 ((__GucDistance[__RIGHT]==0x03)&&(__GucDistance[__LEFT]==0x00))){
    __GucMouseState   = 6;
    __GmWPID.sRef=26;
    __GmLeft.cState   = __MOTORRUN;
    __GmRight.cState  = __MOTORRUN;
    GucMouseDir = (GucMouseDir + 2) % 4;                                  
    while(1)
   {
       if(GW>160000) //160000 
       {
         break;
       }                        
   }

	 //mouseStop();
	   __GmWPID.sRef=0;
	   mouseStop();
     __delay(500000);//ת���ͣ
}
 else{
	 __GucMouseState=10;               //   180ת���ٶ�     10
	 __GmWPID.sRef=-40;              //   180ת���ٶ�       30
	 __GmLeft.cState=__MOTORRUN;
	 __GmRight.cState=__MOTORRUN;
	 GucMouseDir=(GucMouseDir+2)%4;
	 
	 while(1)
	 {
			if(GW>130000)      //180ת���ٶ�  ��Ӧ�ĽǶ�  1 60000
			{
				break;
			}
		}
 
		__GmWPID.sRef=0;
		mouseStop();
		__delay(500000);
}
__GmSPID.sRef=45;


__GucMouseState=__GOBACK;
__GmSPID.sRef=-8;
__GmRight.uiPulse=5000;   //
__GmRight.uiPulseCtr=0;
__GmLeft.uiPulse=5000;    //
__GmLeft.uiPulseCtr=0;
while ((__GmRight.uiPulseCtr+150)<=__GmRight.uiPulse);
while((__GmLeft.uiPulseCtr+150)<=__GmLeft.uiPulse);
mouseStop();

	    //  while(1);
     __GucMouseState   = __GOAHEAD ;//mouseStop();__delay(900000000);
    __delay(200000);
  //  delay_ms(300);     
  
    GuiSpeedCtr=__SPEEDUP;
    __GmRight.cState = __MOTORSTOP;       
    __GmLeft.cState  = __MOTORSTOP;
    __GmRight.sSpeed = 0;//5
    __rightMotorContr();
    __GmLeft.sSpeed = 0;//5
    __leftMotorContr();
    __GmRight.uiPulseCtr = 0;
    __GmLeft.uiPulseCtr = 0;
} 
/*********************************************************************************************************
** Function name:       mouseTurnback
** Descriptions:        ����ǰ�����࣬��ת180�ȣ������˺�תǰ�ж�һ�����Ҿ൲��ľ���������
������˳ʱ�뻹����ʱ����ת,liang
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnback_H(void)
{
    uint8 w=0;
    uint16 q=0;
   if(GucFrontNear)
   {
     while(1)
     {
       if(GucFrontJinju){
         while(GucFrontJinju){
           q++;
           if(q>8){
             w=1;
             break;
           }          
         }
         q=0;
       }
      if(w)
        break;
     }
   }
   else
   {
     __GmRight.uiPulse +=1200;//////gaile5000
     __GmLeft.uiPulse +=1200;/////5000
    while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
    while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
     
   }
   
    __GmSPID.sRef=0; 
    __GucMouseState   = __TURNBACK; 
   /////gaile __delay(1500000); 
    __GmRight.uiPulse =6200;            
    __GmRight.uiPulseCtr=0;
    __GmLeft.uiPulse =6200; 
    __GmLeft.uiPulseCtr=0;
 
    GW=0;
	 if(((__GucDistance[__RIGHT]==0x03)&&(__GucDistance[__LEFT]==0x01))||
		 ((__GucDistance[__RIGHT]==0x03)&&(__GucDistance[__LEFT]==0x00))){
    __GucMouseState   = 6;
    __GmWPID.sRef=16;
    __GmLeft.cState   = __MOTORRUN;
    __GmRight.cState  = __MOTORRUN;
    GucMouseDir = (GucMouseDir + 2) % 4;                                  
    while(1)
   {
       if(GW>155100)    // 155100
       {
         break;
       }                        
   }
	 /*
	   �յ�ֹͣ
	 */
//	  mouseStop();
//    while(1);
//	 mouseStop();
	 
	   __GmWPID.sRef=0;
	   mouseStop();
     __delay(3000000);//ת���ͣ
}
 else{
	 __GucMouseState=5;
	 __GmWPID.sRef=-15;    //
	 __GmLeft.cState=__MOTORRUN;
	 __GmRight.cState=__MOTORRUN;
	 GucMouseDir=(GucMouseDir+2)%4;
	 
	 while(1)
	 {
			if(GW>165000)   //
			{
				break;
			}
		}
	 
//		 mouseStop();
//     while(1);
//		 mouseStop();
		
		__GmWPID.sRef=0;
		mouseStop();
		__delay(1000000);
}
__GmSPID.sRef=30;


__GucMouseState=__GOBACK;
__GmSPID.sRef=-8;  //7
__GmRight.uiPulse=5000;  //3000
__GmRight.uiPulseCtr=0;
__GmLeft.uiPulse=5000;  //  3000
__GmLeft.uiPulseCtr=0;
while ((__GmRight.uiPulseCtr+200)<=__GmRight.uiPulse);
while((__GmLeft.uiPulseCtr+200)<=__GmLeft.uiPulse);
mouseStop();
	
     __GucMouseState   = __GOAHEAD ;
    __delay(300000);
    delay_ms(300);     ////gaile300
  
    /////__GmSPID.sRef=30;
   ////// __GmWPID.sRef=0;
    //mouseStop();
    //__delay(3000000);
		
    GuiSpeedCtr=__SPEEDUP;
    __GmRight.cState = __MOTORSTOP;       
    __GmLeft.cState  = __MOTORSTOP;
    __GmRight.sSpeed = 0;//5
    __rightMotorContr();
    __GmLeft.sSpeed = 0;//5
    __leftMotorContr();
    __GmRight.uiPulseCtr = 0;
    __GmLeft.uiPulseCtr = 0;
	 
	 
	 
	 
// 	 
//     __mazeInfDebug();
// 	 
// // 	   mouseStop();
// //      while(1);
// 	 
//     __GmWPID.sRef=0;
//     __delay(500000);
// 	 
//     __GmSPID.sRef=10;
//     __GmWPID.sRef=0;
//     GuiSpeedCtr=__SPEEDUP;
//     __GmRight.cState = __MOTORSTOP;       
//     __GmLeft.cState  = __MOTORSTOP;
//     __GmRight.sSpeed = 0;
//     __rightMotorContr();
//     __GmLeft.sSpeed = 0;
//     __leftMotorContr();
//     __GmRight.uiPulseCtr = 0;
//     __GmLeft.uiPulseCtr = 0;

} 
/****************************************************************************************************************************************
** Function name:       mouseTurnback_Y
** Descriptions:        ԭ����ת180��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mouseTurnback_Y(void)
{
 
  
  /*     __GmRight.uiPulse +=2500;
     __GmLeft.uiPulse +=2500;
    while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);*/
   __delay(90000);
   __GmSPID.sRef=0;
    TIM2->CCR3 = 0;
    __GucMouseState   = __TURNBACK;
    
   
    GW=0;
   if(((__GucDistance[__RIGHT] == 0x03) && (__GucDistance[__LEFT] == 0x01))
       ||((__GucDistance[__RIGHT] == 0x03) && (__GucDistance[__LEFT] == 0x00))){
    __GucMouseState   = 6;
    __GmWPID.sRef=16;
    __GmLeft.cState   = __MOTORRUN;
    __GmRight.cState  = __MOTORRUN;
    GucMouseDir = (GucMouseDir + 2) % 4;                                
    
    while(1)
   {
       if(GW>183000)   //183000    ///gaile155000
       {
         break;
       }                  
       
      
   }
   // __mazeInfDebug();
    __GmWPID.sRef=0;
     mouseStop();
    __delay(1000000);
    }
    else{
    __GucMouseState   = 5;
    __GmWPID.sRef=-16;
    __GmLeft.cState   = __MOTORRUN;
    __GmRight.cState  = __MOTORRUN;
    GucMouseDir = (GucMouseDir + 2) % 4;                                
    
    while(1)
   {
       if(GW>160000)   //183000     /////gaile160000
       {
         break;
       }                  
       
     
   }
    __mazeInfDebug();
    __GmWPID.sRef=0;
    mouseStop();
    __delay(10000);
   } 
   
   
   
   __GucMouseState   = __GOBACK ; //  ????
    __GmSPID.sRef=-8;
     __GmRight.uiPulse =6600;             
     __GmRight.uiPulseCtr=0;
     __GmLeft.uiPulse = 6600;
     __GmLeft.uiPulseCtr=0; 
     while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);    
     while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);    
  
     mouseStop();
    __GucMouseState   = __GOAHEAD ;
    __delay(2000000);   //hou  zhuan */
    delay_ms(300); 
   
    
    
    // __GmSPID.sRef=0;
   // __GmWPID.sRef=0;
    GuiSpeedCtr=__SPEEDUP;
    __GmRight.cState = __MOTORSTOP;       
    __GmLeft.cState  = __MOTORSTOP;
    __GmRight.sSpeed = 0;//5
    __rightMotorContr();
    __GmLeft.sSpeed = 0;//5
    __leftMotorContr();
    //__delay(100000);
    __GmRight.uiPulseCtr = 0;
    __GmLeft.uiPulseCtr = 0;
    //while(1);
}
void mouseGoahead_SC_Llow (int8  cNBlock)                                    //����ת����,����ʮ��·�ڵ��١�������
{
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
    // TIM2->CCR3 =300;
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1500;//1500
           GuiTpusle_S_R = 0;
					 GuiTpusle_S_L = 0;

        }
        else
        {
           GuiTpusle_LR = 1900;//100
            GuiTpusle_S_R = 300;//400
					
        }
        __GiMaxSpeed = 55;
    }
    else{
        GuiTpusle_LR = 0;
         GuiTpusle_S_R = 0;
			 GuiTpusle_S_L = 0;
        __GiMaxSpeed = 70;
			GuiTpusle_LR_R= 0;
			GuiTpusle_LR_L= 0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
       GuiTpusle_S_R = 0;
						  GuiTpusle_S_L = 0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7000;       //1182(34mm)
        __GmRight.uiPulseCtr = 7000;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB)
                  GuiSpeedCtr=__SPEEDUP;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }							  if(cNBlock < 10)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=130;
						}
				 }
				 
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
              __GmSPID.sRef=55;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {                 
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 6000- GuiTpusle_LR + GuiTpusle_LR_L+GuiTpusle_S_R;    //3094(89mm)
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 6000- GuiTpusle_LR + GuiTpusle_LR_L+GuiTpusle_S_R;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 600) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {  
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {                
                __GmRight.uiPulse = __GmRight.uiPulseCtr + 6000- GuiTpusle_LR+GuiTpusle_LR_R;
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr  + 6000- GuiTpusle_LR+GuiTpusle_LR_R;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
    /*
     *  �趨���������õ������ߵ�֧·������λ��
     */
End:    ;
        
}
void mouseGoahead_SC (int8  cNBlock)                                    //����ת����
{
	
    int8 cL = 0, cR = 0, cCoor = 1,cB;
    GuiSpeedCtr=__SPEEDUP;
		
    if (__GmLeft.cState) {
        cCoor = 0;
    }
    if(cNBlock==1)
    {
        cL = 1;
        cR = 1;
        if(GucFangXiang == GucDirTemp)
        {
           GuiTpusle_LR = 1600;
           GuiTpusle_S_R  = 0;
					GuiTpusle_S_L=0;
        }
        else
        {
           GuiTpusle_LR = 1600;
          GuiTpusle_S_R  = 300;
					GuiTpusle_S_L=300;
        }
        __GiMaxSpeed = 55;
    }
    else{
        GuiTpusle_LR = 0;
       GuiTpusle_S_R= 0;
			GuiTpusle_S_L=0;
    }
    GucFangXiang = GucDirTemp;
    if(((GmcMouse.cX==7)&&(GmcMouse.cY==7))|| 
         ((GmcMouse.cX==8)&&(GmcMouse.cY==8))||
         ((GmcMouse.cX==7)&&(GmcMouse.cY==8))||
           ((GmcMouse.cX==8)&&(GmcMouse.cY==7))){
       cL = 0;
       cR = 0;
       GuiTpusle_LR = 0;
      GuiTpusle_S_R  = 0;
						 GuiTpusle_S_L=0;
    }
    if((__GucMouseState==__TURNRIGHT)||(__GucMouseState==__TURNLEFT))
    {
        __GmLeft.uiPulseCtr = 7200;       //1182(34mm)
        __GmRight.uiPulseCtr = 7200;
    }
    
    cB=cNBlock;
    __GucMouseState   = __GOAHEAD;
    __GmRight.uiPulse = __GmRight.uiPulse + cNBlock * ONEBLOCK ;
    __GmLeft.uiPulse  = __GmLeft.uiPulse  + cNBlock * ONEBLOCK ;
    __GmRight.cState  = __MOTORRUN;
    __GmLeft.cState   = __MOTORRUN;
    if(cNBlock >7)
    {
      __GiMaxSpeed=130;        //75//120
	
    }
    else if(cNBlock == 7)
    {
      __GiMaxSpeed=120;						//75//120
	
    } 	
    else if(cNBlock ==6)
    {
      __GiMaxSpeed=110;						//70//100
		
    }    
    else if(cNBlock == 5)
    {
      __GiMaxSpeed=100;						//64//80
		
    }
    else if(cNBlock == 4)
    {
      __GiMaxSpeed=100;						//56//70
		
    }
    else if(cNBlock == 3)
    {
      __GiMaxSpeed=90;						//48//50
		
    }
    else if(cNBlock == 2)
    {
      __GiMaxSpeed=80;
	
    }
    else
     ;
	
    while (__GmLeft.cState != __MOTORSTOP) {
       
        if (__GmLeft.uiPulseCtr >= ONEBLOCK) {                          /*  �ж��Ƿ�����һ��            */
            __GmLeft.uiPulse    -= ONEBLOCK;
            __GmLeft.uiPulseCtr -= ONEBLOCK;
            if (cCoor) {
                cNBlock--;
                if(cNBlock==0)
                   goto End;
                if(cNBlock<cB-1)//������һ��ʱ��
                  GuiSpeedCtr=__SPEEDUP;
								if(cNBlock<2)
									;
             //      TIM2->CCR3 =350;
            } else {
                cCoor = 1;
            }
        }
        
        if (__GmRight.uiPulseCtr >= ONEBLOCK) {                         /*  �ж��Ƿ�����һ��            */
            __GmRight.uiPulse    -= ONEBLOCK;
            __GmRight.uiPulseCtr -= ONEBLOCK;
        }
							  if(cNBlock < 10)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=140;
						}
				 }
				   if(cNBlock < 6)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=130;
						}
				 }
				  if(cNBlock < 6)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=110;
						}
				 }
			  if(cNBlock < 4)
				 {
					 if(__GmSPID.sFeedBack>55)
						{
              GuiSpeedCtr= 3;
             __GmSPID.sRef=90;
						}
				 }
        if (cNBlock < 2) {
          if(__GmSPID.sFeedBack>55){
              GuiSpeedCtr= 3;
             __GmSPID.sRef=70;
          }  
          if (cL) 
          {                                                       /*  �Ƿ�����������            */
            if ((__GucDistance[ __LEFT] & 0x01) == 0)             /*  �����֧·����������        */
            {        
									
                __GmRight.uiPulse = __GmRight.uiPulseCtr  + 3500- GuiTpusle_LR;    //3094(89mm)///2800
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr   + 3500- GuiTpusle_LR;
                while ((__GucDistance[ __LEFT] & 0x01) == 0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {                                                    /*  �����ǽʱ��ʼ����������  */
                if ( __GucDistance[ __LEFT] & 0x01) {
                    cL = 1;
                }
            }
         if (cR) 
            {                                                       /*  �Ƿ���������ұ�            */
            if ((__GucDistance[__RIGHT] & 0x01) == 0)               /*  �ұ���֧·����������        */
            {      
						
							//	mouseStop();while(1);							
                __GmRight.uiPulse = __GmRight.uiPulseCtr +3500- GuiTpusle_LR;
                __GmLeft.uiPulse  = __GmLeft.uiPulseCtr +3500- GuiTpusle_LR;
                while ((__GucDistance[ __RIGHT] & 0x01) ==0) 
                {
                    
                    if ((__GmLeft.uiPulseCtr + 200) > __GmLeft.uiPulse) 
                    {
                        goto End;
                    }
                }
            }
            } else {
                if ( __GucDistance[__RIGHT] & 0x01) {                   /*  �ұ���ǽʱ��ʼ��������ұ�  */
                    cR = 1;
                }
            }
        }
   }
End:     ;
}
void mouseTurnleft_SC(void)
{

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
     
   __GmLeft.uiPulse =6000;            
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =6000;          
   __GmRight.uiPulseCtr=0;
   
 __GmSPID.sRef = 50;
   __GmWPID.sRef = 60;
	GW=0;
   __GucMouseState   = __TURNLEFT; 
   __GmRight.cState = __MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  = __MOTORRUN;
   GucMouseDir     = (GucMouseDir + 3) % 4;                            /*  ������                    */
   while(1)
   {
       if(GW>17000+GW_L)//42000
       {
         break;
       }
     
   }

   __mazeInfDebug();
   __GmWPID.sRef=0; 
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse =3000+GuiTpusle_S_L;     //1500        
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =3000+GuiTpusle_S_L;           
   __GmRight.uiPulseCtr=0;
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);
  
   __GucMouseState   = __TURNLEFT;	// mouseStop();__delay(90000000);
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
   
#ifdef DEBUG_TURN_STOP
while(1)
{
   mouseStop();           // 停止电机
}              // 调试：左转完成后停止，便于观察左转效果
#endif
}
void mouseTurnright_SC(void)
{  

   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);     
   __GmLeft.uiPulse =6000;            
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =6000;           
   __GmRight.uiPulseCtr=0;
   __GmSPID.sRef = 50;
   __GmWPID.sRef = -60;
   GW=0; 
   __GucMouseState   = __TURNRIGHT;   
   __GmRight.cState =__MOTORRUN;        //�üӵ�����״̬
   __GmLeft.cState  =__MOTORRUN;
   GucMouseDir     = (GucMouseDir + 1) % 4;                            /*  ������                    */
   
   while(1)
   {
       if(GW>17000+GW_R)   //4300
       {
         break;
       }
   }

   __mazeInfDebug();
   __GmWPID.sRef=0;  
   __GucMouseState   = __GOAHEAD;
   GuiSpeedCtr=3;
   __GmLeft.uiPulse =3000+ GuiTpusle_S_R;  //1200        
   __GmLeft.uiPulseCtr=0;
   __GmRight.uiPulse =3000+GuiTpusle_S_R;           
   __GmRight.uiPulseCtr=0;
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);
   while ((__GmLeft.uiPulseCtr+200 ) <=__GmLeft.uiPulse);

   __GucMouseState   = __TURNRIGHT;   // mouseStop();__delay(900000000);
   GuiSpeedCtr=__SPEEDUP;
   __GmRight.cState = __MOTORSTOP;       
   __GmLeft.cState  = __MOTORSTOP;
   __GmRight.uiPulseCtr = 0;
   __GmLeft.uiPulseCtr = 0;
}
/*void mouseTurnback_Y(void)
{
   while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);    
   while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse); 
   __GmSPID.sRef=0;  
    __GucMouseState   = __TURNBACK;    
    __delay(1500000);
    __GmRight.uiPulse =5000;             
    __GmRight.uiPulseCtr=0;
    __GmLeft.uiPulse =5000; 
    __GmLeft.uiPulseCtr=0;
    GW=0;
     __GucMouseState   = 6;
    __GmWPID.sRef=16;
    __GmLeft.cState   = __MOTORRUN;
    __GmRight.cState  = __MOTORRUN;
    GucMouseDir = (GucMouseDir + 2) % 4;                                  
    while(1)
   {
       if(GW>113000)   
       {
         break;
       }                        
   }
    __GmSPID.sRef=0;
    __GmWPID.sRef=0;
    GuiSpeedCtr=__SPEEDUP;
	 
    __GmRight.cState = __MOTORSTOP;       
    __GmLeft.cState  = __MOTORSTOP;
    __GmRight.sSpeed = 0;
    __rightMotorContr();
    __GmLeft.sSpeed = 0;
    __leftMotorContr();
    __GmRight.uiPulseCtr = 0;
    __GmLeft.uiPulseCtr = 0;
	 //mouseStop();
}     */
/****************************************************************************************************************************************
** Function name:       mouseTurnback_YY
** Descriptions:        ԭ����ת180��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
// void onestep(void){
// 	__GmRight.uiPulse =7000;             
//     __GmRight.uiPulseCtr=0;
//     __GmLeft.uiPulse =7000; 
//     __GmLeft.uiPulseCtr=0;
//    while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);    
//    while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse);}
void mouseTurnback_YY(void)
{
// 	    onestep();
// 	__GmRight.uiPulse =7000;             
//     __GmRight.uiPulseCtr=0;
//     __GmLeft.uiPulse =7000; 
//     __GmLeft.uiPulseCtr=0;
//    while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);    
//    while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse); 
   __GmSPID.sRef=0;  
    __GucMouseState   = __TURNBACK;    
    __delay(1500000);
    __GmRight.uiPulse =5000;             
    __GmRight.uiPulseCtr=0;
    __GmLeft.uiPulse =5000; 
    __GmLeft.uiPulseCtr=0;
//	while ((__GmRight.uiPulseCtr+200 ) <= __GmRight.uiPulse);    
   //while ((__GmLeft.uiPulseCtr+200 ) <= __GmLeft.uiPulse); 
    GW=0;
     __GucMouseState   = 6;
    __GmWPID.sRef=16;
    __GmLeft.cState   = __MOTORRUN;
    __GmRight.cState  = __MOTORRUN;
    GucMouseDir = (GucMouseDir + 2) % 4;                                  
    while(1)
   {
       if(GW>160000)   
       {
         break;
       }                        
   }
	//  mouseStop();
  //  while(1);
    __GmSPID.sRef=0;
    __GmWPID.sRef=0;
 __GmSPID.sRef=-7;                  /////////////////////
__GmRight.uiPulse=3000;             ///////////////////
__GmRight.uiPulseCtr=0;             //////////////////
__GmLeft.uiPulse=3000;              ///////////////////
__GmLeft.uiPulseCtr=0;              ////////////////////
while ((__GmRight.uiPulseCtr+200)<=__GmRight.uiPulse)          //////////////////////////
while((__GmLeft.uiPulseCtr+200)<=__GmLeft.uiPulse)             //////////////////////////
    GuiSpeedCtr=__SPEEDUP;
	 
    __GmRight.cState = __MOTORSTOP;       
    __GmLeft.cState  = __MOTORSTOP;
    __GmRight.sSpeed = 0;
    __rightMotorContr();
    __GmLeft.sSpeed = 0;
    __leftMotorContr();
    __GmRight.uiPulseCtr = 0;
    __GmLeft.uiPulseCtr = 0;
	 //mouseStop();
}     
/*********************************************************************************************************
** Function name:       __mouseCoorUpdate
** Descriptions:        ���ݵ�ǰ�����������ֵ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __mouseCoorUpdate (void)
{
    switch (GucMouseDir) {

    case 0:
        GmcMouse.cY++;
        break;

    case 1:
        GmcMouse.cX++;
        break;

    case 2:
        GmcMouse.cY--;
        break;

    case 3:
        GmcMouse.cX--;
        break;

    default:
        break;
    }
    __mazeInfDebug();
    __wallCheck();
}


/*********************************************************************************************************
** Function name:       __wallCheck
** Descriptions:        ���ݴ�����������ж��Ƿ����ǽ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      cValue: ����λ������һ�δ�����ǰ�ҡ�1Ϊ��ǽ��0Ϊûǽ��
*********************************************************************************************************/
void __wallCheck (void)
{
    uint8 ucMap = 0;
    uint8 uctemp = 0;
    ucMap |= MOUSEWAY_B;
    
    if (__GucDistance[__LEFT]  & 0x01) {
        ucMap &= ~MOUSEWAY_L;
        uctemp |= 0x06;
    }else {
        ucMap |=  MOUSEWAY_L;
    }
    if (__GucDistance[__FRONT] & 0x01) {
        ucMap &= ~MOUSEWAY_F;
         uctemp |= 0x40;
    }else {
        ucMap |=  MOUSEWAY_F;
    }
    if (__GucDistance[__RIGHT] & 0x01) {
        ucMap &= ~MOUSEWAY_R;
         uctemp |= 0x30;
    }else {
        ucMap |=  MOUSEWAY_R;
    }
    
    
    GucMapBlock0[GmcMouse.cX][GmcMouse.cY]=ucMap;
    GucMapBlock[GmcMouse.cX][GmcMouse.cY]=ucMap;
    GucMapBlock1[GmcMouse.cX][GmcMouse.cY]=ucMap;
    if(GmcMouse.cY<(MAZETYPE-1))
        {GucMapBlock1[GmcMouse.cX][GmcMouse.cY+1] &= ~(((~ucMap)&0x01)*4);}       /*����������Χ����ǽ�����ϸ���  ע����ˮ��*/
       if(GmcMouse.cX<(MAZETYPE-1))
        { GucMapBlock1[GmcMouse.cX+1][GmcMouse.cY]&= ~(((~ucMap)&0x02)*4);}
         if(GmcMouse.cY>0)
         {GucMapBlock1[GmcMouse.cX][GmcMouse.cY-1]&= ~(((~ucMap)&0x04)/4);}
        if(GmcMouse.cX>0)
         {GucMapBlock1[GmcMouse.cX-1][GmcMouse.cY]&= ~(((~ucMap)&0x08)/4);}
              
      if(GmcMouse.cY<(MAZETYPE-1))
         {GucMapBlock[GmcMouse.cX][GmcMouse.cY+1] |=    ((ucMap&0x01)*4);}        /*����������Χ����ǽ�����ϸ���  ע���ڳ�ʼΪ��ǽʱ����*/
      if(GmcMouse.cX<(MAZETYPE-1))
         { GucMapBlock[GmcMouse.cX+1][GmcMouse.cY]|=  ((ucMap&0x02)*4);}
        if(GmcMouse.cY>0)
         {GucMapBlock[GmcMouse.cX][GmcMouse.cY-1]|=  ((ucMap&0x04)/4);}
        if(GmcMouse.cX>0)
          {GucMapBlock[GmcMouse.cX-1][GmcMouse.cY]|=  ((ucMap&0x08)/4);}
    
    zlg7289Download(2, 2, 0, uctemp);
}
/*********************************************************************************************************
** Function name:       SensorDebug
** Descriptions:        ���������ʾ��������״̬���������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void sensorDebug (void)
{
    zlg7289Download(2, 0, 0, __GucDistance[__LEFT  ]);
    zlg7289Download(2, 1, 0, __GucDistance[__FRONTL]);
    zlg7289Download(2, 2, 0, __GucDistance[__FRONT ]);
    zlg7289Download(2, 3, 0, __GucDistance[__FRONTR]);    
    zlg7289Download(2, 4, 0, __GucDistance[__RIGHT ]);
}


/*********************************************************************************************************
** Function name:       __mazeInfDebug
** Descriptions:        ���������ʾ����ǰ������ǰ�����������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __mazeInfDebug (void)
{
    /*
     *  ��ʾ����
     */
    switch (GucMouseDir) {
        
    case 0:
        zlg7289Download(2, 3, 0, 0x47);                                /*  ��ǰ����F��ʾ               */
        break;
        
    case 1:
        zlg7289Download(2, 3, 0, 0x77);                                /*  ���ң���R��ʾ               */
        break;
        
    case 2:
        zlg7289Download(2, 3, 0, 0x1f);                                /*  �����b��ʾ               */
        break;
        
    case 3:
        zlg7289Download(2, 3, 0, 0x0e);                                /*  ������L��ʾ               */
        break;
        
    default :
        zlg7289Download(2, 3, 0, 0x4f);                                /*  ������E��ʾ               */
        break;
    }
    /*
     *  ��ʾ����
     */
    zlg7289Download(1, 0, 0, GmcMouse.cX / 10);
    zlg7289Download(1, 1, 0, GmcMouse.cX % 10);
    zlg7289Download(1, 6, 0, GmcMouse.cY / 10);
    zlg7289Download(1, 7, 0, GmcMouse.cY % 10);
}




uint8 DenggaoCheck (void)
{
  if((__GucDistance[__FRONT])&&(__GucDistance[ __LEFT] & 0x01)&&(__GucDistance[__RIGHT] & 0x01)){
        return(TE);
    }else {
        return(FE);
    }
}
uint8 PulseCtrCheck (void)
{
  if(__GmLeft.uiPulseCtr  > (__GmLeft.uiPulse+2000)){
        return(TE);
    }else {
        return(FE);
    }
}



void GYRO_Z_Angle(void)
{
    short int  w=0;
    w = GetData(0x47);

	 w -= 7;
	if(w < 0)
	{
		GW -= w/16.4;
	}
	else 
	{
		GW += w/16.4;
	}
}

void GetStaticAngle()
{
		unsigned short int i = 0;
	  short int StaticAngleSpeed[1000] = {0};
		for(i = 0;i < 1000;i++)
		{
			StaticAngleSpeed[i] = 0;
		}
		
}



/*
��һ�£�����
�����£�Ƿѹ


*/
void LEDShowMouseState(unsigned char Led_state)
{
	static unsigned int i = 0;
	unsigned int count[10] = {500,250,167,125,101,83,71,63,55,51};
	i++;
	//Led_state = (Led_state + 1) << 1;
	//count = 500 / Led_state;
	if(i == 2000)
		{i = 0; 
			RLED0 = 0;
}
	if(i < 1000)
	{
				if(i % count[Led_state-1] == 0)
					RLED0 = !RLED0;
	}
	//else LED0  = 0;
}


/*********************************************************************************************************
** Function name:       Timer7_ISR
** Descriptions:        Timer7�жϷ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void TIM7_IRQHandler(void)
{
    static int8 n = 0,m = 0,k=0;

		
    //TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);                     /*  �����ʱ��2A�жϡ�  */
	if(TIM7->SR&0X0001)//����ж�
	{
		TIM7->SR&=~(1<<0);//����жϱ�־λ
		
    __Encoder();
    switch (__GmRight.cState) {
        
    case __MOTORSTOP:                                                   /*  ֹͣ��ͬʱ�����ٶȺ�����ֵ  */
        __GmRight.uiPulse    = 0;
        __GmRight.uiPulseCtr = 0;
        __GmLeft.uiPulse    = 0;
        __GmLeft.uiPulseCtr = 0;
		
        break;

    case __WAITONESTEP:                                                 /*  ��ͣһ��  */
        __GmRight.cState = __MOTORRUN;
        if ((__GucDistance[__FRONTL] && (__GucDistance[__FRONTR] == 0))||((__GucDistance[__RIGHT] == 1) && (__GucDistance[__LEFT] == 0)))
           GsTpusle_T = -3;   
        else if((__GucDistance[__FRONTR] &&(__GucDistance[__FRONTL]==0))||((__GucDistance[__LEFT] == 1) && (__GucDistance[__RIGHT] == 0)))
           GsTpusle_T = 3;//3
        __PIDContr();
					
        break;

    case __MOTORRUN:                                                    /*  �������                    */
// 		i++;
// 		if(i == 1000){LED0 = !LED0;i = 0;}
      if (__GucMouseState == __GOAHEAD)                                 /*  ���ݴ�����״̬΢�����λ��  */
      {                             
            if ((__GucDistance[__FRONTL] && (__GucDistance[__FRONTR] == 0))||(__GucDistance[__FRONTR] &&(__GucDistance[__FRONTL]==0)))     /* ƫ��,ƫ�� */
            {
              if (n == 3)
              {
                    __GmRight.cState = __WAITONESTEP;
                    //__GmRight.uiPulseCtr += 2;
                 /*if (__GucDistance[__FRONTL] && (__GucDistance[__FRONTR] == 0))
                    GsTpusle_T = -1;
                 else
                    GsTpusle_T = 1;*/
              }               
              n++;
              n %= 6;
            }            
            else if (((__GucDistance[__RIGHT] == 1) && (__GucDistance[__LEFT] == 0))||((__GucDistance[__LEFT] == 1) && (__GucDistance[__RIGHT] == 0)))   /* Զƫ��Զƫ�� */
            {
                if(m ==3) 
                {
                    __GmRight.cState = __WAITONESTEP;
                }
                m++;
                m %=6;
            } 
            else 
            {
                m  = 0;
                n = 0;
                GsTpusle_T = 0;
            }
        
            if(GuiSpeedCtr==__SPEEDUP)
            { 
              k=(k+1)%20;
              if(k==19)
              __SpeedUp();              
            }
            else if(GuiSpeedCtr==__SPEEDDOWN)
            {
              k=(k+1)%10;
              if(k==9)
              __SpeedDown(); 
            }
            else;
        }
      else{
        GsTpusle_T = 0;
        GYRO_Z_Angle();
      }     
        __PIDContr();
        break;
    case 4:
      GsTpusle_T = 0;
      __PIDContr();
      break;
      
    case 5:
      __GmRight.sSpeed = 5;
      __rightMotorContr();
      __GmRight.cState = __MOTORSTOP;
      __GmLeft.sSpeed = 5;
      __leftMotorContr();
      __GmLeft.cState = __MOTORSTOP;
      break;
      
		
		
    default:
        break;
    }
	}
	if(SystemVolt < 2300)
	LEDShowMouseState(2);
	else 
		if(LedMouseTast  == 0)	//�ȴ�״̬����
		{
			RLED0 = 1;
		}
		else
			LEDShowMouseState(1);
}

/*********************************************************************************************************
** Function name:       GPIO_Port_A_ISR
** Descriptions:        ZLG7289�����жϷ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void EXTI2_IRQHandler(void)                                       /*  �������ں���Ƶ��             */
{
		uint16    DIS[10]     = {0};
    uint8 key;
    uint8 B,S,G;
    //status = GPIOPinIntStatus(GPIO_PORTA_BASE, true);              /*  ��PA���ж�״̬              */
    if((EXTI->PR & (1<<2)) == (1<<2))		//line 13                                      /*  �ж��Ƿ�Ϊ�����ж�          */
    {
       // GPIOPinIntClear(GPIO_PORTA_BASE, ZLG7289_KEY);                  /*  ���ж�                      */
        EXTI->PR = 1<<2;  																							// ���ж�  
        key = zlg7289Key();                                           /*  ������ֵ                    */
        /*��������Ч������8�������һ����ʾ*/
        if (key != 0xff) 
        {
          if(key>=4)
          {
              switch(key)
              {
              case 4:
                Tab=0;				//ǰ��Զ��
                B=GusWidthFrontLeftFar/100;
                S=(GusWidthFrontLeftFar/10)%10;
                G=(GusWidthFrontLeftFar/1)%10;
                zlg7289Download(1, 5, 0, B);
                zlg7289Download(1, 6, 1, S);
                zlg7289Download(1, 7, 0, G); 
                break;
                
              case 5:
                Tab=1;			//б45��
                B=GusWidthXieLeft/100;
                S=(GusWidthXieLeft/10)%10;
                G=(GusWidthXieLeft/1)%10;
                zlg7289Download(1, 5, 0, B);
                zlg7289Download(1, 6, 1, S);
                zlg7289Download(1, 7, 0, G);
                break;
                
              case 6:
                Tab=2;		//����Զ��
                 B=GusWidthLeftFar/100;
                 S=(GusWidthLeftFar/10)%10;
                 G=(GusWidthLeftFar/1)%10;
                 zlg7289Download(1, 5, 0, B);
                 zlg7289Download(1, 6, 1, S);
                 zlg7289Download(1, 7, 0, G);
                break;
              case 7:
                Tab=3;		//���ҽ���
                 B=GusWidthLeftNear/100;
                 S=(GusWidthLeftNear/10)%10;
                 G=(GusWidthLeftNear/1)%10;
                 zlg7289Download(1, 5, 0, B);
                 zlg7289Download(1, 6, 1, S);
                 zlg7289Download(1, 7, 0, G);
                break;
              case 8:
                Tab=4;		//ǰ������
               B=GusWidthFrontLeftNear/100;
               S=(GusWidthFrontLeftNear/10)%10;
               G=(GusWidthFrontLeftNear/1)%10;
               zlg7289Download(1, 5, 0, B);
               zlg7289Download(1, 6, 1, S);
               zlg7289Download(1, 7, 0, G);
               break;
               
               case 10:  
									  DIS[0] = GusWidthFrontLeftNear;						//ǰ�������
									  DIS[1] = GusWidthFrontLeftFar;						//ǰ����Զ��
									  DIS[2] = GusWidthFrontRightNear;					//ǰ���ҽ���
									  DIS[3] = GusWidthFrontRightFar;						//ǰ����Զ��
									  DIS[4] = GusWidthRightNear;								//�ҽ���
									  DIS[5] = GusWidthRightFar;								//��Զ��
									  DIS[6] = GusWidthLeftNear;								//�����
									  DIS[7] = GusWidthLeftFar;									//��Զ��
									  DIS[8] = GusWidthXieLeft;									//б45����
									  DIS[9] = GusWidthXieRight;								//б45����
//                 DIS[0]=GusFreq_F/1000;
//                 DIS[1]=GusFreq_F%1000/100;
//                 DIS[2]=(GusFreq_X/1000);
//                 DIS[3]=(GusFreq_X%1000/100);
//                 DIS[4]=(GusFreq_LF/1000);
//                 DIS[5]=(GusFreq_LF%1000/100);
//                 DIS[6]=(GusFreq_L/1000);
//                 DIS[7]=(GusFreq_L%1000/100);
//                 DIS[8]=(GusFreq_FJ/1000);
//                 DIS[9]=(GusFreq_FJ%1000/100);
                //write_fm24LC16(DIS,0x00,0xa0,10);
							 STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)DIS,20);
                __delay(1000000);
                break;
                
              default:
                break;
            }
          }
          else
          {
              if(key==1)			//������+
              {
                switch(Tab)
                {
                case 0:			//ǰ����Զ��	
										 GusWidthFrontLeftFar += 1;
										if(GusWidthFrontLeftFar > 800)GusWidthFrontLeftFar = 800;
                     //GusFreq_F +=100;
                     B=GusWidthFrontLeftFar/100;
										 S=(GusWidthFrontLeftFar/10)%10;
										 G=(GusWidthFrontLeftFar/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G); 
                     break;
                     
                   case 1:	//��б45��
                     //GusFreq_X +=100;
									   GusWidthXieLeft += 1;
									 if(GusWidthXieLeft > 800)GusWidthXieLeft = 800;
                     B=GusWidthXieLeft/100;
                     S=(GusWidthXieLeft/10)%10;
                     G=(GusWidthXieLeft/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 2:	//��Զ��
                     //GusFreq_LF +=100;
										GusWidthLeftFar += 1;
									 if(GusWidthLeftFar > 800)GusWidthLeftFar = 800;
                     B=GusWidthLeftFar/100;
                     S=(GusWidthLeftFar/10)%10;
                     G=(GusWidthLeftFar/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 3:	//�����
                     //GusFreq_L +=100;
										GusWidthLeftNear += 1;
									 if(GusWidthLeftNear > 800)GusWidthLeftNear = 800;
                     B=GusWidthLeftNear/100;
                     S=(GusWidthLeftNear/10)%10;
                     G=(GusWidthLeftNear/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 4:		//��ǰ����
                     //GusFreq_FJ +=100;
									   GusWidthFrontLeftNear += 1;
									 if(GusWidthFrontLeftNear > 800)GusWidthFrontLeftNear = 800;
                     B=GusWidthFrontLeftNear/100;
                     S=(GusWidthFrontLeftNear/10)%10;
                     G=(GusWidthFrontLeftNear/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   default:
                     break;
                           
               }
            }
            
            if(key==3)	//������-
            {
               switch(Tab)
                {
                case 0:			//ǰ����Զ��	
										 GusWidthFrontLeftFar -= 1;
										if(GusWidthFrontLeftFar <1)GusWidthFrontLeftFar = 1;
                     //GusFreq_F +=100;
                     B=GusWidthFrontLeftFar/100;
										 S=(GusWidthFrontLeftFar/10)%10;
										 G=(GusWidthFrontLeftFar/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G); 
                     break;
                     
                   case 1:	//��б45��
                     //GusFreq_X +=100;
									   GusWidthXieLeft -= 1;
									 if(GusWidthXieLeft <1)GusWidthXieLeft = 1;
                     B=GusWidthXieLeft/100;
                     S=(GusWidthXieLeft/10)%10;
                     G=(GusWidthXieLeft/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 2:	//��Զ��
                     //GusFreq_LF +=100;
										GusWidthLeftFar -= 1;
									 if(GusWidthLeftFar <1)GusWidthLeftFar = 1;
                     B=GusWidthLeftFar/100;
                     S=(GusWidthLeftFar/10)%10;
                     G=(GusWidthLeftFar/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 3:	//�����
                     //GusFreq_L +=100;
										GusWidthLeftNear -= 1;
									  if(GusWidthLeftNear <1)GusWidthLeftNear = 1;
                     B=GusWidthLeftNear/100;
                     S=(GusWidthLeftNear/10)%10;
                     G=(GusWidthLeftNear/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 4:		//��ǰ����
                     //GusFreq_FJ +=100;
									   GusWidthFrontLeftNear -= 1;
									 if(GusWidthFrontLeftNear <1)GusWidthFrontLeftNear = 1;
                     B=GusWidthFrontLeftNear/100;
                     S=(GusWidthFrontLeftNear/10)%10;
                     G=(GusWidthFrontLeftNear/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   default:
                     break;          
               }
            }  
						if(key==0)	//�����ҷ�-
						{
							switch(Tab)
							{
							case 0:			//ǰ����Զ��	
										 GusWidthFrontRightFar -= 1;
										if(GusWidthFrontRightFar <1)GusWidthFrontRightFar = 1;
                     //GusFreq_F +=100;
                     B=GusWidthFrontRightFar/100;
										 S=(GusWidthFrontRightFar/10)%10;
										 G=(GusWidthFrontRightFar/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G); 
                     break;
                     
                   case 1:	//��б45��
                     //GusFreq_X +=100;
									   GusWidthXieRight -= 1;
										if(GusWidthXieRight <1)GusWidthXieRight = 1;
                     B=GusWidthXieRight/100;
                     S=(GusWidthXieRight/10)%10;
                     G=(GusWidthXieRight/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 2:	//��Զ��
                     //GusFreq_LF +=100;
										GusWidthRightFar -= 1;
									 if(GusWidthRightFar <1)GusWidthRightFar = 1;
                     B=GusWidthRightFar/100;
                     S=(GusWidthRightFar/10)%10;
                     G=(GusWidthRightFar/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 3:	//�ҽ���
                     //GusFreq_L +=100;
										GusWidthRightNear -= 1;
									 if(GusWidthRightNear <1)GusWidthRightNear = 1;
                     B=GusWidthRightNear/100;
                     S=(GusWidthRightNear/10)%10;
                     G=(GusWidthRightNear/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 4:		//��ǰ����
                     //GusFreq_FJ +=100;
									   GusWidthFrontRightNear -= 1;
									 if(GusWidthFrontRightNear <1)GusWidthFrontRightNear = 1;
                     B=GusWidthFrontRightNear/100;
                     S=(GusWidthFrontRightNear/10)%10;
                     G=(GusWidthFrontRightNear/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
									 default :
										 break;
								 }
						}
						if(key==2)	//�����ҷ�+
						{
							switch(Tab)
							{
							case 0:			//ǰ����Զ��	
										 GusWidthFrontRightFar += 1;
											if(GusWidthFrontRightFar > 999)GusWidthFrontRightFar = 999;
                     //GusFreq_F +=100;
                     B=GusWidthFrontRightFar/100;
										 S=(GusWidthFrontRightFar/10)%10;
										 G=(GusWidthFrontRightFar/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G); 
                     break;
                     
                   case 1:	//��б45��
                     //GusFreq_X +=100;
									   GusWidthXieRight += 1;
									 if(GusWidthXieRight > 999)GusWidthXieRight = 999;
                     B=GusWidthXieRight/100;
                     S=(GusWidthXieRight/10)%10;
                     G=(GusWidthXieRight/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 2:	//��Զ��
                     //GusFreq_LF +=100;
										GusWidthRightFar += 1;
									 if(GusWidthRightFar > 999)GusWidthRightFar = 999;
                     B=GusWidthRightFar/100;
                     S=(GusWidthRightFar/10)%10;
                     G=(GusWidthRightFar/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 3:	//�ҽ���
                     //GusFreq_L +=100;
										GusWidthRightNear += 1;
									 if(GusWidthRightNear > 999)GusWidthRightNear = 999;
                     B=GusWidthRightNear/100;
                     S=(GusWidthRightNear/10)%10;
                     G=(GusWidthRightNear/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
                     
                   case 4:		//��ǰ����
                     //GusFreq_FJ +=100;
									   GusWidthFrontRightNear += 1;
									   if(GusWidthFrontRightNear > 999)GusWidthFrontRightNear = 999;
									 
                     B=GusWidthFrontRightNear/100;
                     S=(GusWidthFrontRightNear/10)%10;
                     G=(GusWidthFrontRightNear/1)%10;
                     zlg7289Download(1, 5, 0, B);
                     zlg7289Download(1, 6, 1, S);
                     zlg7289Download(1, 7, 0, G);
                     break;
									 default :
										 break;
								 }
						}
          }
                    
        }
    }
}

/*********************************************************************************************************
** Function name:       TIM6_IRQHandler
** Descriptions:        ��ʱ�ж�ɨ�衣
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
// void SysTick_ISR(void)
// {
//   __irCheck();
// }
void TIM6_IRQHandler(void)
{ 
	//static unsigned int i = 0;
	
	if(TIM6->SR&0X0001)//����ж�
	{
		__irCheck();
		//i++;
		//if(i == 1000)
		//{
		//	i = 0;
		//	LED0 = !LED0;
			//LED_OutPutFlip();
		//}			
	}				   
	TIM6->SR&=~(1<<0);//����жϱ�־λ
}
/*********************************************************************************************************
** Function name:       mouseInit
** Descriptions:        ��LM3S615���������г�ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int ww = 0;
void mouseInit (void)
{
  Stm32_Clock_Init(9); //72M
	
	delay_init(72);//��ʱ��ʼ��
	//uart_init(72,9600);//���ô���1������
	zp_UART4_Init(36,9600);	//���ô���5������  ��APB1������
	LED_Init();
	
	
	IrRecieveGpioInilization();				//�����ʼ��
	delay_ms(20);
	
	Timer6_Init(72-1,625-1);		//625us ��ʱ
	Timer7_Init(72-1,1000-1);			//	1ms
	IR_SendInitialization();		//���ⷢ���ʼ��
	
	TIM8_PWM_Init(3600,0);		//40K  Motor
	
	fan_init(3600,0);
	
	IIC_Init();
	MPU6050_Init();
	//
	
	
	ZLG7289Init();
	KEY_Init();
	MotorRightEncoderInit();		//�ҵ����������ʼ��Time4
	MotorLeftEncoderInit();			//������������ʼ��Time1
	Adc_Init();
	
	PIDInit();
	
    GucMapBlock[0][0] = 0x01;
    GucMapBlock0[0][0] = 0x01;
		
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/




