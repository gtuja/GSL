/**
 * @file    app_callback.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 10, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
//#include "stm32l0xx_hal.h"
#include "app_api.h"
#include <stdio.h>
#include "gsl_api.h"

/* External variables --------------------------------------------------------*/
//EXTERN TIM_HandleTypeDef htim21;
//EXTERN TIM_HandleTypeDef htim2;

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

#ifdef __COMMENT_REFER
  switch (enuType) {
    case BSM_TYPE_B0 :
      enuEvent = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) ? BSM_EVT_PSH : BSM_EVT_RLS;
      break;
    default :
      break;
  }
#endif /* __COMMENT_REFER */
  return enuEvent;
}

PUBLIC tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuLsmType) {
  tenuLsmEvent  enuReturn = LSM_EVT_NA;
#ifdef __COMMENT_REFER
  tenuBsmNotify enuBsmNotify;
  U32           u32PwmDuty;

  switch (enuLsmType) {
    case LSM_TYPE_L0 :
      u32PwmDuty = (U32)__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);
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
    default :
      break;
  }
#endif /* __COMMENT_REFER */

  return enuReturn;
}

PUBLIC void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty) {
#ifdef __COMMENT_REFER
  switch (enuType) {
    case LSM_TYPE_L0 :
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, u32PwmDuty);  /* Set PWM duty 0 on TIM2, Channel#1. */
      break;
    default :
      break;
  }
#endif /* __COMMENT_REFER */
  return;
}

PUBLIC U32 u32DiagTusCntCallback(void* pvArgs) {
#ifdef __COMMENT_REFER
  return (U32)TIM22->CNT;
#else
  return 0;
#endif /* __COMMENT_REFER */
}

PUBLIC U32 u32DiagTusCntPrdCallback(void* pvArgs) {
#ifdef __COMMENT_REFER
  return (U32)TIM22->ARR;
#else
  return 0;
#endif /* __COMMENT_REFER */
}

PUBLIC void vidDiagTraceCallback(char* pcTrace) {
#ifdef __COMMENT_REFER
  printf("%02d: %s\r\n", (int)((gu32TraceCounter++)%100), pcTrace);
#endif /* __COMMENT_REFER */
}

#ifdef __COMMENT_REFER
PUBLIC void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim21) {
    vidGslSrvcCallback(gNULL);
  }
  return;
}
#endif /* __COMMENT_REFER */

