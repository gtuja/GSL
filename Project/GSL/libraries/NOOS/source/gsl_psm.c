/**
 * @file    gsl_psm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_feature.h"
#include "gsl_api.h"
#include "gsl_bsm.h"
#include "gsl_lsm.h"
#include "gsl_dsm.h"

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PRIVATE U32 gu32PsmClockCounter;

/* Public functions ----------------------------------------------------------*/
/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC void vidPsmInitialize(void* pvArgs) {
  gu32PsmClockCounter = (U32)0;
  vidBsmInitialize(NULL);
  vidLsmInitialize(NULL);
}

PUBLIC void vidPsmService(void* pvArgs) {
  U32 i;
  gu32PsmClockCounter++;

  for (i=0; i<(U32)PSM_TYPE_MAX; i++)  {
    if (gcpstrPsmCfgTbl[i].u32Period != (U16)0) {
      if ((gu32PsmClockCounter % gcpstrPsmCfgTbl[i].u32Period) == (U32)0) {
        if (gcpstrPsmCfgTbl[i].pfPsmService != NULL) {
          gcpstrPsmCfgTbl[i].pfPsmService(NULL);
        }
      }
    }
  }
}

PUBLIC void vidPsmServiceDsm(void* pvArgs) {
  vidDsmService(NULL);
}

PUBLIC void vidPsmServiceBsm(void* pvArgs) {
  vidBsmService(NULL);
}

PUBLIC void vidPsmServiceLsm(void* pvArgs) {
//  vidLsmService(NULL);
}

