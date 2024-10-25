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
#include "gsl_diag.h"

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
 * @brief gstrPsmKeepAlive is a private variable holding occupancy time of PSM.
 */
PRIVATE tstrDiagKeepAlive gstrPsmKeepAlive = {0};

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
  for (i=0; i<(U32)PSM_TYPE_MAX; i++) {
    if (gcpstrPsmCfgTbl[i].u32Period != (U32)0) {
      if (gcpstrPsmCfgTbl[i].pfPsmInit != gNULL) {
        gcpstrPsmCfgTbl[i].pfPsmInit(gNULL);
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
  U32 u32TusElapsed;

  gu32PsmClkCnt++;
  vidDiagTusStart(gNULL);

  for (i=0; i<(U32)PSM_TYPE_MAX; i++)  {
    if (gcpstrPsmCfgTbl[i].u32Period != gNULL) {
      if ((gu32PsmClkCnt % gcpstrPsmCfgTbl[i].u32Period) == (U32)0) {
        if (gcpstrPsmCfgTbl[i].pfPsmSrvc != gNULL) {
          gcpstrPsmCfgTbl[i].pfPsmSrvc(gNULL);
        }
      }
    }
  }

  gstrPsmKeepAlive.u32TusElapsedTotal = (U32)((U32)gu32PsmClkCnt * (U32)1000);
  u32TusElapsed = u32DiagTusElapsed(gNULL);
  
  if (u32TusElapsed > gstrPsmKeepAlive.u32TusElapsedMax) {
    gstrPsmKeepAlive.u32TusElapsedMax = u32TusElapsed;
  }
  gstrPsmKeepAlive.u32TusElapsed += u32TusElapsed;
}

/**
 * @brief   A public function that returns keep alive data.
 * @param   pvArgs  arguments reserved.
 * @sa      tstrDiagKeepAlive The diagnostic information for keep alive.
 * @return  void
 */
PUBLIC tstrDiagKeepAlive* pstrPsmKeepAlive(void *pvArgs) {
  return &gstrPsmKeepAlive;
}

