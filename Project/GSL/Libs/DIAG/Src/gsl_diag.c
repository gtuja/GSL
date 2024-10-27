/**
 * @file    gsl_diag.c
 * @brief   This file implements DIAG interfaces and UA callbacks as week functions.
 * @author  Gtuja
 * @date    Oct 27, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_xsm.h"
#include "gsl_bsm.h"
#include "gsl_lsm.h"
#include "gsl_diag.h"
#include "gsl_queue.h"

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
/* Private functions ----------------------------------------------- */
/* Private variables ----------------------------------------------- */
PRIVATE U32   gu32DiagTusPast = (U32)0;
PRIVATE gBOOL gbIsDiagTusMeas = gFALSE;
PRIVATE U32   gu32DiagTusCntPrd = (U32)0;

/* Public functions ------------------------------------------------ */
/**
 * @brief   A public function to initialize diagnostic modules.
 * @param   pvArgs  arguments reserved.
 * @sa      gbIsDiagTusMeas
 * @sa      gu32DiagTusPast
 * @sa      u32DiagTusCntCallback
 * @return  void
 */
PUBLIC void vidDiagInit(void* pvArgs) {
  gu32DiagTusPast = (U32)0;
  gbIsDiagTusMeas = gFALSE;
  gu32DiagTusCntPrd = u32DiagTusCntPrdCallback(gNULL);
}

/**
 * @brief   A public function to setup us unit timer elapsed time calculation.
 * @param   pvArgs  arguments reserved.
 * @sa      gbIsDiagTusMeas
 * @sa      gu32DiagTusPast
 * @sa      u32DiagTusCntCallback
 * @return  void
 */
PUBLIC void vidDiagTusStart(void* pvArgs) {
  gu32DiagTusPast = u32DiagTusCntCallback(gNULL);
  gbIsDiagTusMeas = gTRUE;
}

/**
 * @brief   A public function for getting us unit timer elapsed time.
 * @param   pvArgs  arguments reserved.
 * @sa      gbIsDiagTusMeas
 * @sa      gu32DiagTusPast
 * @sa      u32DiagTusCntCallback
 * @return  U32     us unit timer elapsed time.
 */
PUBLIC U32 u32DiagTusElapsed(void* pvArgs) {
  U32 u32TusElapsed;
  U32 u32TusCur;

  u32TusElapsed = (U32)0;
  if (gbIsDiagTusMeas == gTRUE) {
    u32TusCur = u32DiagTusCntCallback(gNULL);
    if (u32TusCur >= gu32DiagTusPast) {
      u32TusElapsed = u32TusCur - gu32DiagTusPast;
    } else {
      u32TusElapsed = u32TusCur + gu32DiagTusCntPrd - gu32DiagTusPast;
    }
  }
  return u32TusElapsed;
}

/**
 * @brief   A public function to trace GSL.
 * @param   CH* pcTrace  String to trace GSL
 * @return  void
 */
PUBLIC void vidDiagTrace(CH* pcTrace) {
  vidQueEnqueue(QUE_DIAG_TRACE, (void*)pcTrace);
}

/**
 * @brief   A public function to trace the state of XSM.
 * @param   CH* pcTrace  String to trace GSL
 * @return  void
 */
PUBLIC void vidDiagTraceXsmState(tenuXsmType enuType, const CH* pcName, U32 u32SttPrevious, U32 u32SttCurrent, U32 u32Event) {
  tstrDiagTraceXsmState strTraceXsmState;
  strTraceXsmState.enuType = enuType;
  strTraceXsmState.pcName = pcName;
  strTraceXsmState.u32SttPrevious = u32SttPrevious;
  strTraceXsmState.u32SttCurrent = u32SttCurrent;
  strTraceXsmState.u32Event = u32Event;

  vidQueEnqueue(QUE_DIAG_TRACE_XSM_STATE, (void*)&strTraceXsmState);
}

/**
 * @brief   A public function to request keep alive.
 * @param   u32TmsCnt     The ms unit counter after powered on.
 * @param   u32TusOrtMax  The us unit counter holding maximum occupation time of PSM.
 * @return  void
 */
PUBLIC void vidDiagKeepAlive(U32 u32TmsCnt, U32 u32TusOrtMax) {
  tstrDiagKeepAlive strKeepAlive;
  strKeepAlive.u32TmsElapsed = u32TmsCnt;
  strKeepAlive.u32TusOtMax = u32TusOrtMax;
  
  vidQueEnqueue(QUE_DIAG_KEEP_ALIVE, (void*)&strKeepAlive);
}

/* Weak functions -------------------------------------------------- */
/**
 * @brief   A public weak function for retrieving count of us unit timer.
 * @param   pvArgs  arguments reserved.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  U32     Count of us unit timer.
 */
PUBLIC __attribute__((weak)) U32 u32DiagTusCntCallback(void* pvArgs) {
  return (U32)0;
}

/**
 * @brief   A public weak function for retrieving counter period of us unit timer.
 * @param   pvArgs  arguments reserved.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  U32     Counter period of us unit timer.
 */
PUBLIC __attribute__((weak)) U32 u32DiagTusCntPrdCallback(void* pvArgs) {
  return (U32)0;
}

/**
 * @brief   A public weak function for tracing GSL features.
 * @param   char*   Trace string.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  void
 */
PUBLIC __attribute__((weak)) void vidDiagTraceCallback(char* pcTrace) {
}


