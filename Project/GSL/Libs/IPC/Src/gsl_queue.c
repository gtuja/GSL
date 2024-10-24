/**
 * @file    gsl_queue.c
 * @brief   This file implements GSL queue features. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_queue.h"
#include <stdio.h>
#include <string.h>

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
/* Private functions ----------------------------------------------- */
/* Private variables ----------------------------------------------- */

/**
 * @brief gps32QueHead is a private variable holding head pointer for each of queues.
 */
PRIVATE S32   gps32QueHead[QUE_MAX] = {0};

/**
 * @brief gps32QueTail is a private variable holding tail pointer for each of queues.
 */
PRIVATE S32   gps32QueTail[QUE_MAX] = {0};

/**
 * @brief gpcQueTraceBuf is a private buffer holding items of QUE_TRACE.
 */
PRIVATE char  gpcQueTraceBuf[QUE_ITM_MAX][QUE_TRACE_LEN] = {0};
PRIVATE U64   gu64QueKeepAliveBuf[QUE_ITM_MAX] = {0};

/* Public functions ------------------------------------------------ */
/**
 * @brief   A public function that initialize GSL Queue.
 * @param   pvArgs  arguments reserved.
 * @sa      vidGslInitCallback
 * @return  void
 */
PUBLIC void vidQueInit(void* pvArgs) {
  U32 i;
  
  for (i=0; i<(U32)QUE_MAX; i++)  {
    gps32QueHead[i] = (S32)0;
    gps32QueTail[i] = (S32)0;
  }
}

/**
 * @brief   A public function that returns gTRUE when the GSL Queue is empty.
 * @param   enuType The type for each of queues.
 * @return  gBOOL
 */
PUBLIC gBOOL bQueIsEmpty(tenuQueType enuType) {
  return (gps32QueHead[(U32)enuType] == gps32QueTail[(U32)enuType]) ? gTRUE : gFALSE;
}

/**
 * @brief   A public function that returns gTRUE when the GSL Queue is full.
 * @param   enuType The type for each of queues.
 * @return  gBOOL
 */
PUBLIC gBOOL bQueIsFull(tenuQueType enuType) {
  return (gps32QueHead[(U32)enuType] == (gps32QueTail[(U32)enuType] + 1) % QUE_ITM_MAX) ? gTRUE : gFALSE;
}

/**
 * @brief   A public function that enqueues an item to queue.
 * @param   enuType The type for each of queues.
 * @param   pvItem  An item about to enqueued.
 * @return  void
 */
PUBLIC void vidQueEnqueue(tenuQueType enuType, void* pvItem) {
  switch (enuType) {
    case QUE_TRACE :
      if (bQueIsFull(enuType) != gTRUE) {
        strncpy(gpcQueTraceBuf[gps32QueTail[(U32)enuType]],(char*)pvItem, QUE_TRACE_LEN);
        gps32QueTail[(U32)enuType]++;
        if (gps32QueTail[(U32)enuType] == QUE_ITM_MAX) {
          gps32QueTail[(U32)enuType] = (S32)0;
        }
      }
      break;
    case QUE_DIAG_KEEP_ALIVE :
      if (bQueIsFull(enuType) != gTRUE) {
        gu64QueKeepAliveBuf[gps32QueTail[(U32)enuType]] = *((U64*)pvItem);
        gps32QueTail[(U32)enuType]++;
        if (gps32QueTail[(U32)enuType] == QUE_ITM_MAX) {
          gps32QueTail[(U32)enuType] = (S32)0;
        }
      }
      break;
    default :
      break;
  }
}

/**
 * @brief   A public function that dequeues an item from queue.
 * @param   enuType The type for each of queues.
 * @return  void*   An item dequeued.
 */
PUBLIC void* pvQueDequeue(tenuQueType enuType) {
  void* pvReturn = gNULL;

  switch (enuType) {
    case QUE_TRACE :
      if (bQueIsEmpty(enuType) != gTRUE) {
        pvReturn = (void*)(gpcQueTraceBuf[gps32QueHead[(U32)enuType]]);
        gps32QueHead[(U32)enuType]++;
        if (gps32QueHead[(U32)enuType] == QUE_ITM_MAX) {
          gps32QueHead[(U32)enuType] = (S32)0;
        }
      }
    case QUE_DIAG_KEEP_ALIVE :
      if (bQueIsEmpty(enuType) != gTRUE) {
        pvReturn = (void*)(&gu64QueKeepAliveBuf[gps32QueHead[(U32)enuType]]);
        gps32QueHead[(U32)enuType]++;
        if (gps32QueHead[(U32)enuType] == QUE_ITM_MAX) {
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
