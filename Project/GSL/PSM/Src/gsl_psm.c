/**
 * @file    gsl_psm.c
 * @brief   This file is used to implement PSM manages periodic services.
 * @author  Gtuja
 * @date    Nov 7, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_psm.h"
#include "gsl_bsm.h"
#include "gsl_lsm.h"
#include "gsl_diag.h"

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
typedef void (*tpfPsmInit)(void* pvArgs);
typedef void (*tpfPsmSrvc)(void* pvArgs);
typedef struct {
  U32         u32Period;  /**< PSM period. */
  tpfPsmInit  pfPsmInit;  /**< PSM Initialize. */
  tpfPsmSrvc  pfPsmSrvc;  /**< PSM service. */
} tstrPsmCfg;

/* Private function prototypes ------------------------------------- */
/* Private variables ----------------------------------------------- */
PRIVATE U32 gu32PsmCnt;   /** @brief A private variable holding PSM count. */
PRIVATE U32 gu32PsmOtMax; /** @brief A private variable holding the maximum occupation time of PSM. */

/**
 * @brief gcpstrPsmCfgTbl is a private table holding PSM services.
 * @sa    tstrPsmCfg
 * @sa    tenuPsmType
 */
PRIVATE const tstrPsmCfg gcpstrPsmCfgTbl[PSM_TYPE_MAX] = {
  /* u32Period    tpfPsmInit  pfPsmService  */
  {  PSM_PRD_BSM, vidBsmInit, vidBsmSrvc  },  /* PSM_TYPE_BSM */
  {  PSM_PRD_LSM, vidLsmInit, vidLsmSrvc  },  /* PSM_TYPE_LSM */
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

  gu32PsmCnt = (U32)0;
  gu32PsmOtMax = (U32)0;

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
  
  gu32PsmCnt++;
  vidDiagTusStart(gNULL);

  for (i=0; i<(U32)PSM_TYPE_MAX; i++)  {
    if (gcpstrPsmCfgTbl[i].u32Period != gNULL) {
      if ((gu32PsmCnt % gcpstrPsmCfgTbl[i].u32Period) == (U32)0) {
        if (gcpstrPsmCfgTbl[i].pfPsmSrvc != gNULL) {
          gcpstrPsmCfgTbl[i].pfPsmSrvc(gNULL);
        }
      }
    }
  }

  u32TusElapsed = u32DiagTusElapsed(gNULL);
  if (u32TusElapsed > gu32PsmOtMax) {
    gu32PsmOtMax = u32TusElapsed;
  }

  if ((gu32PsmCnt % DIAG_PRD_KA) == (U32)0) {
    vidDiagKeepAlive(gu32PsmCnt, gu32PsmOtMax);
  }
}

