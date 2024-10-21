/**
 * @file    gsl_queue.c
 * @brief   This file implements GSL queue features. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_queue.h"
#include <string.h>

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
/* Private functions ----------------------------------------------- */
/* Private variables ----------------------------------------------- */

/**
 * @brief gps32QueHead is a private variable holding head pointer for each of queues.
 */
PRIVATE S32   gps32QueHead[GSL_QUE_MAX] = {0};

/**
 * @brief gps32QueTail is a private variable holding tail pointer for each of queues.
 */
PRIVATE S32   gps32QueTail[GSL_QUE_MAX] = {0};

/**
 * @brief gpcQueTraceBuf is a private buffer holding items of GSL_QUE_TRACE.
 */
PRIVATE char  gpcQueTraceBuf[GSL_QUE_LEN][GSL_QUE_TRACE_LEN] = {0};
PRIVATE U64   gu64QueKeepAliveBuf[GSL_QUE_LEN] = {0};

/* Public functions ------------------------------------------------ */
/**
 * @brief   A public function that initialize GSL Queue.
 * @param   pvArgs  arguments reserved.
 * @sa      vidGslInitCallback
 * @return  void
 */
PUBLIC void vidGslQueInit(void* pvArgs) {
  U32 i;
  
  for (i=0; i<(U32)GSL_QUE_MAX; i++)  {
    gps32QueHead[i] = (S32)0;
    gps32QueTail[i] = (S32)0;
  }
}

/**
 * @brief   A public function that returns gTRUE when the GSL Queue is empty.
 * @param   enuType The type for each of queues.
 * @return  gBOOL
 */
PUBLIC gBOOL bGslQueIsEmpty(tenuGslQueType enuType) {
  return (gps32QueHead[(U32)enuType] == gps32QueTail[(U32)enuType]) ? gTRUE : gFALSE;
}

/**
 * @brief   A public function that returns gTRUE when the GSL Queue is full.
 * @param   enuType The type for each of queues.
 * @return  gBOOL
 */
PUBLIC gBOOL bGslQueIsFull(tenuGslQueType enuType) {
  return (gps32QueHead[(U32)enuType] == (gps32QueTail[(U32)enuType] + 1) % GSL_QUE_LEN) ? gTRUE : gFALSE;
}

/**
 * @brief   A public function that enqueues an item to queue.
 * @param   enuType The type for each of queues.
 * @param   pvItem  An item about to enqueued.
 * @return  void
 */
PUBLIC void vidGslQueEnqueue(tenuGslQueType enuType, void* pvItem) {
  switch (enuType) {
    case GSL_QUE_TRACE :
      if (bGslQueIsFull(enuType) != gTRUE) {
        strncpy(gpcQueTraceBuf[gps32QueTail[(U32)enuType]],(char*)pvItem, GSL_QUE_TRACE_LEN);
        gps32QueTail[(U32)enuType]++;
        if (gps32QueTail[(U32)enuType] == GSL_QUE_LEN) {
          gps32QueTail[(U32)enuType] = (S32)0;
        }
      }
      break;
    case GSL_QUE_KEEP_ALIVE :
      if (bGslQueIsFull(enuType) != gTRUE) {
        gu64QueKeepAliveBuf[gps32QueTail[(U32)enuType]] = *((U64*)pvItem);
        gps32QueTail[(U32)enuType]++;
        if (gps32QueTail[(U32)enuType] == GSL_QUE_LEN) {
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
PUBLIC void* pvGslQueDequeue(tenuGslQueType enuType) {
  void* pvReturn = gNULL;

  switch (enuType) {
    case GSL_QUE_TRACE :
      if (bGslQueIsEmpty(enuType) != gTRUE) {
        pvReturn = (void*)(gpcQueTraceBuf[gps32QueHead[(U32)enuType]]);
        gps32QueHead[(U32)enuType]++;
        if (gps32QueHead[(U32)enuType] == GSL_QUE_LEN) {
          gps32QueHead[(U32)enuType] = (S32)0;
        }
      }
    case GSL_QUE_KEEP_ALIVE :
      if (bGslQueIsEmpty(enuType) != gTRUE) {
        pvReturn = (void*)(&gu64QueKeepAliveBuf[gps32QueHead[(U32)enuType]]);
        gps32QueHead[(U32)enuType]++;
        if (gps32QueHead[(U32)enuType] == GSL_QUE_LEN) {
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
