/****************************************Copyright (c)****************************************************
**?????????????????????
**https://www.qcmcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           Mouse.Drive.h
** Last modified Date:  
** Last Version: 
** Description:         ???????????????
** 
**--------------------------------------------------------------------------------------------------------
** Created By: 
** Created date: 
** Version: 
** Descriptions: 
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/


#ifndef __Mouse_Drive_h
#define __Mouse_Drive_h


/*********************************************************************************************************
  ????????
*********************************************************************************************************/
#include "test.h"
#include "Micromouse.h"
#include "Mouse_Config.h"

#define FLASH_SAVE_ADDR  0X08070000		//FLASH��????    10
#define FLASH_SAVE_ADDR_START  FLASH_SAVE_ADDR + 40				//???��????			1
//#define FLASH_SAVE_ADDR_WALL   FLASH_SAVE_ADDR_START + 2		//???��???
#define FLASH_SAVE_ADDR_WALL   0X08071000
/*********************************************************************************************************
  ????????--??????
*********************************************************************************************************/
#define __LEFT              0                                           /*  ???????                  */
#define __FRONTL            1                                           /*  ???????????                */
#define __FRONT             2                                           /*  ?????????                  */
#define __FRONTR            3                                           /*  ???????????                */
#define __RIGHT             4                                           /*  ?????????                  */


/*********************************************************************************************************
  ????????--????????
*********************************************************************************************************/
#define __STOP              0                                           /*  ????????                  */
#define __GOAHEAD           1                                           /*  ?????????                  */
#define __TURNLEFT          3                                           /*  ???????????                */
#define __TURNRIGHT         4                                           /*  ???????????                */
#define __TURNBACK          5                                           /*  ??????????                */
#define __TURNLEFTY         7                                           /*  ???????????                */
#define __TURNRIGHTY        8                                           /*  ???????????                */
#define __GOBACK            9
/*********************************************************************************************************
  ????????--?????????
*********************************************************************************************************/
#define __SPEEDUP         0                                           /*  ???????                    */
#define __SPEEDDOWN       1                                           /*  ???????                */

/*********************************************************************************************************
  ????????--?????
*********************************************************************************************************/
#define __MOTORSTOP         0                                           /*  ?????                    */
#define __WAITONESTEP       1                                           /*  ?????????                */
#define __MOTORRUN          2                                           /*  ???????                    */


/*********************************************************************************************************
  ????????--??????��???
*********************************************************************************************************/
#define __MOTORGOAHEAD      0                                           /*  ??????                    */
#define __MOTORGOBACK       1                                           /*  ???????                    */
#define __MOTORGOSTOP       2                                           /*  ??????????                */

/*********************************************************************************************************
  ????????--PID
*********************************************************************************************************/
#define __KP 50     //???? 30
#define __KI 0.15   //???? 0.01
#define __KD 0            //???

#define U_MAX 3400       //?????????,??pwm??????? 
#define U_MIN 5 
#define error_IMAX 10     //??????? 
#define Deadband 1   //???PID????????????��

/*********************************************************************************************************
 调试用于一个格子左转和右转
*********************************************************************************************************/
//#define DEBUG_TURN_STOP_L 1 
//#define DEBUG_TURN_STOP_R 1

/*********************************************************************************************************
  ????????--????
*********************************************************************************************************/
#define FrontLeftPosition  0
#define LeftPosition       1
#define XieLeftPosition    2
#define XieRightPosition   3
#define RightPosition      4
#define FrontRightPosition 5

/*********************************************************************************************************
  ???�G??
*********************************************************************************************************/
struct __motor {
    int8    cState;                                                     /*  ?????????                */
    int8    cDir;                                                       /*  ??????��???                */
    int8    cRealDir;                                                   /*  ??????��???                */
    int32  uiPulse;                                                    /*  ?????????��?????          */
    int32  uiPulseCtr;                                                 /*  ????????��?????            */
    int16   sSpeed;                                                    /*  ???????                    */
};
typedef struct __motor __MOTOR;

struct __pid       //???????????????? 
{ 
    //uint16 usRef;      //???PID??????څ? 
    int32 usFeedBack;  //???PID?????????
    int32 usEncoder_new; //??????
    int32 usEncoder_last; //??????
    
    int16 sRef;
    int16 sFeedBack;
    int16 sPreError;  //???PID?????��???????,,vi_Ref - vi_FeedBack 
    int16 sPreDerror; //???PID?????��??????????d_error-PreDerror; 
  
