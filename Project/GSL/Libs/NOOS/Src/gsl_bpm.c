/**
 * @file    gsl_bpm.c
 * @brief   This file is used to implement BPM manages background processes.
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_bpm.h"
#include "gsl_diag.h"
#include "gsl_queue.h"
#include <stdio.h>

/* External variables ----------------------------------------------- */
/* Private define --------------------------------------------------- */
/* Private typedef -------------------------------------------------- */
/* Private function prototypes -------------------------------------- */
PRIVATE void vidBpmProcIdle(void* pvArgs);
PRIVATE void vidBpmProcDiag(void* pvArgs);
PRIVATE void vidBpmProcDiagKeepAlive(tstrDiagKeepAlive* pstrKeepAlive);

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
  
  if (bQueIsEmpty(QUE_DIAG_KEEP_ALIVE) != gTRUE) {
    vidBpmProcDiagKeepAlive((tstrDiagKeepAlive*)pvQueDequeue(QUE_DIAG_KEEP_ALIVE));
  }
  
  if (bQueIsEmpty(QUE_TRACE) != gTRUE) {
    vidDiagTraceCallback((char*)pvQueDequeue(QUE_TRACE));
  }
}

/**
 * @brief   A private function that do keep alive diagnostic process.
 * @param   pvArgs        arguments reserved.
 * @sa      u32TusElapsed us unit elapsed time.
 * @return  void
 */
PRIVATE void vidBpmProcDiagKeepAlive(tstrDiagKeepAlive* pstrKeepAlive) {
  CH pchTrace[QUE_KEEP_ALIVE_LEN];
  
  U32 u32hour;
  U32 u32min;
  U32 u32sec;
  U32 u32ms;
  U32 u32us;
  U32 u32hourTotal;
  U32 u32minTotal;
  U32 u32secTotal;
  U32 u32msTotal;
  U32 u32usTotal;

  u32usTotal    = (U32)(pstrKeepAlive->u32TusElapsedTotal % 1000);
  u32msTotal    = (U32)((U32)(pstrKeepAlive->u32TusElapsedTotal  /       1000) % 1000);
  u32secTotal   = (U32)((U32)(pstrKeepAlive->u32TusElapsedTotal  /    1000000) % 60);
  u32minTotal   = (U32)((U32)(pstrKeepAlive->u32TusElapsedTotal  /   60000000) % 60);
  u32hourTotal  = (U32)((U32)(pstrKeepAlive->u32TusElapsedTotal  / 3600000000) % 24);

  u32us   = (U32)(pstrKeepAlive->u32TusElapsed % 1000);
  u32ms   = (U32)((U32)(pstrKeepAlive->u32TusElapsed /       1000) % 1000);
  u32sec  = (U32)((U32)(pstrKeepAlive->u32TusElapsed /    1000000) % 60);
  u32min  = (U32)((U32)(pstrKeepAlive->u32TusElapsed /   60000000) % 60);
  u32hour = (U32)((U32)(pstrKeepAlive->u32TusElapsed / 3600000000) % 24);

  snprintf(pchTrace, QUE_KEEP_ALIVE_LEN, \
          "Keep alive... ET[%02ld:%02ld:%02ld.%03ld%03ld] PS[%02ld:%02ld:%02ld.%03ld%03ld] OTM[%02ld.%02ld%%]", \
          u32hourTotal, u32minTotal, u32secTotal, u32msTotal, u32usTotal, \
          u32hour, u32min, u32sec, u32ms, u32us, \
          (pstrKeepAlive->u32TusElapsedMax / 10), (pstrKeepAlive->u32TusElapsedMax % 10));
  vidDiagTraceCallback(pchTrace);
}
