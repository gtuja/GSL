/**
 * @file    gsl_queue.c
 * @brief   This file implements GSL queue features. 
 * @author  Gtuja
 * @date    Oct 27, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_queue.h"
#include "gsl_diag.h"
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
 * @brief Buffer for each of queue types, i.e., tenuQueType.
 */
PRIVATE char  gpcQueTraceBuf[QUE_ITM_MAX][QUE_DIAG_TRACE_LEN] = {0};
PRIVATE tstrDiagKeepAlive gpstrQueDiagKeepAliveBuf[QUE_ITM_MAX] = {0};
PRIVATE tstrDiagTracePsmState gpstrQueTracePsmStt[QUE_ITM_MAX] = {0};

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
  gBOOL bIsEnqueued = gFALSE;

  switch (enuType) {
  case QUE_DIAG_TRACE :
    if (bQueIsFull(enuType) != gTRUE) {
      strncpy(gpcQueTraceBuf[gps32QueTail[(U32)enuType]],(char*)pvItem, QUE_DIAG_TRACE_LEN);
      bIsEnqueued = gTRUE;
    }
    break;
  case QUE_DIAG_TRACE_PSM_STATE :
    if (bQueIsFull(enuType) != gTRUE) {
      memcpy(&gpstrQueTracePsmStt[gps32QueTail[(U32)enuType]], (tstrDiagTracePsmState*)pvItem, sizeof(tstrDiagTracePsmState));
      bIsEnqueued = gTRUE;
    }
    break;
  case QUE_DIAG_KEEP_ALIVE :
    if (bQueIsFull(enuType) != gTRUE) {
      memcpy(&gpstrQueDiagKeepAliveBuf[gps32QueTail[(U32)enuType]], (tstrDiagKeepAlive*)pvItem, sizeof(tstrDiagKeepAlive));
      bIsEnqueued = gTRUE;
    }
    break;
  default :
    break;
  }

  /* Manipulate the tail of each queues after enqueued. */
  if (bIsEnqueued == gTRUE) {
    gps32QueTail[(U32)enuType]++;
    if (gps32QueTail[(U32)enuType] == QUE_ITM_MAX) {
      gps32QueTail[(U32)enuType] = (S32)0;
    }
  }
}

/**
 * @brief   A public function that dequeues an item from queue.
 * @param   enuType The type for each of queues.
 * @return  void*   An item dequeued.
 */
PUBLIC void* pvQueDequeue(tenuQueType enuType) {
  void* pvReturn = gNULL;
  gBOOL bIsDequeued = gFALSE;

  switch (enuType) {
  case QUE_DIAG_TRACE :
    if (bQueIsEmpty(enuType) != gTRUE) {
      pvReturn = (void*)(gpcQueTraceBuf[gps32QueHead[(U32)enuType]]);
      bIsDequeued = gTRUE;
    }
    break;
  case QUE_DIAG_TRACE_PSM_STATE :
    if (bQueIsEmpty(enuType) != gTRUE) {
      pvReturn = (void*)(&gpstrQueTracePsmStt[gps32QueHead[(U32)enuType]]);
      bIsDequeued = gTRUE;
    }
    break;
  case QUE_DIAG_KEEP_ALIVE :
    if (bQueIsEmpty(enuType) != gTRUE) {
      pvReturn = (void*)(&gpstrQueDiagKeepAliveBuf[gps32QueHead[(U32)enuType]]);
      bIsDequeued = gTRUE;
    }
    break;
  default :
    break;
  }

  /* Manipulate the head of each queues after dequeued. */
  if (bIsDequeued == gTRUE) {
    gps32QueHead[(U32)enuType]++;
    if (gps32QueHead[(U32)enuType] == QUE_ITM_MAX) {
      gps32QueHead[(U32)enuType] = (S32)0;
    }
  }  
  return pvReturn;
}
