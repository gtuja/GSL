/**
 * @file    gsl_queue.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 14, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_queue.h"
#include <string.h>

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
#define QUE_LEN 10
#define QUE_NA  (S32)-1
#define QUE_TRACE_ITEM_LEN 72

/* Private typedef ------------------------------------------------- */
/* Private functions ----------------------------------------------- */
/* Private variables ----------------------------------------------- */
PRIVATE S32   gps32QueHead[GSL_QUE_MAX] = {0};
PRIVATE S32   gps32QueTail[GSL_QUE_MAX] = {0};
PRIVATE char  gpcQueTraceBuf[QUE_LEN][QUE_TRACE_ITEM_LEN] = {0};

/* Public functions ------------------------------------------------ */
/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC void vidGslQueInitialize(void) {
  U32 i;

  for (i=0; i<(U32)GSL_QUE_MAX; i++)  {
    gps32QueHead[i] = (S32)0;
    gps32QueTail[i] = (S32)0;;
  }
}

PUBLIC BOOL bGslQueIsEmpty(tenuGslQueType enuType) {
  return (gps32QueHead[(U32)enuType] == gps32QueTail[(U32)enuType]) ? TRUE : FALSE;
}

PUBLIC BOOL bGslQueIsFull(tenuGslQueType enuType) {
  return (gps32QueHead[(U32)enuType] == (gps32QueTail[(U32)enuType] + 1) % QUE_LEN) ? TRUE : FALSE;
}

PUBLIC void vidGslQueEnqueue(tenuGslQueType enuType, void* pvItem) {
  switch (enuType) {
    case GSL_QUE_TRACE :
      if (bGslQueIsFull(enuType) != TRUE) {
        strncpy(gpcQueTraceBuf[gps32QueTail[(U32)enuType]],(char*)pvItem, QUE_TRACE_ITEM_LEN);
        gps32QueTail[(U32)enuType]++;
        if (gps32QueTail[(U32)enuType] == QUE_LEN) {
          gps32QueTail[(U32)enuType] = (S32)0;
        }
      }
      break;
    default :
      break;
  }
}

PUBLIC void* u32GslQueDequeue(tenuGslQueType enuType) {
  void* pvReturn = NULL;

  switch (enuType) {
    case GSL_QUE_TRACE :
      if (bGslQueIsEmpty(enuType) != TRUE) {
        pvReturn = (void*)(gpcQueTraceBuf[gps32QueHead[(U32)enuType]]);
        gps32QueHead[(U32)enuType]++;
        if (gps32QueHead[(U32)enuType] == QUE_LEN) {
          gps32QueHead[(U32)enuType] = (S32)0;
        }
      }
      break;
    default :
      break;
  }
  return pvReturn;
}

/* Private functions ----------------------------------------------- */



/**
 * @brief   A private function that initialize XXX.
 * @param   xxParam  A xxx parameter used for xxxx.
 * @sa      vidXxx
 * @return  void
 */
