/**
 * @file    app_callback.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 10, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_api.h"
#include "stm32l0xx_hal.h"
#include "app_api.h"
#include <stdio.h>

/* External variables --------------------------------------------------------*/
EXTERN TIM_HandleTypeDef htim21;

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PRIVATE U32 gu32TraceCounter = (U32)0;
PUBLIC tstrAppBtnHandle gpstrAppBtnHandle[10] = {0};

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC U32 u32AppTickCallback(void* pvArgs) {
  return (U32)(TIM22->CNT);
}

PUBLIC void vidAppTraceCallback(char* pcTrace) {
  printf("%02d: %s\r\n", (int)((gu32TraceCounter++)%100), pcTrace);
}

PUBLIC BOOL bAppBtnExtractEventCallback(S32 s32Handle) {
  BOOL bReturn = FALSE;

  if (s32Handle != GSL_HNDL_NA && s32Handle < 10) {
    bReturn = (HAL_GPIO_ReadPin(gpstrAppBtnHandle[s32Handle].pGpiox, gpstrAppBtnHandle[s32Handle].u16GpioPin) == GPIO_PIN_RESET) ? FALSE : TRUE;
  }
  return bReturn;
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
    u32Tick22_before[i] = u32AppTickCallback(NULL);
#endif /* __MEASURE_TIM22 */
    vidGslService(NULL);
#if __MEASURE_TIM22
    u32Tick22_after[i] = u32AppTickCallback(NULL);
    i++;
    if (i >= 10) i=0;
#endif /* __MEASURE_TIM22 */
  }
}

