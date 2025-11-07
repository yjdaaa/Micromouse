/****************************************Copyright (c)****************************************************
天津启诚伟业科技有限公司出品
https://www.qcmcu.com
*********************************************************************************************************/


/*********************************************************************************************************
  策略
	
*********************************************************************************************************/
#include "Maze.h"

/*********************************************************************************************************
  全局变量定义
*********************************************************************************************************/
static uint8    GucXStart                           = 0;                /*  起点横坐标                  */
static uint8    GucYStart                           = 0;                /*  起点纵坐标                  */
static uint8    GucXGoal0                           = XDST0;            /*  终点X坐标，有两个值         */
static uint8    GucXGoal1                           = XDST1;
static uint8    GucYGoal0                           = YDST0;            /*  终点Y坐标，有两个值         */
static uint8    GucYGoal1                           = YDST1;
static uint8    GucMouseTask                        = WAIT;             /*  状态机，初始状态为等待      */
static uint8    GucMapStep[MAZETYPE][MAZETYPE]      = {0xff};           /*  保存各坐标的等高值          */
static uint8    GucMapStep1[MAZETYPE][MAZETYPE]     = {0xff};            /*  保存各坐标的等高值          */

static MAZECOOR GmcStack[MAZETYPE * MAZETYPE]       = {0};              /*  在mapStepEdit()中作堆栈使用 */
static MAZECOOR GmcStack1[MAZETYPE * MAZETYPE]      = {0};
static MAZECOOR GmcCrossway[MAZETYPE * MAZETYPE]    = {0};              /*  Main()中暂存未走过支路坐标  */

static uint8    GucMouseTurn                        =0;
static uint8    kang                        =0;
static uint8    hang                        =0;
static uint8    SEARCHMETHOD=0;                   //法则选择
/*********************************************************************************************************
** Function name:       Delay
** Descriptions:        延时函数
** input parameters:    uiD :延时参数，值越大，延时越久
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void delay (uint32 uiD)
{
    for (; uiD; uiD--);
}

/*********************************************************************************************************
** Function name:       goalwFall
** Descriptions:       
** input parameters:    无
** output parameters:   无
** Returned value:     
*********************************************************************************************************/
void goalwall(void)
{
   if ((GmcMouse.cX==7)&&(GmcMouse.cY==7)) {                                /* 终点墙壁资料赋值*/
     if(GucMouseDir==UP) {
       
        GucMapBlock1[7][8]=0x16;
        GucMapBlock1[8][7]=0x19;
        GucMapBlock1[8][8]=0x1c;
        GucMapBlock1[6][7]=(GucMapBlock1[6][7]&0xfd);
        GucMapBlock1[6][8]=(GucMapBlock1[6][8]&0xfd);
        GucMapBlock1[7][9]=(GucMapBlock1[7][9]&0xfb);
        GucMapBlock1[8][9]=(GucMapBlock1[8][9]&0xfb);
        GucMapBlock1[9][8]=(GucMapBlock1[9][8]&0xf7);
        GucMapBlock1[9][7]=(GucMapBlock1[9][7]&0xf7);
        GucMapBlock1[8][6]=(GucMapBlock1[8][6]&0xfe);
        GucMapBlock0[7][8]=0x16;
        GucMapBlock0[8][7]=0x19;
        GucMapBlock0[8][8]=0x1c;
       
        }
     if(GucMouseDir==RIGHT) {
        GucMapBlock1[7][8]=0x16;
        GucMapBlock1[8][7]=0x19;
        GucMapBlock1[8][8]=0x1c;
        GucMapBlock1[6][8]=(GucMapBlock1[6][8]&0xfd);
        GucMapBlock1[7][9]=(GucMapBlock1[7][9]&0xfb);
        GucMapBlock1[8][9]=(GucMapBlock1[8][9]&0xfb);
        GucMapBlock1[9][8]=(GucMapBlock1[9][8]&0xf7);
        GucMapBlock1[9][7]=(GucMapBlock1[9][7]&0xf7);
        GucMapBlock1[8][6]=(GucMapBlock1[8][6]&0xfe);
        GucMapBlock1[7][6]=(GucMapBlock1[7][6]&0xfe);
         GucMapBlock0[7][8]=0x16;
        GucMapBlock0[8][7]=0x19;
        GucMapBlock0[8][8]=0x1c;
     }
   }
   if ((GmcMouse.cX==7)&&(GmcMouse.cY==8)) {
     if(GucMouseDir==DOWN) {
        GucMapBlock1[7][7]=0x13;
        GucMapBlock1[8][7]=0x19;
        GucMapBlock1[8][8]=0x1c;
        GucMapBlock1[6][7]=(GucMapBlock1[6][7]&0xfd);
        GucMapBlock1[6][8]=(GucMapBlock1[6][8]&0xfd);
        GucMapBlock1[8][9]=(GucMapBlock1[8][9]&0xfb);
        GucMapBlock1[9][8]=(GucMapBlock1[9][8]&0xf7);
        GucMapBlock1[9][7]=(GucMapBlock1[9][7]&0xf7);
        GucMapBlock1[8][6]=(GucMapBlock1[8][6]&0xfe);
        GucMapBlock1[7][6]=(GucMapBlock1[7][6]&0xfe);
        GucMapBlock0[7][7]=0x13;
        GucMapBlock0[8][7]=0x19;
        GucMapBlock0[8][8]=0x1c;
     }
     if(GucMouseDir==RIGHT) {
        GucMapBlock1[7][7]=0x13;
        GucMapBlock1[8][7]=0x19;
        GucMapBlock1[8][8]=0x1c;
        GucMapBlock1[6][7]=(GucMapBlock1[6][7]&0xfd);
        GucMapBlock1[7][9]=(GucMapBlock1[7][9]&0xfb);
        GucMapBlock1[8][9]=(GucMapBlock1[8][9]&0xfb);
        GucMapBlock1[9][8]=(GucMapBlock1[9][8]&0xf7);
        GucMapBlock1[9][7]=(GucMapBlock1[9][7]&0xf7);
        GucMapBlock1[8][6]=(GucMapBlock1[8][6]&0xfe);
        GucMapBlock1[7][6]=(GucMapBlock1[7][6]&0xfe);
         GucMapBlock0[7][7]=0x13;
        GucMapBlock0[8][7]=0x19;
        GucMapBlock0[8][8]=0x1c;
     }
   }
   if ((GmcMouse.cX==8)&&(GmcMouse.cY==7)) {
     if(GucMouseDir==UP) {
        GucMapBlock1[7][7]=0x13;
        GucMapBlock1[7][8]=0x16;
        GucMapBlock1[8][8]=0x1c;
        GucMapBlock1[6][7]=(GucMapBlock1[6][7]&0xfd);
        GucMapBlock1[6][8]=(GucMapBlock1[6][8]&0xfd);
        GucMapBlock1[7][9]=(GucMapBlock1[7][9]&0xfb);
        GucMapBlock1[8][9]=(GucMapBlock1[8][9]&0xfb);
        GucMapBlock1[9][8]=(GucMapBlock1[9][8]&0xf7);
        GucMapBlock1[9][7]=(GucMapBlock1[9][7]&0xf7);
        GucMapBlock1[7][6]=(GucMapBlock1[7][6]&0xfe);
         GucMapBlock0[7][7]=0x13;
        GucMapBlock0[7][8]=0x16;
        GucMapBlock0[8][8]=0x1c;
     }
     if(GucMouseDir==LEFT) {
        GucMapBlock1[7][7]=0x13;
        GucMapBlock1[7][8]=0x16;
        GucMapBlock1[8][8]=0x1c;
        GucMapBlock1[6][7]=(GucMapBlock1[6][7]&0xfd);
        GucMapBlock1[6][8]=(GucMapBlock1[6][8]&0xfd);
        GucMapBlock1[7][9]=(GucMapBlock1[7][9]&0xfb);
        GucMapBlock1[8][9]=(GucMapBlock1[8][9]&0xfb);
        GucMapBlock1[9][8]=(GucMapBlock1[9][8]&0xf7);
        GucMapBlock1[8][6]=(GucMapBlock1[8][6]&0xfe);
        GucMapBlock1[7][6]=(GucMapBlock1[7][6]&0xfe);
        GucMapBlock0[7][7]=0x13;
        GucMapBlock0[7][8]=0x16;
        GucMapBlock0[8][8]=0x1c;
     }
   }
   if ((GmcMouse.cX==8)&&(GmcMouse.cY==8)) {
     if(GucMouseDir==DOWN) {
        GucMapBlock1[7][7]=0x13;
        GucMapBlock1[7][8]=0x16;
        GucMapBlock1[8][7]=0x19;
        GucMapBlock1[6][7]=(GucMapBlock1[6][7]&0xfd);
        GucMapBlock1[6][8]=(GucMapBlock1[6][8]&0xfd);
        GucMapBlock1[7][9]=(GucMapBlock1[7][9]&0xfb);
        GucMapBlock1[9][8]=(GucMapBlock1[9][8]&0xf7);
        GucMapBlock1[9][7]=(GucMapBlock1[9][7]&0xf7);
        GucMapBlock1[8][6]=(GucMapBlock1[8][6]&0xfe);
        GucMapBlock1[7][6]=(GucMapBlock1[7][6]&0xfe);
        GucMapBlock0[7][7]=0x13;
        GucMapBlock0[7][8]=0x16;
        GucMapBlock0[8][7]=0x19;
     }
     if(GucMouseDir==LEFT) {
        GucMapBlock1[7][7]=0x13;
        GucMapBlock1[7][8]=0x16;
        GucMapBlock1[8][7]=0x19;
        GucMapBlock1[6][7]=(GucMapBlock1[6][7]&0xfd);
        GucMapBlock1[6][8]=(GucMapBlock1[6][8]&0xfd);
        GucMapBlock1[7][9]=(GucMapBlock1[7][9]&0xfb);
        GucMapBlock1[8][9]=(GucMapBlock1[8][9]&0xfb);
        GucMapBlock1[9][7]=(GucMapBlock1[9][7]&0xf7);
        GucMapBlock1[8][6]=(GucMapBlock1[8][6]&0xfe);
        GucMapBlock1[7][6]=(GucMapBlock1[7][6]&0xfe);
        GucMapBlock0[7][7]=0x13;
        GucMapBlock0[7][8]=0x16;
        GucMapBlock0[8][7]=0x19;
      }
    }
}
/*********************************************************************************************************
** Function name:       mapStepEdit
** Descriptions:        制作以目标点为起点的等高图
** input parameters:    uiX:    目的地横坐标
**                      uiY:    目的地纵坐标
** output parameters:   GucMapStep[][]:  各坐标上的等高值
** Returned value:      无
*********************************************************************************************************/
void mapStepEdit (int8  cX, int8  cY)
{
    uint8 n         = 0;                                                /*  GmcStack[]下标              */
    uint8 ucStep    = 1;                                                /*  等高值                      */
    uint8 ucStat    = 0;                                                /*  统计可前进的方向数          */
    uint8 i,j;
    
    GmcStack[n].cX  = cX;                                               /*  起点X值入栈                 */
    GmcStack[n].cY  = cY;                                               /*  起点Y值入栈                 */
    n++;
    /*
     *  初始化各坐标等高值
     */
    for (i = 0; i < MAZETYPE; i++) {
        for (j = 0; j < MAZETYPE; j++) {
            GucMapStep[i][j] = 0xff;
        }
    }
    /*
     *  制作等高图，直到堆栈中所有数据处理完毕
     */
    while (n) {
        GucMapStep[cX][cY] = ucStep++;                                  /*  填入等高值                  */

        /*
         *  对当前坐标格里可前进的方向统计
         */
        ucStat = 0;
        if ((GucMapBlock[cX][cY] & 0x01) &&                             /*  前方有路                    */
            (GucMapStep[cX][cY + 1] > (ucStep))) {                      /*  前方等高值大于计划设定值    */
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  右方有路                    */
            (GucMapStep[cX + 1][cY] > (ucStep))) {                      /*  右方等高值大于计划设定值    */
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&
            (GucMapStep[cX][cY - 1] > (ucStep))) {
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&
            (GucMapStep[cX - 1][cY] > (ucStep))) {
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if (ucStat == 0) {
            n--;
            cX = GmcStack[n].cX;
            cY = GmcStack[n].cY;
            ucStep = GucMapStep[cX][cY];
        } else {
            if (ucStat > 1) {                                           /*  有多个可前进方向，保存坐标  */
                GmcStack[n].cX = cX;                                    /*  横坐标X值入栈               */
                GmcStack[n].cY = cY;                                    /*  纵坐标Y值入栈               */
                n++;
            }
            /*
             *  任意选择一条可前进的方向前进
             */
            if ((GucMapBlock[cX][cY] & 0x01) &&                         /*  上方有路                    */
                (GucMapStep[cX][cY + 1] > (ucStep))) {                  /*  上方等高值大于计划设定值    */
                cY++;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x02) &&                         /*  右方有路                    */
                (GucMapStep[cX + 1][cY] > (ucStep))) {                  /*  右方等高值大于计划设定值    */
                cX++;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x04) &&                         /*  下方有路                    */
                (GucMapStep[cX][cY - 1] > (ucStep))) {                  /*  下方等高值大于计划设定值    */
                cY--;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock[cX][cY] & 0x08) &&                         /*  左方有路                    */
                (GucMapStep[cX - 1][cY] > (ucStep))) {                  /*  左方等高值大于计划设定值    */
                cX--;                                                   /*  修改坐标                    */
                continue;
            }
        }
    }
}


