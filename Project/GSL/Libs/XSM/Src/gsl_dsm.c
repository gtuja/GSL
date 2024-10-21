/**
 * @file    gsl_dsm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 16, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_bsm.h"
#include "gsl_diag.h"

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
PRIVATE void vidDsmKeepAlive(void* pvArgs);
PRIVATE void vidDsmDiag(void* pvArgs);

/* Private variables ----------------------------------------------- */
typedef struct {
  U32         u32Period;  /**< The cycle of execution. */
  tpfDsmSrvc  pfDsmSrvc;  /**< DSM service. */
} tstrDsmCfg;

PRIVATE U32 gu32DsmCnt = (U32)0;

PRIVATE const tstrDsmCfg gcpstrDsmCfgTbl[DSM_TYPE_MAX] = {
  /* u32Period            tpfDsmSrvc */
  {  DSM_PRD_KEEP_ALIVE,  vidDsmKeepAlive },  /* DSM_TYPE_KEEP_ALIVE */
  {  DSM_PRD_XSM_STATE,   vidDsmDiag      },  /* DSM_TYPE_XSM_STT */
};

/* A public functions ------------------------------------------------ */
/**
 * @brief   A public function that process BSM called by PSM.
 * @param   void
 * @sa      vidGslPsmService
 * @return  void
 */
PUBLIC void vidDsmInit(void* pvArgs) {
  gu32DsmCnt = (U32)0;
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

/* Private functions ----------------------------------------------- */
/**
 * @brief   A private function that process DSM keep alive service.
 * @param   void
 * @sa      vidDsmSrvc
 * @return  void
 */
PRIVATE void vidDsmKeepAlive(void* pvArgs) {
  vidDiagKeepAlive(gNULL);
}

/**
 * @brief   A private function that process DSM diagnostic service.
 * @param   void
 * @sa      vidDsmSrvc
 * @return  void
 */
PRIVATE void vidDsmDiag(void* pvArgs) {
  vidBsmDiag(gNULL);
}