    fp32 fKp;      //???PID??Ka = Kp 
    fp32 fKi;      //???PID??Kb = Kp * ( T / Ti ) 
    fp32 fKd;      //???PID?? 
       
    int16 iPreU;    //???????????      
};
typedef struct __pid __PID;
/*********************************************************************************************************
  ????????
*********************************************************************************************************/

uint8 keyCheck(void);
void mouseInit(void);
void mouseGoahead(int8  cNBlock);                                       /*  ???N??                     */
void mouseGoahead_C1(int8  cNBlock);
void mouseGoahead_h(int8  cNBlock);
void mouseGoahead_C(int8  cNBlock);                                       /*  ???N??                     */
void mouseGoahead_C1(int8  cNBlock);                                       /*  ???N??                     */
void mouseGoahead_C2(int8  cNBlock);                                       /*  ???N??                     */
void mouseGoahead_C3(int8  cNBlock);                                       /*  ???N??                     */
void mazeSearch(void);                                                  /*  ???N??                     */
void mouseTurnleft(void);                                               /*  ?????90??                  */
void mouseTurnright(void);                                              /*  ?????90??                  */
void mouseTurnback(void);                                               /*  ????                      */
void mouseTurnback_H(void);                                               /*  ????                      */
void sensorDebug(void);                                                 /*  ??????????                  */
void voltageDetect(void);
void voltageDetectRef(void);
void mouseGoaheadhui(int8  cNBlock);                                       /*  ???N??                     */
void mouseGoahead_Llow (int8  cNBlock);
void mouseGoahead_C1_Llow (int8  cNBlock);
void mouseGoahead_hLlow (int8  cNBlock);
void mouseGoahead_C_Llow (int8  cNBlock);
void mouseGoahead_C2_Llow (int8  cNBlock);
void mouseGoahead_C3_Llow (int8  cNBlock);
void mouseGoahead_L_Llow (int8  cNBlock);

void __UART0Init (void);
void SendChar(uint8 dat);
void SendStr(uint8 *s);
void mouseTurnback_Y(void);
void mouseSpurt_C(void);
void mouseSpurt_LL(void);
void mouseTurnback_YY(void);
void onestep(void);
static void __delay(uint32  uiD);
void testEncoder(void);
static void __rightMotorContr(void);
static void __leftMotorContr(void);
static void __mouseCoorUpdate(void);
 void __mazeInfDebug(void);
static void __irSendFreq(uint32  __uiFreq, int8  __cNumber);
static void __irSendWidth (unsigned short int  Width, int8  __cNumber);
static void __irCheck(void);
static void __wallCheck(void);                                          /*  ?????                    */
static void __sensorInit(void);
static void __MotorIint(void);
static void __keyInit(void);
static void __sysTickInit(void);
void mouseGoahead_SC(int8  cNBlock);                                       /*  ???N??                     */

void mouseGoahead_SC_Llow (int8  cNBlock);
void mouseTurnleft_SC(void);                                               /*  ?????90??                  */
void mouseTurnright_SC(void);                                              /*  ?????90??                  */

void mouseTurnleft_L(void);                                               /*  ?????90??                  */
void mouseTurnright_L(void);                                              /*  ?????90??                  */
void mouseGoahead_L(int8  cNBlock);                                       /*  ???N??                     */
void mouseStop(void); 
void mouseGo(void);
void __keyIntDisable (void);
void mouseTurnback_C(void);                                               /*  ????                      */
void mouseTurnback_C1(void);                                               /*  ????                      */


void mouseTurnleft_H(void);                                               /*  ?????90??                  */
void mouseTurnright_H(void);                                              /*  ?????90??                  */
void mouseTurnleft_C(void);                                               /*  ?????90??                  */
void mouseTurnright_C(void);                                              /*  ?????90??                  */
void mouseTurnleft_KC(void);                                               /*  ?????90??                  */
void mouseTurnright_KC(void);                                              /*  ?????90??                  */
uint8 DenggaoCheck (void);
uint8 PulseCtrCheck (void);
void mouseTurnleft_Y(void);                                               /*  ?????90??                  */
void mouseTurnright_Y(void);                                              /*  ?????90??                  */
uint8 startCheck (void);
void GYRO_Z_Angle(void);
void GetStaticAngle(void);
void TestEncoder(void);


#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