/*********************************************************************************************************
** Function name:       mapStepEdit1
** Descriptions:        制作以目标点为起点的等高图
** input parameters:    uiX:    目的地横坐标
**                      uiY:    目的地纵坐标
** output parameters:   GucMapStep[][]:  各坐标上的等高值
** Returned value:      无
*********************************************************************************************************/
void mapStepEdithong (int8  cX, int8  cY)
{
    uint8 n         = 0;                                                /*  GmcStack[]下标              */
    uint8 ucStep    = 1;                                                /*  等高值                      */
    uint8 ucStat    = 0;                                                /*  统计可前进的方向数          */
    uint8 x = 1;
    uint8 i,j;
    
    GmcStack1[n].cX  = cX;                                               /*  起点X值入栈                 */
    GmcStack1[n].cY  = cY;                                               /*  起点Y值入栈                 */
    n++;
    /*
     *  初始化各坐标等高值
     */
    for (i = 0; i < MAZETYPE; i++) {
        for (j = 0; j < MAZETYPE; j++) {
            GucMapStep1[i][j] = 0xff;
        }
    }
    //mouseStop();
    /*
     *  制作等高图，直到堆栈中所有数据处理完毕
     */
    while (n) {
      if(x){
        if(DenggaoCheck ()== TE){
          if(GucFrontJinju){
              mouseStop();
              x=0;
          }
        }
      }
        GucMapStep1[cX][cY] = ucStep++;                                  /*  填入等高值                  */

        /*
         *  对当前坐标格里可前进的方向统计
         */
        ucStat = 0;
        if ((GucMapBlock1[cX][cY] & 0x01) &&                             /*  前方有路                    */
            (GucMapStep1[cX][cY + 1] > (ucStep))) {                      /*  前方等高值大于计划设定值    */
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock1[cX][cY] & 0x02) &&                             /*  右方有路                    */
            (GucMapStep1[cX + 1][cY] > (ucStep))) {                      /*  右方等高值大于计划设定值    */
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock1[cX][cY] & 0x04) &&
            (GucMapStep1[cX][cY - 1] > (ucStep))) {
            ucStat++;                                                   /*  可前进方向数加1             */
        }
        if ((GucMapBlock1[cX][cY] & 0x08) &&
            (GucMapStep1[cX - 1][cY] > (ucStep))) {
            ucStat++;                                                   /*  可前进方向数加1             */
        }
   
        if (ucStat == 0) {
            n--;
            cX = GmcStack1[n].cX;
            cY = GmcStack1[n].cY;
            ucStep = GucMapStep1[cX][cY];
        } else {
            if (ucStat > 1) {                                           /*  有多个可前进方向，保存坐标  */
                GmcStack1[n].cX = cX;                                    /*  横坐标X值入栈               */
                GmcStack1[n].cY = cY;                                    /*  纵坐标Y值入栈               */
                n++;
            }
            /*
             *  任意选择一条可前进的方向前进
             */
            if ((GucMapBlock1[cX][cY] & 0x01) &&                         /*  上方有路                    */
                (GucMapStep1[cX][cY + 1] > (ucStep))) {                  /*  上方等高值大于计划设定值    */
                cY++;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock1[cX][cY] & 0x02) &&                         /*  右方有路                    */
                (GucMapStep1[cX + 1][cY] > (ucStep))) {                  /*  右方等高值大于计划设定值    */
                cX++;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock1[cX][cY] & 0x04) &&                         /*  下方有路                    */
                (GucMapStep1[cX][cY - 1] > (ucStep))) {                  /*  下方等高值大于计划设定值    */
                cY--;                                                   /*  修改坐标                    */
                continue;
            }
            if ((GucMapBlock1[cX][cY] & 0x08) &&                         /*  左方有路                    */
                (GucMapStep1[cX - 1][cY] > (ucStep))) {                  /*  左方等高值大于计划设定值    */
                cX--;                                                   /*  修改坐标                    */
                continue;
            }
        }
    }
    GucMouseTurn =1;
}

