/**
 * @file    gsl_diag.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_feature.h"
#include "gsl_api.h"
#include "gsl.h"

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PRIVATE U32 gu32DiagTickBefore;
PRIVATE BOOL bIsDiagTimeStarted = FALSE;
PRIVATE U64 gu4DiagTusElapsed = 0;

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC void vidGslDiagTimeStart(void) {
  gu32DiagTickBefore = u32GslTickCountCallback(NULL);
  bIsDiagTimeStarted = TRUE;
}

PUBLIC U32 u32GslDiagTimeElapsed(void) {
  U32 u32TickElapsed;
  U32 u32TickCurrent;

  u32TickCurrent = u32GslTickCountCallback(NULL);
  u32TickElapsed = (U32)0;

  if (bIsDiagTimeStarted == TRUE) {
    if (u32TickCurrent >= gu32DiagTickBefore) {
      u32TickElapsed = u32TickCurrent - gu32DiagTickBefore;
    } else {
      u32TickElapsed = u32TickCurrent + GSL_TUS_PRD_CNT - gu32DiagTickBefore;
    }
  }
  bIsDiagTimeStarted = FALSE;
  return u32TickElapsed;
}

PUBLIC void vidDiagRefreshTus(U32 u32PrdCnt) {
  gu4DiagTusElapsed += (U64)u32PrdCnt;
}

PUBLIC U64 u64DiagGetTusElapsed(void) {
  return gu4DiagTusElapsed;
}
