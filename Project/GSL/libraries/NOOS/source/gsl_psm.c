/**
 * @file    gsl_psm.c
 * @brief   This file is used to implement PSM manages periodic services.
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_psm.h"
#include "gsl_bsm.h"
#include "gsl_lsm.h"
#include "gsl_dsm.h"

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
  {  PSM_PRD_BSM, vidBsmInit, vidBsmSrvc  },  /* PSM_TYPE_DSM */
  {  PSM_PRD_LSM, vidLsmInit, gNULL       },  /* PSM_TYPE_BSM */
};

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
  for (i=0; i<(U32)PSM_TYPE_MAX; i++)  {
    if (gcpstrPsmCfgTbl[i].pfPsmInit != gNULL) {
      gcpstrPsmCfgTbl[i].pfPsmInit(gNULL);
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
