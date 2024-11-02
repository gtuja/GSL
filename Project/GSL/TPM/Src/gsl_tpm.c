/**
 * @file    gsl_bpm.c
 * @brief   This file is used to implement BPM manages background processes.
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include <gsl_tpm.h>
#include "gsl_psm.h"
#include "gsl_bsm.h"
#include "gsl_lsm.h"
#include "gsl_diag.h"
#include "gsl_queue.h"
#include <stdio.h>

/* External variables ----------------------------------------------- */
/* Private define --------------------------------------------------- */
#define BPM_XSM_NAME(index)      gpcXsmNameTbl[index]
#define BPM_BSM_STT_NAME(index)  gpcBsmSttNameTbl[index]
#define BPM_BSM_EVT_NAME(index)  gpcBsmEvtNameTbl[index]
#define BPM_BSM_NTF_NAME(index)  gpcBsmNtfNameTbl[index]

#define BPM_LSM_STT_NAME(index)  gpcLsmSttNameTbl[index]
#define BPM_LSM_EVT_NAME(index)  gpcLsmEvtNameTbl[index]

/* Private typedef -------------------------------------------------- */
/* Private function prototypes -------------------------------------- */
PRIVATE void vidBpmProcIdle(void* pvArgs);
PRIVATE void vidBpmProcDiag(void* pvArgs);
PRIVATE void vidBpmProcDiagTrace(CH* pcTrace);
PRIVATE void vidBpmProcDiagTracePsmState(tstrDiagTracePsmState* tstrDiagTracePsmState);
PRIVATE void vidBpmProcDiagKeepAlive(tstrDiagKeepAlive* pstrKeepAlive);

/* Private variables ------------------------------------------------ */
PRIVATE const char* gpcPsmNameTbl[BSM_STT_MAX] = {
  "BSM",      /**< XSM type, BSM. */
  "LSM",      /**< XSM type, LSM. */
};

PRIVATE const char* gpcBsmSttNameTbl[BSM_STT_MAX] = {
  "NA",       /**< BSM state, not available. */
  "RLS",      /**< BSM state, button is released. */
  "PSH_CFM",  /**< BSM state, button is pushed, but under confirmation. */
  "PSH",      /**< BSM state, button is pushed. */
  "RLS_CFM",  /**< BSM state, button is pushed, but under confirmation. */
};

PRIVATE const char* gpcBsmEvtNameTbl[BSM_EVT_MAX] = {
  "SPONT",    /**< BSM spontaneous event. */
  "RLS",      /**< BSM event that button is released. */
  "PSH",      /**< BSM event that button is pushed. */
};

#if 0 /* TBD */
PRIVATE const char* gpcBsmNtfNameTbl[BSM_NTF_MAX] = {
  "BSM_NTF_NA",       /**< BSM event notified not available. */
  "BSM_NTF_SHORT",    /**< BSM event notified that is short pressed. */
  "BSM_NTF_LONG",     /**< BSM event notified that is long pressed. */
};
#endif

PRIVATE const char* gpcLsmSttNameTbl[LSM_STT_MAX] = {
  "NA",       /**< LSM state, not available. */
  "OFF",      /**< LSM state, LED is off. */
  "FADE_IN",  /**< LSM state, LED is under fade in. */
  "ON",       /**< LSM state, LED is on. */
  "FADE_OFF", /**< LSM state, LED is under fade off. */
};

PRIVATE const char* gpcLsmEvtNameTbl[LSM_EVT_MAX] = {
  "SPONT",    /**< LSM spontaneous event. */
  "ON",       /**< LSM event that requests ON. */
  "OFF",      /**< LSM event that requests OFF. */
  "FRC_ON",   /**< LSM event that requests FORCE ON. */
  "FRC_OFF",  /**< LSM event that requests FORCE OFF. */
};

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
  if (bQueIsEmpty(QUE_DIAG_TRACE) != gTRUE) {
    vidBpmProcDiagTrace((char*)pvQueDequeue(QUE_DIAG_TRACE));
  }

  if (bQueIsEmpty(QUE_DIAG_TRACE_PSM_STATE) != gTRUE) {
    vidBpmProcDiagTracePsmState((tstrDiagTracePsmState*)pvQueDequeue(QUE_DIAG_TRACE_PSM_STATE));
  }

  if (bQueIsEmpty(QUE_DIAG_KEEP_ALIVE) != gTRUE) {
    vidBpmProcDiagKeepAlive((tstrDiagKeepAlive*)pvQueDequeue(QUE_DIAG_KEEP_ALIVE));
  }
}