/*********************************************************************************************************
** Function name:       mouseSpurt
** Descriptions:        电脑鼠从起点以最短路径跑向终点
** input parameters:    无
 ** output parameters:  无
** Returned value:      无
*********************************************************************************************************/
void mouseSpurt (void)
{
    uint8 ucTemp = 0xff;
    int8 cXdst = 0,cYdst = 0;
    /*
     *  对终点的四个坐标分别制作等高图
     *  取离起点最近的一个点作为目标点
     */
    if (GucMapBlock[GucXGoal0][GucYGoal0] & 0x0c) {                     /*  判断该终点坐标是否有出口    */
        mapStepEdit(GucXGoal0,GucYGoal0);                               /*  制作等高图                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  保存离起点最近的坐标        */
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal0][GucYGoal1] & 0x09) {                     /*  判断该终点坐标是否有出口    */
        mapStepEdit(GucXGoal0,GucYGoal1);                               /*  制作等高图                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  保存离起点最近的坐标        */
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal0] & 0x06) {                     /*  判断该终点坐标是否有出口    */
        mapStepEdit(GucXGoal1,GucYGoal0);                               /*  制作等高图                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  保存离起点最近的坐标        */
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal1] & 0x03) {                     /*  判断该终点坐标是否有出口    */
        mapStepEdit(GucXGoal1,GucYGoal1);                               /*  制作等高图                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  保存离起点最近的坐标        */
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    
    objectGoTo_C1(cXdst,cYdst);                                            /*  运行到指定目标点            */
    
}

void objectGoTo(int8  cXdst, int8  cYdst)
{
    uint8 ucStep = 1;
    int8  cNBlock = 0, cDirTemp;
    int8 cX,cY;
    GucCrossroad=0;
    cX = GmcMouse.cX;
    cY = GmcMouse.cY;
    mapStepEdit(cXdst,cYdst);                                           /*  制作等高图                  */
    
    /*
     *  根据等高值向目标点运动，直到达到目的地
     */
    while ((cX != cXdst) || (cY != cYdst)) {
        
        ucStep = GucMapStep[cX][cY];
        /*
         *  任选一个等高值比当前自身等高值小的方向前进
         */
        if ((GucMapBlock[cX][cY] & 0x01) &&                             /*  上方有路                    */
            (GucMapStep[cX][cY + 1] < ucStep)) {                        /*  上方等高值较小              */
            cDirTemp = UP;                                              /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cY++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  右方有路                    */
            (GucMapStep[cX + 1][cY] < ucStep)) {                        /*  右方等高值较小              */
            cDirTemp = RIGHT;                                           /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cX++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&                             /*  下方有路                    */
            (GucMapStep[cX][cY - 1] < ucStep)) {                        /*  下方等高值较小              */
            cDirTemp = DOWN;                                            /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cY--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&                             /*  左方有路                    */
            (GucMapStep[cX - 1][cY] < ucStep)) {                        /*  左方等高值较小              */
            cDirTemp = LEFT;                                            /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cX--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        cDirTemp = (cDirTemp + 4 - GucMouseDir)%4;                      /*  计算方向偏移量              */
        GucDirTemp = cDirTemp;
        if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
              mouseGoahead_C2(cNBlock);                                      /*  前进cNBlock步               */
          else{
            mouseGoahead_C2_Llow(cNBlock);
            GucCrossroad = 0;
          }
          
          
        }        
        cNBlock = 0;  
        /*  任务清零                    */
        
        /*
         *  控制电脑鼠转弯
         */
         
       switch (cDirTemp) {

        case 1:
            mouseTurnright_KC();
						//mouseTurnright();
            break;

        case 2:
            mouseTurnback();
						//mouseTurnback();
            break;

        case 3:
            mouseTurnleft_KC();
						//mouseTurnleft();
            break;

        default:
            break;
        }
      GmcMouse.cX=cX;
      GmcMouse.cY=cY;
    }
    /*
     *  判断任务是否完成，否则继续前进
     */
    
      if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
            mouseGoahead_C2(cNBlock);                                      /*  前进cNBlock步               */
          else{
            mouseGoahead_C2_Llow(cNBlock);
            GucCrossroad = 0;
          }
          GmcMouse.cX=cX;
          GmcMouse.cY=cY;
      }
}




