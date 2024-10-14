/**
 * @file    gsl_btm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_feature.h"
#include "gsl_api.h"
#include "gsl_btm.h"
#include "gsl_queue.h"

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */

PUBLIC void vidBtmInitialize(void* pvArgs) {

}

PUBLIC void vidBtmProcess(void* pvArgs) {
  U32 i;

  for (i=0; i<(U32)BTM_TYPE_MAX; i++) {
    if (gcpstrBtmCfgTbl[i].pfBtmProcess != NULL) {
      gcpstrBtmCfgTbl[i].pfBtmProcess(NULL);
    }
  }
}

PUBLIC void vidBtmProcessIdle(void* pvArgs) {
}

PUBLIC void vidBtmProcessTrace(void* pvArgs) {
  if (bGslQueIsEmpty(GSL_QUE_TRACE) == FALSE) {
    gcstrGslCalbacks.pfGslTraceCallback((char*)u32GslQueDequeue(GSL_QUE_TRACE));
  }
}

