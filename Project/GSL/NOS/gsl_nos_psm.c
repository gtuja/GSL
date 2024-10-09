/**
 * @file    gsl_nos_psm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 9, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"
#include "gsl_feature.h"
#include "gsl_nos_api.h"
#include <string.h>

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct {
  tenuGnosPriority  enuPriority;
  tpfGnosInitialize pfInitialize;
  tpfGnosService    pfService;
} tstrGnosService;

/* Private function prototypes -----------------------------------------------*/
PRIVATE void vidGnosServiceDetail(tenuGnosPriority enuGnosPriority);

/* Private variables ---------------------------------------------------------*/
PRIVATE U64 gullGnosCounter;

PRIVATE tstrGnosService gstrGnosService[GNOS_PRD_MAX];

/* Exported variables --------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
PUBLIC void vidGnosInitialize(void* pvArgs) {
  gullGnosCounter = 0;
  memset(gstrGnosService, 0, sizeof(gstrGnosService));
}

PUBLIC void vidGnosService(void* pvArgs) {
  gullGnosCounter++;

  if ((gullGnosCounter % GNOS_PS_INTRVL_LOW) == (U64)0)    vidGnosServiceDetail(GNOS_PS_INTRVL_LOW);
  if ((gullGnosCounter % GNOS_PS_INTRVL_NORMAL) == (U64)0) vidGnosServiceDetail(GNOS_PS_INTRVL_NORMAL);
  if ((gullGnosCounter % GNOS_PS_INTRVL_HIGH) == (U64)0)   vidGnosServiceDetail(GNOS_PS_INTRVL_HIGH);
}

PUBLIC BOOL bGnosRegister(tenuGnosPriority enuPriority, tpfGnosInitialize pfInitialize, tpfGnosService pfService)
{
  U8 i;
  BOOL bReturn = FALSE;

  for (i=0; i<(U8)GNOS_PRD_MAX; i++)  {
    if (gstrGnosService[i].enuPriority != GNOS_PRRT_NA) {
      gstrGnosService[i].enuPriority = enuPriority;
      gstrGnosService[i].pfInitialize = pfInitialize;
      gstrGnosService[i].pfService = pfService;
      bReturn = TRUE;
      break;
    }
  }
  return bReturn;
}

PRIVATE void vidGnosServiceDetail(tenuGnosPriority enuGnosPriority) {
  U8 i;

  for (i=0; i<(U8)GNOS_PRD_MAX; i++)  {
    if (gstrGnosService[i].enuPriority == enuGnosPriority) {
      if (gstrGnosService[i].pfService != NULL) {
        gstrGnosService[i].pfService(NULL);
      }
    }
  }
}