void objectGoTo_C1(int8  cXdst, int8  cYdst)
{
    uint8 ucStep = 1;
    int8  cNBlock = 0, cDirTemp;
    int8 cX,cY;
    GucCrossroad=0;
    cX = GmcMouse.cX;
    cY = GmcMouse.cY;
    mapStepEdit(cXdst,cYdst);                                           /*  制作等高图                  */
    
    /*
     *  根据等高值向目标点运动，直到达到目的地
     */
    while ((cX != cXdst) || (cY != cYdst)) {
        
        ucStep = GucMapStep[cX][cY];
        /*
         *  任选一个等高值比当前自身等高值小的方向前进
         */
        if ((GucMapBlock[cX][cY] & 0x01) &&                             /*  上方有路                    */
            (GucMapStep[cX][cY + 1] < ucStep)) {                        /*  上方等高值较小              */
            cDirTemp = UP;                                              /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cY++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  右方有路                    */
            (GucMapStep[cX + 1][cY] < ucStep)) {                        /*  右方等高值较小              */
            cDirTemp = RIGHT;                                           /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cX++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&                             /*  下方有路                    */
            (GucMapStep[cX][cY - 1] < ucStep)) {                        /*  下方等高值较小              */
            cDirTemp = DOWN;                                            /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cY--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&                             /*  左方有路                    */
            (GucMapStep[cX - 1][cY] < ucStep)) {                        /*  左方等高值较小              */
            cDirTemp = LEFT;                                            /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cX--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        cDirTemp = (cDirTemp + 4 - GucMouseDir)%4;                      /*  计算方向偏移量              */
        GucDirTemp = cDirTemp;
        if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
              mouseGoahead_C1(cNBlock);                                      /*  前进cNBlock步               */
          else{
            mouseGoahead_C1_Llow(cNBlock);
            GucCrossroad = 0;
          }
          
          
        }        
        cNBlock = 0;  
        /*  任务清零                    */
        
        /*
         *  控制电脑鼠转弯
         */
         
       switch (cDirTemp) {

        case 1:
            //mouseTurnright_H();
						mouseTurnright_C();
            break;

        case 2:
            mouseTurnback();
						//mouseTurnback();
            break;

        case 3:
            //mouseTurnleft_H();
						mouseTurnleft_C();
            break;

        default:
            break;
        }
      GmcMouse.cX=cX;
      GmcMouse.cY=cY;
    }
    /*
     *  判断任务是否完成，否则继续前进
     */
    
      if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
            mouseGoahead_C1(cNBlock);                                      /*  前进cNBlock步               */
          else{
            mouseGoahead_C1_Llow(cNBlock);
            GucCrossroad = 0;
          }
          GmcMouse.cX=cX;
          GmcMouse.cY=cY;
      }
}
void objectGoTo_h(int8  cXdst, int8  cYdst)
{
    uint8 ucStep = 1;
    int8  cNBlock = 0, cDirTemp;
    int8 cX,cY;
    GucCrossroad=0;
    cX = GmcMouse.cX;
    cY = GmcMouse.cY;
    mapStepEdit(cXdst,cYdst);                                           /*  制作等高图                  */
    
    /*
     *  根据等高值向目标点运动，直到达到目的地
     */
    while ((cX != cXdst) || (cY != cYdst)) {
        
        ucStep = GucMapStep[cX][cY];
        /*
         *  任选一个等高值比当前自身等高值小的方向前进
         */
        if ((GucMapBlock[cX][cY] & 0x01) &&                             /*  上方有路                    */
            (GucMapStep[cX][cY + 1] < ucStep)) {                        /*  上方等高值较小              */
            cDirTemp = UP;                                              /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cY++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  右方有路                    */
            (GucMapStep[cX + 1][cY] < ucStep)) {                        /*  右方等高值较小              */
            cDirTemp = RIGHT;                                           /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cX++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&                             /*  下方有路                    */
            (GucMapStep[cX][cY - 1] < ucStep)) {                        /*  下方等高值较小              */
            cDirTemp = DOWN;                                            /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cY--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&                             /*  左方有路                    */
            (GucMapStep[cX - 1][cY] < ucStep)) {                        /*  左方等高值较小              */
            cDirTemp = LEFT;                                            /*  记录方向                    */
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                cNBlock++;                                              /*  前进一个方格                */
                cX--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  跳过本次循环                */
            }
        }
        cDirTemp = (cDirTemp + 4 - GucMouseDir)%4;                      /*  计算方向偏移量              */
        GucDirTemp = cDirTemp;
        if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
              mouseGoahead_h(cNBlock);                                      /*  前进cNBlock步               */
          else{
            mouseGoahead_hLlow(cNBlock);                    //到达终点回来
            GucCrossroad = 0;
          }
          
          
        }        
        cNBlock = 0;  
        /*  任务清零                    */
        
        /*
         *  控制电脑鼠转弯
         */
         
       switch (cDirTemp) {

        case 1:
            mouseTurnright_H();
						//mouseTurnright();
            break;

        case 2:
            mouseTurnback();
						//mouseTurnback();
            break;

        case 3:
            mouseTurnleft_H();
						//mouseTurnleft();
            break;

        default:
            break;
        }
      GmcMouse.cX=cX;
      GmcMouse.cY=cY;
    }
    /*
     *  判断任务是否完成，否则继续前进
     */
    
      if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))//单个转弯
            mouseGoahead_h(cNBlock);                                      /*  前进cNBlock步               */
          else{
            mouseGoahead_hLlow(cNBlock);//连续转弯
            GucCrossroad = 0;
          }
          GmcMouse.cX=cX;
          GmcMouse.cY=cY;
      }
}
/*********************************************************************************************************
** Function name:       mouseSpurt
** Descriptions:        电脑鼠从起点以最短路径跑向终点
** input parameters:    无
 ** output parameters:  无
** Returned value:      无
*********************************************************************************************************/
void mouseSpurt_C(void)
{
    uint8 ucTemp = 0xff;
    int8 cXdst = 0,cYdst = 0;
 
    if (GucMapBlock[GucXGoal0][GucYGoal0] & 0x0c) {                     
        mapStepEdit(GucXGoal0,GucYGoal0);                               
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal0][GucYGoal1] & 0x09) {                  
        mapStepEdit(GucXGoal0,GucYGoal1);                              
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {               
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal0] & 0x06) {                    
        mapStepEdit(GucXGoal1,GucYGoal0);                              
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {             
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal1] & 0x03) {                    
        mapStepEdit(GucXGoal1,GucYGoal1);                               
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {               
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    
    objectGoTo(cXdst,cYdst);                                          

}
void mouseSpurt_SC(void)
{
    uint8 ucTemp = 0xff;
    int8 cXdst = 0,cYdst = 0;
 
    if (GucMapBlock[GucXGoal0][GucYGoal0] & 0x0c) {                     
        mapStepEdit(GucXGoal0,GucYGoal0);                               
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal0][GucYGoal1] & 0x09) {                  
        mapStepEdit(GucXGoal0,GucYGoal1);                              
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {               
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal0] & 0x06) {                    
        mapStepEdit(GucXGoal1,GucYGoal0);                              
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {             
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal1] & 0x03) {                    
        mapStepEdit(GucXGoal1,GucYGoal1);                               
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {               
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    
    objectGoTo_SC(cXdst,cYdst);                                          

}
void objectGoTo_SC(int8  cXdst, int8  cYdst)
{
    uint8 ucStep = 1;
    int8  cNBlock = 0, cDirTemp;
    int8 cX,cY;
    GucCrossroad=0;
    cX = GmcMouse.cX;
    cY = GmcMouse.cY;
    mapStepEdit(cXdst,cYdst);                                           /*  ?????                  */
    
    /*
     *  ???????????,???????
     */
    while ((cX != cXdst) || (cY != cYdst)) {
        
        ucStep = GucMapStep[cX][cY];
        /*
         *  ?????????????????????
         */
        if ((GucMapBlock[cX][cY] & 0x01) &&                            
            (GucMapStep[cX][cY + 1] < ucStep)) {                        
            cDirTemp = UP;                                              /*  ????                    */
            if (cDirTemp == GucMouseDir) {                              /*  ????????????    */
                cNBlock++;                                              /*  ??????                */
                cY++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  ??????                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  ????                    */
            (GucMapStep[cX + 1][cY] < ucStep)) {                        /*  ???????              */
            cDirTemp = RIGHT;                                           /*  ????                    */
            if (cDirTemp == GucMouseDir) {                              /*  ????????????    */
                cNBlock++;                                              /*  ??????                */
                cX++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  ??????                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&                             /*  ????                    */
            (GucMapStep[cX][cY - 1] < ucStep)) {                        /*  ???????              */
            cDirTemp = DOWN;                                            /*  ????                    */
            if (cDirTemp == GucMouseDir) {                              /*  ????????????    */
                cNBlock++;                                              /*  ??????                */
                cY--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  ??????                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&                             /*  ????                    */
            (GucMapStep[cX - 1][cY] < ucStep)) {                        /*  ???????              */
            cDirTemp = LEFT;                                            /*  ????                    */
            if (cDirTemp == GucMouseDir) {                              /*  ????????????    */
                cNBlock++;                                              /*  ??????                */
                cX--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  ??????                */
            }
        }
        cDirTemp = (cDirTemp + 4 - GucMouseDir)%4;                      /*  ???????              */
        GucDirTemp = cDirTemp;
        if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
              mouseGoahead_SC(cNBlock);                                      /*  ??cNBlock?               */
          else{
            mouseGoahead_SC_Llow(cNBlock);
          // mouseGoaheadhui(cNBlock);
            GucCrossroad = 0;
          }
          
          
        }        
        cNBlock = 0;  
        /*  ????                    */
        
        /*
         *  ???????
         */
         
       switch (cDirTemp) {

        case 1:
          // mouseTurnright_C(); 
          mouseTurnright_SC();
            break;

        case 2:
            mouseTurnback();
            break;

        case 3:
        //  mouseTurnleft_C();  
          mouseTurnleft_SC();
            break;

        default:
            break;
        }
      GmcMouse.cX=cX;
      GmcMouse.cY=cY;
    }
    /*
     *  ????????,??????
     */
    
      if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
            mouseGoahead_C3(cNBlock);                                      /*  ??cNBlock?               */
          else{
            mouseGoahead_C3_Llow(cNBlock);
            GucCrossroad = 0;
          }
          GmcMouse.cX=cX;
          GmcMouse.cY=cY;
      }
}
/*********************************************************************************************************
** Function name:       mouseSpurt
** Descriptions:        电脑鼠从起点以最短路径跑向终点
** input parameters:    无
 ** output parameters:  无
** Returned value:      无
*********************************************************************************************************/
void mouseSpurt_LL(void)
{
    uint8 ucTemp = 0xff;
    int8 cXdst = 0,cYdst = 0;
    /*
     *  ???????????????
     *  ???????????????
     */
    if (GucMapBlock[GucXGoal0][GucYGoal0] & 0x0c) {                     /*  ????????????    */
        mapStepEdit(GucXGoal0,GucYGoal0);                               /*  ?????                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  ??????????        */
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal0][GucYGoal1] & 0x09) {                     /*  ????????????    */
        mapStepEdit(GucXGoal0,GucYGoal1);                               /*  ?????                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  ??????????        */
            cXdst  = GucXGoal0;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal0] & 0x06) {                     /*  ????????????    */
        mapStepEdit(GucXGoal1,GucYGoal0);                               /*  ?????                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  ??????????        */
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal0;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    if (GucMapBlock[GucXGoal1][GucYGoal1] & 0x03) {                     /*  ????????????    */
        mapStepEdit(GucXGoal1,GucYGoal1);                               /*  ?????                  */
        if (ucTemp > GucMapStep[GucXStart][GucYStart]) {                /*  ??????????        */
            cXdst  = GucXGoal1;
            cYdst  = GucYGoal1;
            ucTemp = GucMapStep[GucXStart][GucYStart];
        }
    }
    
    objectGoTo2(cXdst,cYdst);                                            /*  ????????            */
    //objectGoTo2(cXdst,cYdst);
    //objectGoTo(cXdst,cYdst);
}
void objectGoTo2(int8  cXdst, int8  cYdst)
{
    uint8 ucStep = 1;
    int8  cNBlock = 0, cDirTemp;
    int8 cX,cY;
    GucCrossroad=0;
    cX = GmcMouse.cX;
    cY = GmcMouse.cY;
    mapStepEdit(cXdst,cYdst);                                           /*  ?????                  */
    
    /*
     *  ???????????,???????
     */
    while ((cX != cXdst) || (cY != cYdst)) {
        
        ucStep = GucMapStep[cX][cY];
        /*
         *  ?????????????????????
         */
        if ((GucMapBlock[cX][cY] & 0x01) &&                            
            (GucMapStep[cX][cY + 1] < ucStep)) {                        
            cDirTemp = UP;                                              /*  ????                    */
            if (cDirTemp == GucMouseDir) {                              /*  ????????????    */
                cNBlock++;                                              /*  ??????                */
                cY++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  ??????                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x02) &&                             /*  ????                    */
            (GucMapStep[cX + 1][cY] < ucStep)) {                        /*  ???????              */
            cDirTemp = RIGHT;                                           /*  ????                    */
            if (cDirTemp == GucMouseDir) {                              /*  ????????????    */
                cNBlock++;                                              /*  ??????                */
                cX++;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  ??????                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x04) &&                             /*  ????                    */
            (GucMapStep[cX][cY - 1] < ucStep)) {                        /*  ???????              */
            cDirTemp = DOWN;                                            /*  ????                    */
            if (cDirTemp == GucMouseDir) {                              /*  ????????????    */
                cNBlock++;                                              /*  ??????                */
                cY--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  ??????                */
            }
        }
        if ((GucMapBlock[cX][cY] & 0x08) &&                             /*  ????                    */
            (GucMapStep[cX - 1][cY] < ucStep)) {                        /*  ???????              */
            cDirTemp = LEFT;                                            /*  ????                    */
            if (cDirTemp == GucMouseDir) {                              /*  ????????????    */
                cNBlock++;                                              /*  ??????                */
                cX--;
                if((GucMapBlock[cX][cY] & 0x0f)==0x0f)
                  GucCrossroad++;
                continue;                                               /*  ??????                */
            }
        }
        cDirTemp = (cDirTemp + 4 - GucMouseDir)%4;                      /*  ???????              */
        GucDirTemp = cDirTemp;
        if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
              mouseGoahead_C3(cNBlock);                                      /*  ??cNBlock?               */
          else{
            mouseGoahead_C3_Llow(cNBlock);
          // mouseGoaheadhui(cNBlock);
            GucCrossroad = 0;
          }
          
          
        }        
        cNBlock = 0;  
        /*  ????                    */
        
        /*
         *  ???????
         */
         
       switch (cDirTemp) {

        case 1:
          // mouseTurnright_C(); 
          mouseTurnright_KC();
            break;

        case 2:
            mouseTurnback();
            break;

        case 3:
        //  mouseTurnleft_C();  
          mouseTurnleft_KC();
            break;

        default:
            break;
        }
      GmcMouse.cX=cX;
      GmcMouse.cY=cY;
    }
    /*
     *  ????????,??????
     */
    
      if (cNBlock) {
          if((GucCrossroad <= 1)&&(cNBlock>1))
            mouseGoahead_C3(cNBlock);                                      /*  ??cNBlock?               */
          else{
            mouseGoahead_C3_Llow(cNBlock);
            GucCrossroad = 0;
          }
          GmcMouse.cX=cX;
          GmcMouse.cY=cY;
      }
}
/*********************************************************************************************************
** Function name:       mazeBlockDataGet
** Descriptions:        根据电脑鼠的相对方向，取出该方向上迷宫格的墙壁资料
** input parameters:    ucDir: 电脑鼠的相对方向
** output parameters:   无
** Returned value:      GucMapBlock[cX][cY] : 墙壁资料
*********************************************************************************************************/

uint8 mazeBlockDataGet (uint8  ucDirTemp)
{
    int8 cX = 0,cY = 0;
    
    /*
     *  把电脑鼠的相对方向转换为绝对方向
     */
    switch (ucDirTemp) {

    case MOUSEFRONT:
        ucDirTemp = GucMouseDir;
        break;

    case MOUSELEFT:
        ucDirTemp = (GucMouseDir + 3) % 4;
        break;

    case MOUSERIGHT:
        ucDirTemp = (GucMouseDir + 1) % 4;
        break;

    default:
        break;
    }
    
    /*
     *  根据绝对方向计算该方向上相邻格的坐标
     */
    switch (ucDirTemp) {

    case 0:
        cX = GmcMouse.cX;
        cY = GmcMouse.cY + 1;
        break;
        
    case 1:
        cX = GmcMouse.cX + 1;
        cY = GmcMouse.cY;
        break;
        
    case 2:
        cX = GmcMouse.cX;
        cY = GmcMouse.cY - 1;
        break;
        
    case 3:
        cX = GmcMouse.cX - 1;
        cY = GmcMouse.cY;
        break;
        
    default:
        break;
    }
    
    return(GucMapBlock0[cX][cY]);                                        /*  返回迷宫格上的资料          */
}
/*********************************************************************************************************
** Function name:       rightMethod
** Descriptions:        右手法则，优先向右前进
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void rightMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       leftMethod
** Descriptions:        左手法则，优先向左运动
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void leftMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       frontRightMethod
** Descriptions:        中右法则，优先向前运行，其次向右
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void frontRightMethod (void)
 {
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        
        return;                                                         /*  电脑鼠不用转弯              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
}
/*********************************************************************************************************
** Function name:       frontLeftMethod
** Descriptions:        中左法则，优先向前运行，其次向左
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void frontLeftMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
    }
}

/*********************************************************************************************************
** Function name:     void cornerMethod(int start_X)
** Descriptions:      相角法则，根据电脑鼠目前在迷宫中所处的位置搜索回到起点
** input parameters:    start_X   前点的横坐标
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LeftRightMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
		}
		if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
}
/*********************************************************************************************************
** Function name:     void cornerMethod(int start_X)
** Descriptions:      相角法则，根据电脑鼠目前在迷宫中所处的位置搜索回到起点
** input parameters:    start_X   前点的横坐标
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void RightLeftMethod (void)
{
    if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_R) &&         /*  电脑鼠的右边有路            */
        (mazeBlockDataGet(MOUSERIGHT) == 0x00)) {                       /*  电脑鼠的右边没有走过        */
        mouseTurnright();                                               /*  电脑鼠右转                  */
        return;
		}
		 if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_L) &&         /*  电脑鼠的左边有路            */
        (mazeBlockDataGet(MOUSELEFT ) == 0x00)) {                       /*  电脑鼠的左边没有走过        */
        mouseTurnleft();                                                /*  电脑鼠左转                  */
        return;
    }
		if ((GucMapBlock[GmcMouse.cX][GmcMouse.cY] & MOUSEWAY_F) &&         /*  电脑鼠的前方有路            */
        (mazeBlockDataGet(MOUSEFRONT) == 0x00)) {                       /*  电脑鼠的前方没有走过        */
        return;                                                         /*  电脑鼠不用转弯              */
    }
}


