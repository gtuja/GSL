/**
 * @file    gsl_diag.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_feature.h"
#include "gsl.h"

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PRIVATE U32 gu32DiagTickBefore;
PRIVATE BOOL bIsDiagTimeStarted = FALSE;

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC void vidGslDiagTimeStart(void) {
  gu32DiagTickBefore = u32GslGetTick();
  bIsDiagTimeStarted = TRUE;
}

PUBLIC U32 u32GslDiagGetElapsedTime(void) {
  U32 u32TickElapsed;
  U32 u32TickCurrent;

  u32TickCurrent = u32GslGetTick();
  u32TickElapsed = (U32)0;

  if (bIsDiagTimeStarted == TRUE) {
    u32TickElapsed = (u32TickCurrent >= gu32DiagTickBefore) ? \
                      (u32TickCurrent - gu32DiagTickBefore) : (u32TickCurrent + u32GslGetTickCounterPeriod() - gu32DiagTickBefore);
  }
  bIsDiagTimeStarted = FALSE;
  return u32TickElapsed;
}
