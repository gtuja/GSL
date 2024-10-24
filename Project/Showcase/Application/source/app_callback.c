/**
 * @file    app_callback.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 10, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "app_api.h"
#include <stdio.h>
#include "../../../GSL/Inc/gsl_api.h"

/* External variables --------------------------------------------------------*/
EXTERN TIM_HandleTypeDef htim21;
EXTERN TIM_HandleTypeDef htim22;
EXTERN TIM_HandleTypeDef htim2;

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PRIVATE U32 gu32TraceCounter = (U32)0;

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */

PUBLIC tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType) {
  tenuBsmEvent enuEvent = BSM_EVT_NA;

  switch (enuType) {
    case BSM_TYPE_B0 :
      enuEvent = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) ? BSM_EVT_PSH : BSM_EVT_RLS;
      break;
    default :
      break;
  }
  return enuEvent;
}

PUBLIC tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuLsmType) {
  tenuBsmNotify enuBsmNotify;
  tenuLsmEvent  enuReturn = LSM_EVT_NA;
  U32           u32PwmDuty;

  switch (enuLsmType) {
    case LSM_TYPE_L0 :
      u32PwmDuty = (U32)__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);
      enuBsmNotify = enuGslBsmNotifyCallback(enuBsmType);
      switch (enuBsmNotify) {
        case BSM_NTF_SHORT :
          if (u32PwmDuty == (U32)0) {
            enuReturn = LSM_EVT_ON;
          } else {
            enuReturn = LSM_EVT_OFF;
          }
          break;
        case BSM_NTF_LONG :
          if (u32PwmDuty == (U32)0) {
            enuReturn = LSM_EVT_FRC_ON;
          } else {
            enuReturn = LSM_EVT_FRC_OFF;
          }
          break;
        default :
          break;
      }
    default :
      break;
  }
  return enuReturn;
}

PUBLIC void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty) {
  switch (enuType) {
    case LSM_TYPE_L0 :
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, u32PwmDuty);  /* Set PWM duty 0 on TIM2, Channel#1. */
      break;
    default :
      break;
  }
  return;
}

PUBLIC U32 enuGslLsmPwmMinCallback(tenuLsmType enuType) {
  U32 u32Return = (U32)0;

  switch (enuType) {
    case LSM_TYPE_L0 :
      u32Return = (U32)0;
      break;
    default :
      break;
  }
  return u32Return;
}

PUBLIC U32 enuGslLsmPwmMaxCallback(tenuLsmType enuType) {
  U32 u32Return = (U32)0;

  switch (enuType) {
    case LSM_TYPE_L0 :
      u32Return = (U32)TIM2->ARR;
      break;
    default :
      break;
  }
  return u32Return;
}

PUBLIC U32 u32DiagCntCallback(void* pvArgs) {
  return (U32)TIM22->CNT;
}

PUBLIC U32 u32DiagCntPrdCallback(void* pvArgs) {
  return (U32)TIM22->ARR;
}

PUBLIC void vidDiagTraceCallback(char* pcTrace) {
  printf("%02d: %s\r\n", (int)((gu32TraceCounter++)%100), pcTrace);
}


//#define __MEASURE_TIM22

#ifdef __MEASURE_TIM22
static U32 u32Tick22_before[10];
static U32 u32Tick22_after[10];
static U32 i = 0;
#endif /* __MEASURE_TIM22 */

PUBLIC void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim21) {
#if __MEASURE_TIM22
    u32Tick22_before[i] = u32AppTickCallback(gNULL);
#endif /* __MEASURE_TIM22 */
    vidGslSrvcCallback(gNULL);
#if __MEASURE_TIM22
    u32Tick22_after[i] = u32AppTickCallback(gNULL);
    i++;
    if (i >= 10) i=0;
#endif /* __MEASURE_TIM22 */
  }

  if (htim == &htim22) {
    vidGslDiagElapsedCallback(gNULL);
  }
}