/*********************************************************************************************************
** Function name:       centralMethod
** Descriptions:        中心法则，根据电脑鼠目前在迷宫中所处的位置觉定使用何种搜索法则
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void centralMethod (void)
{
    if (GmcMouse.cX & 0x08) {  
        if (GmcMouse.cY & 0x08) {	

            /*
             *  电脑鼠在迷宫右上角
             */ 
            switch (GucMouseDir) {	//绝对方向
                
            case UP:                                                    /*  当前电脑鼠朝上           */
                leftMethod();                                           /*  左手算法            */
                break;

            case RIGHT:                                               
                rightMethod();                                        
                break;

            case DOWN:                                                  
                frontRightMethod();                                    
                break;

            case LEFT:                                                 
                frontLeftMethod();                                   
                break;

            default:
                break;
            }
        } 
		else 
		{

            /*
             *  电脑鼠在迷宫左上角
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  当前电脑鼠朝上          */
                frontLeftMethod();                                      /*  中左法则                */
                break;

            case RIGHT:                                                
                leftMethod();                                        
                break;

            case DOWN:                                                
                rightMethod();                                         
                break;

            case LEFT:                                                  
                frontRightMethod();                                    
                break;

            default:
                break;
            }
        }
    } else {
        if (GmcMouse.cY & 0x08) {

            /*
             * 电脑鼠在迷宫左下角
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  当前电脑鼠朝上             */
                rightMethod();                                          /*  右手法则                  */
                break;

            case RIGHT:                                                
                frontRightMethod();                                    
                break;

            case DOWN:                                                 
                frontLeftMethod();                                      
                break;

            case LEFT:                                                 
                leftMethod();                                           
                break;

            default:
                break;
            }
        } else {

            /*
             *  ′?ê±μ???êó?ú??1?μ?×ó????
             */    
            switch (GucMouseDir) {
                
            case UP:                                                    /*  当前电脑鼠朝上            */
                frontRightMethod();                                     /*  中右法则                  */
                break;

            case RIGHT:                                                 
                frontLeftMethod();                                      
                break;

            case DOWN:                                                 
                leftMethod();                                        
                break;

            case LEFT:                                             
                rightMethod();                                         
                break;

            default:
                break;
            }
        }
    }
}




