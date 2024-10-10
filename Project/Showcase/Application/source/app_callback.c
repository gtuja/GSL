/**
 * @file    app_callback.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 10, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"
#include "gsl_nos_api.h"
#include "stm32l0xx_hal.h"
#include <stdio.h>

/* External variables --------------------------------------------------------*/
EXTERN TIM_HandleTypeDef htim21;

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
PUBLIC U32 u32AppTickCallback(void) {
  return (U32)TIM22->CNT;
}

PUBLIC void vidAppTraceCallback(char* pcTrace) {
  printf("%02d: %s\r\n", (int)((gu32TraceCounter++)%100), pcTrace);
}

#ifdef __MEASURE_TIM22
static U32 u32Tick21_1m[10];
static U32 u32Tick22_1u[10];
static U8 i;
#endif /* __MEASURE_TIM22 */

PUBLIC void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim21) {
#ifdef __MEASURE_TIM22
    if (i<10) {
      u32Tick21_1m[i] = (U32)TIM21->CNT;
      u32Tick22_1u[i] = (U32)TIM22->CNT;  /* about 970 us for each 1ms. */
      i++;
    }
#endif /* __MEASURE_TIM22 */
    vidGnosService(NULL);
  }
}
