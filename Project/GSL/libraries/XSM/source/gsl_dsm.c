/**
 * @file    gsl_dsm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 16, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_def.h"
#include "gsl_feature.h"
#include "gsl_config.h"
#include "gsl_bsm.h"
#include "gsl.h"
#include "gsl_queue.h"
#include <stdio.h>

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
#define DSM_PRD_KEEP_ALIVE  (U32)5000
#define DSM_PRD_XSM_STATE   (U32)10000

/* Private typedef ------------------------------------------------- */
typedef enum {
  DSM_TYPE_KEEP_ALIVE = 0,  /**< Diagnostic service type : Keep Alive. */
  DSM_TYPE_XSM_STT,         /**< Diagnostic service type : XSM state */
  DSM_TYPE_MAX,
} tenuDsmType;

typedef void (*tpfDsmSrvc)(void* pvArgs);

/* Private functions ----------------------------------------------- */
PRIVATE void vidDsmSrvKeepAlive(void* pvArgs);
PRIVATE void vidDsmSrvXsmState(void* pvArgs);

/* Private variables ----------------------------------------------- */
typedef struct {
  U32         u32Period;  /**< The cycle of execution. */
  tpfDsmSrvc  pfDsmSrvc;  /**< DSM service. */
} tstrDsmCfg;

PRIVATE const tstrDsmCfg gcpstrDsmCfgTbl[DSM_TYPE_MAX] = {
  /* u32Period            tpfDsmSrvc */
  {  DSM_PRD_KEEP_ALIVE,  vidDsmSrvKeepAlive  },  /* DSM_TYPE_KEEP_ALIVE */
  {  DSM_PRD_XSM_STATE,   vidDsmSrvXsmState  },   /* DSM_TYPE_XSM_STT */
};

PRIVATE U32   gu32DsmCnt = (U32)0;
PRIVATE U32   gu32DsmTusPast = (U32)0;
PRIVATE gBOOL gbIsDsmTusMeas = gFALSE;
PRIVATE U64   gu4DsmTusTotal = (U64)0;
PRIVATE U32   gu32DsmTusCntPrd = (U32)0;

/* A public functions ------------------------------------------------ */
/**
 * @brief   A public function that process BSM called by PSM.
 * @param   void
 * @sa      vidGslPsmService
 * @return  void
 */
PUBLIC void vidDsmInit(void* pvArgs) {
  gu32DsmCnt = (U32)0;
  gu32DsmTusPast = (U32)0;
  gbIsDsmTusMeas = gFALSE;
  gu4DsmTusTotal = (U64)0;
  gu32DsmTusCntPrd = u32DsmCntPrdCallback(gNULL);
}

/**
 * @brief   A public function that process BSM called by PSM.
 * @param   void
 * @sa      vidGslPsmService
 * @return  void
 */
PUBLIC void vidDsmSrvc(void* pvArgs) {
  U32 i;
  gu32DsmCnt++;

  for (i=0; i<(U32)DSM_TYPE_MAX; i++) {
    if (gcpstrDsmCfgTbl[(U32)i].u32Period != (U32)0) {
      if (gu32DsmCnt % gcpstrDsmCfgTbl[(U32)i].u32Period == (U32)0) {
        if (gcpstrDsmCfgTbl[(U32)i].pfDsmSrvc != gNULL) {
          gcpstrDsmCfgTbl[(U32)i].pfDsmSrvc(gNULL);
        }
      }
    }
  }
}

/**
 * @brief   A public function to setup us unit timer elapsed time calculation.
 * @param   pvArgs  arguments reserved.
 * @sa      gbIsDsmTusMeas
 * @sa      gu32DsmTusPast
 * @sa      u32DsmCntCallback
 * @return  void
 */
PUBLIC void vidDsmTusMeasStart(void* pvArgs) {
  gu32DsmTusPast = u32DsmCntCallback(gNULL);
  gbIsDsmTusMeas = gTRUE;
}

/**
 * @brief   A public function for getting us unit timer elapsed time.
 * @param   pvArgs  arguments reserved.
 * @sa      gbIsDsmTusMeas
 * @sa      gu32DsmTusPast
 * @sa      u32DsmCntCallback
 * @return  U32     us unit timer elapsed time.
 */
PUBLIC U32 u32DsmTusMeasElapsed(void* pvArgs) {
  U32 u32TusElapsed;
  U32 u32TusCur;

  u32TusElapsed = (U32)0;
  if (gbIsDsmTusMeas == gTRUE) {
    u32TusCur = u32DsmCntCallback(gNULL);
    if (u32TusCur >= gu32DsmTusPast) {
      u32TusElapsed = u32TusCur - gu32DsmTusPast;
    } else {
      u32TusElapsed = u32TusCur + gu32DsmTusCntPrd - gu32DsmTusPast;
    }
  }
  return u32TusElapsed;
}

/**
 * @brief   A public function to accumulate us unit timer period count.
 * @param   pvArgs  arguments reserved.
 * @return  void
 */
PUBLIC void vidDsmTusAccumulate(void *pvArgs) {
  gu4DsmTusTotal += (U64)gu32DsmTusCntPrd;
}

/**
 * @brief   A public function for getting us unit timer period count accumulated.
 * @param   pvArgs  arguments reserved.
 * @return  U64     us unit timer period count accumulated.
 */
PUBLIC U64 u64DsmGetTusTotal(void *pvArgs) {
  return gu4DsmTusTotal;
}

/**
 * @brief   A public weak function for tracing GSL features.
 * @param   char*   Trace string.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  void
 */
PRIVATE void vidDsmSrvKeepAlive(void* pvArgs) {
  CH pchTrace[GSL_QUE_TRACE_LEN];

  snprintf(pchTrace, GSL_QUE_TRACE_LEN, "keep alive...");
  vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
}

PRIVATE void vidDsmSrvXsmState(void* pvArgs) {
  CH pchTrace[GSL_QUE_TRACE_LEN];
  tstrBsmDiag* pstrBsmDiag;
  U32 i, j;

  pstrBsmDiag = pstrBsmGetDiag(NULL);

  for (i=0; i<BSM_TYPE_MAX; i++) {
    if (pstrBsmDiag->strDiag[i].bIsRegistered == gTRUE) {
      snprintf(pchTrace, GSL_QUE_TRACE_LEN, "## %s[%s] ------------------", pstrBsmDiag->pcName, pstrBsmDiag->strDiag[i].pcSrvName);
      vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
      for (j=0; j<BSM_STT_MAX; j++) {
        snprintf(pchTrace, GSL_QUE_TRACE_LEN, "## %s[%08ld]", pstrBsmDiag->strDiag[i].pcSttName[j], pstrBsmDiag->strDiag[i].pu32SttCnt[j]);
        vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
      }
    }
  }
}

/**
 * @brief   A public weak function for retrieving count of us unit timer.
 * @param   pvArgs  arguments reserved.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  U32     Count of us unit timer.
 */
PUBLIC __attribute__((weak)) U32 u32DsmCntCallback(void* pvArgs) {
  return (U32)0;
}

/**
 * @brief   A public weak function for retrieving counter period of us unit timer.
 * @param   pvArgs  arguments reserved.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  U32     Counter period of us unit timer.
 */
PUBLIC __attribute__((weak)) U32 u32DsmCntPrdCallback(void* pvArgs) {
  return (U32)0;
}

/**
 * @brief   A public weak function for tracing GSL features.
 * @param   char*   Trace string.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  void
 */
PUBLIC __attribute__((weak)) void vidDsmTraceCallback(char* pcTrace) {
}