/*********************************************************************************************************
** Function name:       crosswayCheck
** Descriptions:        统计某坐标存在还未走过的支路数
** input parameters:    ucX，需要检测点的横坐标
**                      ucY，需要检测点的纵坐标
** output parameters:   无
** Returned value:      ucCt，未走过的支路数
*********************************************************************************************************/
uint8 crosswayCheck (int8  cX, int8  cY)
{
    uint8 ucCt = 0;
    if ((GucMapBlock[cX][cY] & 0x01) &&                                 /*  绝对方向，迷宫上方有路      */
        (GucMapBlock0[cX][cY + 1]) == 0x00) {                            /*  绝对方向，迷宫上方未走过    */
        ucCt++;                                                         /*  可前进方向数加1             */
    }
    if ((GucMapBlock[cX][cY] & 0x02) &&                                 /*  绝对方向，迷宫右方有路      */
        (GucMapBlock0[cX + 1][cY]) == 0x00) {                            /*  绝对方向，迷宫右方没有走过  */
        ucCt++;                                                         /*  可前进方向数加1             */
    }
    if ((GucMapBlock[cX][cY] & 0x04) &&                                 /*  绝对方向，迷宫下方有路      */
        (GucMapBlock0[cX][cY - 1]) == 0x00) {                            /*  绝对方向，迷宫下方未走过    */
        ucCt++;                                                         /*  可前进方向数加1             */
    }
    if ((GucMapBlock[cX][cY] & 0x08) &&                                 /*  绝对方向，迷宫左方有路      */
        (GucMapBlock0[cX - 1][cY]) == 0x00) {                            /*  绝对方向，迷宫左方未走过    */
        ucCt++;                                                         /*  可前进方向数加1             */
    }
    return ucCt;
}
/*********************************************************************************************************
** Function name:       crosswayChoice
** Descriptions:        选择一条支路作为前进方向
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void crosswayChoice (void)
{
		switch (SEARCHMETHOD) {      //法则进这里选
        
    case RIGHTMETHOD:
        rightMethod();
        break;
    
    case LEFTMETHOD:
        leftMethod();
        break;
    
    case CENTRALMETHOD:
        centralMethod();
        break;

    case FRONTRIGHTMETHOD:
        frontRightMethod();
        break;

    case FRONTLEFTMETHOD:
        frontLeftMethod();
        break;
    default:
        break;
    }
}


/*********************************************************************************************************
** Function name:       floodwall
** Descriptions:        墙壁资料初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void floodwall(void)
{     uint8 n          = 0;                                                                                            
      uint8 ucTemp     = 0;
        for(n=0;n<MAZETYPE;n++) {                                           /*墙壁初值无法写入，所以在此从新赋值*/
          for(ucTemp=0;ucTemp<MAZETYPE;ucTemp++) {                                         /* 洪水算法所用*/
          GucMapBlock1[n][ucTemp] =0x1f; }}
           n=0;    ucTemp     = 0;
       for(n=0;n<MAZETYPE;n++) {
         GucMapBlock1[0][n] =0x17;          
         GucMapBlock1[MAZETYPE-1][n] =0x1d; }
            n=0;
       for(n=0;n<MAZETYPE;n++) 
       {   GucMapBlock1[n][0] =0x1b;          
         GucMapBlock1[n][MAZETYPE-1] =0x1e; }
               n=0;
       GucMapBlock1[MAZETYPE-1][MAZETYPE-1] =0x1c;
        GucMapBlock1[0][MAZETYPE-1] =0x16;
         GucMapBlock1[0][0] =0x11;
         GucMapBlock1[MAZETYPE-1][0] =0x19;
    
             for(n=0;n<MAZETYPE;n++)                                            /*墙壁初值无法写入，所以在此从新赋值*/
       { for(ucTemp=0;ucTemp<MAZETYPE;ucTemp++)
     {  GucMapBlock[n][ucTemp] =0x10; }}
           n=0;    ucTemp     = 0;
}

/*
用于得到起点坐标
搜到终点之后，保存了从起点到终点的路径了
当回来撞墙或者冲刺失败的时候，可以按SET键直接冲刺
*/
void wallsave(void)
{
   uint8 x = 0;                                                                                            
   uint8 y = 0;
   uint16 GucMapBlock4[16][16]={0};
   for(x=0;x<MAZETYPE;x++) {                                           
      for(y=0;y<MAZETYPE;y++) {                                         
          GucMapBlock4[x][y] =GucMapBlock[x][y]; 
      }
   } 
  STMFLASH_Write(FLASH_SAVE_ADDR_WALL,(u16*)GucMapBlock4,256);

}
void wallget(void)
{
   uint8 x = 0;                                                                                            
   uint8 y = 0;
   uint16 GucMapBlock4[16][16]={0};
   STMFLASH_Read(FLASH_SAVE_ADDR_WALL,(u16*)GucMapBlock4,256);
   delay(10000);
   for(x=0;x<MAZETYPE;x++) {                                           
      for(y=0;y<MAZETYPE;y++) {                                         
          GucMapBlock[x][y] =(uint8)GucMapBlock4[x][y]; 
      }
   }   
}
unsigned short int Start_Debug = 0;
void StartGet(void)
{
	uint16 start = 0;
  uint8  start1 = 0;
  STMFLASH_Read(FLASH_SAVE_ADDR_START,&start,1);
	Start_Debug = start;
  start1 = (uint8)start;
	//Start_Debug = start1;
  if(start1 == 15)
  {
    GucXStart   = MAZETYPE - 1;                             /*  ???????????      */
    GmcMouse.cX = MAZETYPE - 1; 
  }
}


