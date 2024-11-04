/**
 * @file    app_callback.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 10, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "app_api.h"
#include <stdio.h>
#include "gsl_api.h"

/* External variables --------------------------------------------------------*/
EXTERN TIM_HandleTypeDef htim3;
EXTERN TIM_HandleTypeDef htim4;
EXTERN TIM_HandleTypeDef htim12;

EXTERN TIM_HandleTypeDef htim6;
EXTERN TIM_HandleTypeDef htim7;

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PRIVATE U32 gu32TraceCounter = (U32)0;
PRIVATE tenuBsmNotify genuBsmNtfPrev[LSM_LED_MAX][BSM_BTN_MAX] = {0};

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
      enuEvent = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) ? BSM_EVT_RLS : BSM_EVT_PSH;
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
      u32PwmDuty = (U32)__HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_3);
      enuBsmNotify = enuGslBsmNotifyCallback(enuBsmType);
      
      if (enuBsmNotify != genuBsmNtfPrev[(U32)enuLsmType][(U32)enuBsmType]) {
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
        genuBsmNtfPrev[(U32)enuLsmType][(U32)enuBsmType] = enuBsmNotify;
      }
      break;

    case LSM_TYPE_L1 :
      u32PwmDuty = (U32)__HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_2);
      enuBsmNotify = enuGslBsmNotifyCallback(enuBsmType);

      if (enuBsmNotify != genuBsmNtfPrev[(U32)enuLsmType][(U32)enuBsmType]) {
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
        genuBsmNtfPrev[(U32)enuLsmType][(U32)enuBsmType] = enuBsmNotify;
      }
      break;

    case LSM_TYPE_L2 :
      u32PwmDuty = (U32)__HAL_TIM_GET_COMPARE(&htim12, TIM_CHANNEL_1);
      enuBsmNotify = enuGslBsmNotifyCallback(enuBsmType);

      if (enuBsmNotify != genuBsmNtfPrev[(U32)enuLsmType][(U32)enuBsmType]) {
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
        genuBsmNtfPrev[(U32)enuLsmType][(U32)enuBsmType] = enuBsmNotify;
      }
      break;

    default :
      break;
  }

  return enuReturn;
}

PUBLIC void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty) {
  switch (enuType) {
    case LSM_TYPE_L0 :
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, u32PwmDuty);  /* Set PWM duty L1 on TIM3, Channel#3. */
      break;
    case LSM_TYPE_L1 :
      __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, u32PwmDuty);  /* Set PWM duty L2 on TIM4, Channel#2. */
      break;
    case LSM_TYPE_L2 :
      __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, u32PwmDuty);  /* Set PWM duty L3 on TIM12, Channel#1. */
      break;
    default :
      break;
  }
  return;
}

PUBLIC U32 u32DiagTusCntCallback(void* pvArgs) {
  return (U32)TIM7->CNT;
}

PUBLIC U32 u32DiagTusCntPrdCallback(void* pvArgs) {
  return (U32)TIM7->ARR;
}

PUBLIC void vidDiagTraceCallback(char* pcTrace) {
  printf("%02d: %s\r\n", (int)((gu32TraceCounter++)%100), pcTrace);
}

PUBLIC void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim6) {
    vidGslSrvcCallback(gNULL);
  }
}

