/**
 * @file    gsl_psm.c
 * @brief   This file is used to implement PSM manages periodic services.
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_psm.h"
#include "gsl_dsm.h"
#include "gsl_bsm.h"
#include "gsl_lsm.h"

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
/* Private function prototypes ------------------------------------- */
/* Private variables ----------------------------------------------- */
/**
 * @brief gu32PsmClkCnt is a private variable accumulating process count.
 */
PRIVATE U32 gu32PsmClkCnt;

/**
 * @brief gcpstrPsmCfgTbl is a private table holding PSM services.
 * @sa    tstrPsmCfg
 * @sa    tenuPsmType
 */
PRIVATE const tstrPsmCfg gcpstrPsmCfgTbl[PSM_TYPE_MAX] = {
  /* u32Period    tpfPsmInit  pfPsmService  */
  {  PSM_PRD_DSM, vidDsmInit, vidDsmSrvc  },  /* PSM_TYPE_DSM */
  {  PSM_PRD_BSM, vidBsmInit, vidBsmSrvc  },  /* PSM_TYPE_BSM */
  {  PSM_PRD_LSM, vidLsmInit, vidLsmSrvc  },  /* PSM_TYPE_LSM */
};

/**
 * @brief gcpcPsmSrvcNameTbl is a string table holding state names for notification.
 * @sa    tenuPsmType
 */
PRIVATE const char* gcpcPsmSrvcNameTbl[PSM_TYPE_MAX] = {
  "DSM",
  "BSM",
  "LSM",
};

/** 
 * @brief gpstrPsmDiag is a private table holding diagnostic information of PSM.
 * @sa    tstrPsmDiag
 */
PRIVATE tstrPsmDiag gstrPsmDiag = {0};

/**
 * @brief gcpcPsmName is the name of PSM.
 */
PRIVATE const char* gcpcPsmName = "PSM";

/* Public functions ------------------------------------------------- */
/**
 * @brief   A public function that initialize PSM.
 * @param   pvArgs  arguments reserved.
 * @sa      vidGslInitCallback
 * @return  void
 */
PUBLIC void vidPsmInit(void* pvArgs) {
  U32 i;

  gu32PsmClkCnt = (U32)0;
  gstrPsmDiag.pcName = gcpcPsmName;
  for (i=0; i<(U32)PSM_TYPE_MAX; i++) {
    if (gcpstrPsmCfgTbl[i].u32Period != (U32)0) {
      if (gcpstrPsmCfgTbl[i].pfPsmInit != gNULL) {
        gcpstrPsmCfgTbl[i].pfPsmInit(gNULL);
        gstrPsmDiag.strDiag[(U32)i].bIsRegistered = gTRUE;
        gstrPsmDiag.strDiag[(U32)i].pcSrvName = gcpcPsmSrvcNameTbl[(U32)i];
      }
    }
  }
}

/**
 * @brief   A public function that do services on PSM.
 * @param   pvArgs  arguments reserved.
 * @sa      vidGslSrvcCallback
 * @return  void
 */
PUBLIC void vidPsmSrvc(void* pvArgs) {
  U32 i;

  
  gu32PsmClkCnt++;
  for (i=0; i<(U32)PSM_TYPE_MAX; i++)  {
    if (gcpstrPsmCfgTbl[i].u32Period != gNULL) {
      if ((gu32PsmClkCnt % gcpstrPsmCfgTbl[i].u32Period) == (U32)0) {
        if (gcpstrPsmCfgTbl[i].pfPsmSrvc != gNULL) {
          gcpstrPsmCfgTbl[i].pfPsmSrvc(gNULL);
        }
      }
    }
  }
}

/**
 * @brief   A public function getting PSM diagnostic information.
 * @param   pvArgs  arguments reserved.
 * @return  tstrPsmDiag*
 */
PUBLIC tstrPsmDiag* pstrPsmGetDiag(void* pvArgs) {
  return &gstrPsmDiag;
}

/**
 * @brief   A public function to trace PSM features.
 * @param   pvArgs  arguments reserved.
 * @return  void
 */
PUBLIC void vidDiagPsm(void* pvArgs) {
#if 0
  CH pchTrace[GSL_QUE_TRACE_LEN];
  tstrPsmDiag* pstrPsmDiag;
  U32 i;

  pstrPsmDiag = pstrPsmGetDiag(gNULL);

  for (i=0; i<PSM_TYPE_MAX; i++) {
    if (pstrPsmDiag->strDiag[i].bIsRegistered == gTRUE) {
      snprintf(pchTrace, GSL_QUE_TRACE_LEN, "%s:> [%s] ------------------", pstrPsmDiag->pcName, pstrPsmDiag->strDiag[i].pcSrvName);
      vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
    }
  }
#endif
}