void centralMethodnew(void){
    int m =0,flag=0;                          /*  统计可前进的支路数          */
  
    while(1){
      if( (((GmcMouse.cX==GucXGoal0)&&(GmcMouse.cY==GucYGoal0)) || 
          ((GmcMouse.cX==GucXGoal1)&&(GmcMouse.cY==GucYGoal1)) ||
          ((GmcMouse.cX==GucXGoal0)&&(GmcMouse.cY==GucYGoal1))||
          ((GmcMouse.cX==GucXGoal1)&&(GmcMouse.cY==GucYGoal0))))
          goto End2;
          m = crosswayCheck(GmcMouse.cX,GmcMouse.cY);
            if (m>0) {                                           /*  有可前进方向                */
               crosswayChoice();                                       /*  选择搜索法则  */
               mazeSearch();  
            } 
            else if (m==0) {
               flag=1;      
               goto End2;
            }
            else;
      }
End2: if(flag)
       floodMethodnew(GucXGoal0, GucYGoal0);
      else;
        
}
/*********************************************************************************************************
** Function name:     floodMethodnew
** Descriptions:       new Method
** input parameters:    无
** output parameters:   无
** Returned value:     
*********************************************************************************************************/
 void floodMethodnew(int8  cXdst, int8  cYdst){
    uint8 flag=0;
    uint8 n=0;
    uint8 ucStep = 0;
    int8  cNBlock = 0, cDirTemp;
    int8 cX,cY;
    int8 t=0;
    cX = GmcMouse.cX;
    cY = GmcMouse.cY;
    mapStepEdithong(cXdst,cYdst);
      if(((cXdst!=GucXGoal0)&&(cXdst!=GucXGoal1))||
         ((cYdst!=GucYGoal0)&&(cYdst!=GucYGoal1))) {       /*判断目标点是否为终点*/     
        t=1; }
    while ((cX!=cXdst)||(cY!=cYdst)) {
    if( (((GmcMouse.cX==GucXGoal0)&&(GmcMouse.cY==GucYGoal0)) || 
         ((GmcMouse.cX==GucXGoal1)&&(GmcMouse.cY==GucYGoal1)) ||
         ((GmcMouse.cX==GucXGoal0)&&(GmcMouse.cY==GucYGoal1))||
         ((GmcMouse.cX==GucXGoal1)&&(GmcMouse.cY==GucYGoal0)))
           &&(t==0)  )
       { cDirTemp=GucMouseDir;
         break;}
    
   
     n=0;
     ucStep = GucMapStep1[cX][cY]; 
        /*
         *  任选一个等高值比当前自身等高值小的方向前进
         */
      if ((GucMapBlock1[cX][cY] & 0x01) &&                             /*  上方有路                    */
          (GucMapStep1[cX][cY + 1] < ucStep)
          &&(GucMapBlock0[cX][cY + 1]>0x00)) {                        /*  上方等高值较小              */
            cDirTemp = UP;
            n++;
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */
                    goto T;                                     
            }
        }
        if ((GucMapBlock1[cX][cY] & 0x02) &&                           
            (GucMapStep1[cX + 1][cY] < ucStep)
            &&(GucMapBlock0[cX + 1][cY] >0x00)) {                      
            cDirTemp = RIGHT;
             n++;
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */            
              goto T; 
            }
        }
        if ((GucMapBlock1[cX][cY] & 0x04) &&                             /*  下方有路                    */
            (GucMapStep1[cX][cY - 1] < ucStep)
            &&(GucMapBlock0[cX][cY - 1]>0x00)) {                        /*  下方等高值较小              */
            cDirTemp = DOWN;
            n++;
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */              
              goto T; 
            }
        }
        if ((GucMapBlock1[cX][cY] & 0x08) &&                             /*  左方有路                    */
            (GucMapStep1[cX - 1][cY] < ucStep)
             &&(GucMapBlock0[cX - 1][cY]>0x00)) {                        /*  左方等高值较小              */
             cDirTemp = LEFT;
             n++;
            if (cDirTemp == GucMouseDir) {                              /*  优先选择不需要转弯的方向    */           
              goto T;                
            }
        }      
      if(n==0){
       flag=1;
       goto End1;
      }
      cDirTemp = (cDirTemp + 4 - GucMouseDir)%4;                      /*  计算方向偏移量              */
           
        switch (cDirTemp) {

        case 1:
          if(GucMouseTurn){
            if(DenggaoCheck ()== FE){
               while(PulseCtrCheck ()== FE);
               mouseStop();
               mouseTurnright_Y();
            }
            GucMouseTurn =0;
          }
          else
            mouseTurnright();
           cX = GmcMouse.cX;
           cY = GmcMouse.cY;
             break;
         case 2:
            GucMouseTurn =0;
            mouseTurnback();
            cX = GmcMouse.cX;
            cY = GmcMouse.cY;
            break;

        case 3:
          if(GucMouseTurn){
            if(DenggaoCheck ()== FE){
               while(PulseCtrCheck ()== FE);
               mouseStop();
               mouseTurnleft_Y();
            }
            GucMouseTurn =0;
          }
          else
            mouseTurnleft();
            cX = GmcMouse.cX;
            cY = GmcMouse.cY;
            break;

        default:
            break;
        }
    
T:   mazeSearch();
     cX = GmcMouse.cX;
     cY = GmcMouse.cY;
      
    }

End1: if(flag)
        centralMethodnew();
      else;
}



