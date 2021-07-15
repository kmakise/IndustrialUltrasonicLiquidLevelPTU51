/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   IAR STM32-SK Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

			 /*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  OS_TASK_TMR_PRIO            (OS_LOWEST_PRIO - 4)
#define  TASK0_PRIO                   5
#define  TASK1_PRIO                  6
#define  TASK2_PRIO                  7
#define  TASK3_PRIO                  8
#define  TASK4_PRIO                  9
#define  TASK5_PRIO                  10
#define  TASK6_PRIO                  11
#define  TASK7_PRIO                  12
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  TASK0_STK_SIZE                 200
#define  TASK1_STK_SIZE                 500
#define  TASK2_STK_SIZE                 500
#define  TASK3_STK_SIZE                 500
#define  TASK4_STK_SIZE                 500
#define  TASK5_STK_SIZE                 500
#define  TASK6_STK_SIZE                 500
#define  TASK7_STK_SIZE                 500


#endif
