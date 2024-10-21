/**
 * @file    gsl_bpm.c
 * @brief   This file is used to implement BPM manages background processes.
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_bpm.h"
#include "gsl_queue.h"
#include <stdio.h>

/* External variables ----------------------------------------------- */
/* Private define --------------------------------------------------- */
/* Private typedef -------------------------------------------------- */
/* Private function prototypes -------------------------------------- */
PRIVATE void vidBpmProcIdle(void* pvArgs);
PRIVATE void vidBpmProcDiag(void* pvArgs);
PRIVATE void vidBpmProcDiagKeepAlive(U64 u64TusElapsed);

/* Private variables ------------------------------------------------ */
/* Public functions ------------------------------------------------- */
/**
 * @brief   A public function that initialize BPM.
 * @param   pvArgs  arguments reserved.
 * @sa      vidGslInitCallback
 * @return  void
 */
PUBLIC void vidBpmInit(void* pvArgs) {
}

/**
 * @brief   A public function that do BPM processes.
 * @param   pvArgs  arguments reserved.
 * @sa      vidGslProcCallback
 * @return  void
 */
PUBLIC void vidBpmProc(void* pvArgs) {
  vidBpmProcIdle(gNULL);
  vidBpmProcDiag(gNULL);
}

/**
 * @brief   A public function that do idle processes.
 * @param   pvArgs  arguments reserved.
 * @sa      vidBpmProc
 * @return  void
 */
PRIVATE void vidBpmProcIdle(void* pvArgs) {
}

/**
 * @brief   A public function that do diagnostic processes.
 * @param   pvArgs  arguments reserved.
 * @sa      vidBpmProc
 * @return  void
 */
PRIVATE void vidBpmProcDiag(void* pvArgs) {
  
  if (bGslQueIsEmpty(GSL_QUE_KEEP_ALIVE) != gTRUE) {
    vidBpmProcDiagKeepAlive(*((U64*)pvGslQueDequeue(GSL_QUE_KEEP_ALIVE)));
  }
  
  if (bGslQueIsEmpty(GSL_QUE_TRACE) != gTRUE) {
    vidDiagTraceCallback((char*)pvGslQueDequeue(GSL_QUE_TRACE));
  }
}

/**
 * @brief   A private function that do keep alive diagnostic process.
 * @param   pvArgs        arguments reserved.
 * @sa      u64TusElapsed us unit elapsed time.
 * @return  void
 */
PRIVATE void vidBpmProcDiagKeepAlive(U64 u64TusElapsed) {
  CH pchTrace[GSL_QUE_TRACE_LEN];
  U32 u32hour;
  U32 u32min;
  U32 u32sec;
  U32 u32ms;
  U32 u32us;

  u32us = (U32)(u64TusElapsed % 1000);
  u32ms = (U32)((U32)(u64TusElapsed / 1000) % 1000);
  u32sec = (U32)((U32)(u64TusElapsed / 1000000) % 60);
  u32min = (U32)((U32)(u64TusElapsed / 60000000) % 60);
  u32hour = (U32)((U32)(u64TusElapsed / 3600000000) % 24);

  snprintf(pchTrace, GSL_QUE_TRACE_LEN, \
          "keep alive...  %2ld:%2ld:%2ld.%3ld%3ld", \
          u32hour, u32min, u32sec, u32ms, u32us);
  vidDiagTraceCallback(pchTrace);
}