/*********************************************************************************************************
** Function name:       __ir_Get
** Descriptions:        读取E2PROM中的红外频率
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __ir_Get(void)
{ 
	
  unsigned short int DIS1[10] = {0};
  STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)DIS1,10);
  delay(1000000);
  GusWidthFrontLeftNear  = DIS1[0];						//前方左近距
  GusWidthFrontLeftFar   = DIS1[1];						//前方左远距
  GusWidthFrontRightNear  = DIS1[2];						//前方右近距
  GusWidthFrontRightFar   = DIS1[3];						//前方右远距
  GusWidthRightNear  = DIS1[4];								//右近距
  GusWidthRightFar   = DIS1[5];								//右远距
  GusWidthLeftNear  = DIS1[6];									//左近距
  GusWidthLeftFar   = DIS1[7];									//左远距
  GusWidthXieLeft   = DIS1[8];									//斜45度左
  GusWidthXieRight  = DIS1[9];									//斜45度右
}


extern __MOTOR  __GmLeft;
struct __motor;
int main (void)
{
	    uint8 LF0          = 0;
	  uint8 LF1          = 0;
    uint8 n          = 0;                                               /*  GmcCrossway[]下标           */
	  uint8 ucRoadStat = 0;                                               /*  统计某一坐标可前进的支路数  */
    uint8 ucTemp     = 0;                                               /*  用于START状态中坐标转换     */
	  uint8 Start_1    =0;	
    uint8 Start_2  = 0;			
	  unsigned short int GucXStartBuffer  = 0;
		delay(10000);
	  mouseInit();                                                        /*  底层驱动的初始化            */
    __ir_Get();
    delay(10000);
	  floodwall();	
 //  TIM2->CCR3 = 1600;
    while (1) 
			{
//				/*数步*/
//					       if (startCheck() == KEY_TRUE)
////									if(wcg==1)
//                 {
//									 TIM2->CCR3=0;
//									 delay(10000000);
//									 delay(10000000); 
////									 mouseGoahead_h(6);
//									mouseTurnleft();
//									 mouseStop();
//									 	 
//								   while(1)
//                     break;
//                 }	
				
				
			SystemVolt = Get_Adc(9);
        switch (GucMouseTask) {                                         /*  状态机处理                  */    					
        case WAIT:
            sensorDebug();                              //挑红外的
            delay(100000);
						LedMouseTast = 0;
				
				 if (startCheck() == KEY_TRUE) 
					 {   						 /*  开始按键按下  */
             LF0++;
             if(LF0==1)						 
						 Start_1=1;						 // zlg7289Reset();   /*  ??ZLG7289                 */
	           if(LF0==2)						 
						 Start_1=2;
						 if(LF0==3)						 
						 Start_1=3;
					 delay(1000);
																																			//  GucMouseTask = START;
            }
				 if (keyCheck() == KEY_TRUE) 
					 {                                  /*  冲刺按键           */
             
					   LF1++;
             if(LF1==1)						 
						 Start_2=1;						 // zlg7289Reset();   /*  ??ZLG7289                 */
	           if(LF1==2)						 
						 Start_2=2;
						 if(LF1==3)						 
						 Start_2=3;  
					   delay(100);
		
					   
          }					
         if((Start_1==1)&&GucMouseStart)
         {
              Start_1=0;
					   SEARCHMETHOD=RIGHTMETHOD;  //法则选择  右手
             GucMouseTask=START;
              zlg7289Reset();
							LedMouseTast = 1;		//代表开始
							//LED0 = 0;
              while(GucMouseStart);
//              delay(1000000);
							delay_ms(100);
	//						delay(1000000);
       
          }
					if((Start_1==2)&&GucMouseStart)
         {
              Start_1=0;
					   SEARCHMETHOD= CENTRALMETHOD   ;   //中心
             GucMouseTask=START;
              zlg7289Reset();
							LedMouseTast = 1;		//代表开始
					
              while(GucMouseStart);
              delay(1000000);
							delay_ms(100);
							
							delay(1000000);
         
          }
					if((Start_1==3)&&GucMouseStart)
         {
              Start_1=0;
					   SEARCHMETHOD=LEFTMETHOD;  //左手
             GucMouseTask=START;
              zlg7289Reset();
							LedMouseTast = 1;		//代表开始
						
              while(GucMouseStart);
              delay(1000000);
							delay_ms(100);
						
							delay(1000000);
         
          }
					if((Start_2==1)&&GucMouseStart)
						{
            Start_2 = 0;  
            wallget();
						delay(10000);
            StartGet();
						delay(10000);
            GucMouseTask =SPURT;
					
            zlg7289Reset();
						LedMouseTast = 1;		//代表开始
            while(GucMouseStart);
            delay(10000);
						delay_ms(10);
						
							delay(10000);
        
          }
					if((Start_2==2)&&GucMouseStart)
						{
            Start_2 = 0;  
            wallget();
						delay(10000);
            StartGet();
						delay(10000);
            GucMouseTask =SPURTL;
            zlg7289Reset();
						LedMouseTast = 1;		//代表开始
            while(GucMouseStart);
            delay(10000);
						delay_ms(10);
							
							delay(10000);
        
          }
					if((Start_2==3)&&GucMouseStart)
						{
            Start_2 = 0;  
            wallget();
						delay(10000);
            StartGet();
						delay(10000);
            GucMouseTask =SPURTLL;
            zlg7289Reset();
						LedMouseTast = 1;		//代表开始
            while(GucMouseStart);
            delay(10000);
						delay_ms(10);
					
							delay(10000);
    
          }
          break;     
        case START:  			
            mazeSearch();                                               /*  向前搜索                    */
            if (GucMapBlock[GmcMouse.cX][GmcMouse.cY] & 0x08) {         /*  判断电老鼠左边是否存在出口  */
                if (MAZETYPE == 16) {                                    /*  修改四分之一迷宫的终点坐标  */
                    GucXGoal0 = 7;
                    GucXGoal1 = 8;
                }
                GucXStart   = MAZETYPE - 1;                             /*  修改电脑鼠起点的横坐标      */
                GmcMouse.cX = MAZETYPE - 1;                             /*  修改电脑鼠当前位置的横坐标  */    
                /*
                 *  由于默认的起点为(0,0)，现在需要把已记录的墙壁资料转换过来
                 */
                ucTemp = GmcMouse.cY;
								GucMapBlock[MAZETYPE - 2][ucTemp]=0x12;
                do {
                    GucMapBlock[MAZETYPE - 1][ucTemp] = GucMapBlock[0][ucTemp];
                    GucMapBlock0[MAZETYPE - 1][ucTemp] = GucMapBlock0[0][ucTemp];
                    GucMapBlock0[0][ucTemp]=0;
                    GucMapBlock1[MAZETYPE - 1][ucTemp] = GucMapBlock1[0][ucTemp];    
                    if(ucTemp>0){
                       if(ucTemp>1)                                           //1
												  GucMapBlock1[MAZETYPE - 2][ucTemp-1]=0x1d;           //2
											 else                                                   //3
                         GucMapBlock1[MAZETYPE - 2][ucTemp-1]=0x19;}         //4
                    GucMapBlock1[0 ][ucTemp+1] =0x17;
                    GucMapBlock1[1 ][ucTemp+1] =0x1f;                           //5
									  GucMapBlock1[1 ][0] =0x1b;                                //6
                    GucMapBlock[0 ][ucTemp] =0x10;
                    GucMapBlock[1 ][ucTemp] =0x10;
                }while (ucTemp--);
								    GucMapBlock1[0][0] =0x13;
                    GucMapBlock1[1][0] =0x1b;
                    GucMapBlock1[MAZETYPE - 2][0] =0x19;      
                /*
                 *  在OFFSHOOT[0]中保存起点坐标
                 */
                GmcCrossway[n].cX = MAZETYPE - 1;
                GmcCrossway[n].cY = 0;
                n++;
                GucMouseTask = MAZESEARCH;                              /*  状态转换为搜寻状态          */
            }
            if (GucMapBlock[GmcMouse.cX][GmcMouse.cY] & 0x02) {         /*  判断电老鼠右边是否存在出口  */
                /*
                 *  在OFFSHOOT[0]中保存起点坐标
                 */
                GmcCrossway[n].cX = 0;
                GmcCrossway[n].cY = 0;
                n++;
                GucMouseTask = MAZESEARCH;                              /*  状态转换为搜寻状态          */
            }
							
						
            break;
           

           case MAZESEARCH:       
                centralMethodnew();        //到达终点
					      mouseTurnback_H();        //终点 原地后退
				//////////////////////不需要看/////////////////////////////////////////////////
					      GucXStartBuffer = GucXStart;
 					    	STMFLASH_Write(FLASH_SAVE_ADDR_START,&GucXStartBuffer,1);
                goalwall();            //做等高图，更新坐标
                wallsave();            //保存坐标
					 /////////////////////////////////////////////////////////////////
               		 delay(600000); 
                     delay(600000); 
                     delay(600000); 
                objectGoTo_h(GucXStart,GucYStart);
					      delay(100000);
                TIM2 -> CCR3 = 0; 					 
					      mouseTurnback_Y();
                GucMouseTask = SPURT;           
           break;
       /*一冲*/
      case SPURT:
				     hang=1;
             wallget();
				     delay(600000); 
                     delay(600000); 
                     delay(600000); 
			       TIM2->CCR3 = 1800;
				     mouseSpurt();       //电脑鼠以最短径跑向终点    
				     mouseTurnback_H();    //搜索完回到原点后退
      				     delay(600000); 
                     delay(600000); 
            
			       TIM2->CCR3 = 0;
      				     delay(600000); 
                     delay(600000); 
                     delay(600000); 
			       objectGoTo_h(GucXStart,GucYStart);
       			 TIM2->CCR3 =0;			
         		 mouseTurnback();  /*  向后转，恢复出发姿势        */                     
				     GucMouseTask = WAIT;
             break;		
			
			
	  case SPURTL:
             wallget();
				     delay(100000); 
             TIM2->CCR3 =900;			
				     mouseSpurt_C();
             TIM2->CCR3 =0;		
						 mouseTurnback_Y(); 	
             objectGoTo_h(GucXStart,GucYStart);			/*  回起点          */		
         		 mouseTurnback();  /*  向后转，恢复出发姿势        */                                       			
  	         GucMouseTask =WAIT;  
			  case SPURTLL:
             wallget();
				     delay(100000); 
             TIM2->CCR3 =900;			
				     mouseSpurt_SC();
             TIM2->CCR3 =0;		
						 mouseTurnback_Y(); 	
             objectGoTo_h(GucXStart,GucYStart);			/*  回起点          */		
         		 mouseTurnback();  /*  向后转，恢复出发姿势        */                                       			
  	         GucMouseTask =WAIT; 
             while (1) 
             {
                 if (startCheck() == KEY_TRUE)
                 {
                     break;
                 }
             }
            break;

        default:
            break;
        }
    }
}
      
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
