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
  U16               u16Period;
  tpfGnosInitialize pfInitialize;
  tpfGnosService    pfService;
} tstrGnosService;

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
PRIVATE U64 gullGnosCounter;
PRIVATE tstrGnosService gstrGnosService[GNOS_PSM_MAX];

/* Exported variables --------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
PUBLIC void vidGnosInitialize(void* pvArgs) {
  gullGnosCounter = 0;
  memset(gstrGnosService, 0, sizeof(gstrGnosService));
}

PUBLIC void vidGnosService(void* pvArgs) {
  U8 i;

  gullGnosCounter++;
  for (i=0; i<(U8)GNOS_PSM_MAX; i++)  {
    if (gstrGnosService[i].u16Period != (U16)0) {
      if ((U64)(gullGnosCounter % (U64)gstrGnosService[i].u16Period) == (U64)0) {
        if (gstrGnosService[i].pfService != NULL) {
          gstrGnosService[i].pfService(NULL);
        }
      }
    }
  }
}

PUBLIC BOOL bGnosRegister(tstrGnosRegisterArgs* pstrArgs)
{
  U8 i;
  BOOL bReturn = FALSE;

  for (i=0; i<(U8)GNOS_PSM_MAX; i++)  {
    if (gstrGnosService[i].u16Period == (U16)0) {
      gstrGnosService[i].u16Period = pstrArgs->u16Period;
      gstrGnosService[i].pfInitialize = pstrArgs->pfGnosInitialize;
      gstrGnosService[i].pfService = pstrArgs->pfGnosService;
      if (gstrGnosService[i].pfInitialize != NULL) {
        gstrGnosService[i].pfInitialize(NULL);
      }
      bReturn = TRUE;
      break;
    }
  }
  return bReturn;
}