/**
 * @brief   A private function that do trace GSL.
 * @param   pcTrace The string for trace GSL
 * @return  void
 */
PRIVATE void vidBpmProcDiagTrace(CH* pcTrace) {
  vidDiagTraceCallback(pcTrace);
}

/**
 * @brief   A private function that do trace XSM state transition.
 * @param   tstrDiagTraceXsmState The dequeued state transition information of the XSM.
 * @return  void
 */
PRIVATE void vidBpmProcDiagTracePsmState(tstrDiagTracePsmState* pstrDiagTracePsmState) {
  CH pcTrace[QUE_DIAG_TRACE_LEN];
  
  switch (pstrDiagTracePsmState->enuType) {
  case PSM_TYPE_BSM :
    snprintf(pcTrace, QUE_DIAG_TRACE_LEN, \
            "[%s](%s)[%s]->[%s] by [%s]", \
            gpcPsmNameTbl[(U32)pstrDiagTracePsmState->enuType], \
            pstrDiagTracePsmState->pcName, \
            gpcBsmSttNameTbl[pstrDiagTracePsmState->u32SttPrevious], \
            gpcBsmSttNameTbl[pstrDiagTracePsmState->u32SttCurrent], \
            gpcBsmEvtNameTbl[pstrDiagTracePsmState->u32Event]
    );
    vidDiagTraceCallback(pcTrace);
    break;
  case PSM_TYPE_LSM :
    snprintf(pcTrace, QUE_DIAG_TRACE_LEN, \
            "[%s](%s)> [%s] -> [%s] by Event [%s]", \
            gpcPsmNameTbl[(U32)pstrDiagTracePsmState->enuType], \
            pstrDiagTracePsmState->pcName, \
            gpcLsmSttNameTbl[pstrDiagTracePsmState->u32SttPrevious], \
            gpcLsmSttNameTbl[pstrDiagTracePsmState->u32SttCurrent], \
            gpcLsmEvtNameTbl[pstrDiagTracePsmState->u32Event]
    );
    vidDiagTraceCallback(pcTrace);
    break;
  default :
    break;
  }
}

/**
 * @brief   A private function that do keep alive diagnostic process.
 * @param   pstrKeepAlive The dequeued keep alive information of the PSM.
 * @return  void
 */
PRIVATE void vidBpmProcDiagKeepAlive(tstrDiagKeepAlive* pstrKeepAlive) {
  CH pcTrace[QUE_DIAG_TRACE_LEN];
  
  U32 u32hourTotal;
  U32 u32minTotal;
  U32 u32secTotal;
  U32 u32msTotal;

  u32msTotal    = (pstrKeepAlive->u32TmsElapsed  % (U32)1000);
  u32secTotal   = (pstrKeepAlive->u32TmsElapsed  / (U32)1000)     % (U32)60;
  u32minTotal   = (pstrKeepAlive->u32TmsElapsed  / (U32)60000)    % (U32)60;
  u32hourTotal  = (pstrKeepAlive->u32TmsElapsed  / (U32)3600000)  % (U32)24;

  snprintf(pcTrace, QUE_DIAG_TRACE_LEN, \
          "Keep alive... ET [%02ld:%02ld:%02ld.%03ld] OTM [%03ld us]", \
          u32hourTotal, u32minTotal, u32secTotal, u32msTotal, pstrKeepAlive->u32TusOtMax);
  vidDiagTraceCallback(pcTrace);
}
