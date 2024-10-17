/**
 * @file    gsl.c
 * @brief   This file is used to implement GSL plays a counter role between GSL and UA. 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_feature.h"
#include "gsl_config.h"
#include "gsl_api.h"
#include "gsl_btm.h"
#include "gsl_psm.h"
#include "gsl_bsm.h"
#include "gsl_queue.h"
#include "gsl_diag.h"

#include <string.h>

/* External variables --------------------------------------------------------*/
EXTERN void vidPsmServiceDsm(void* pvArgs);
EXTERN void vidPsmServiceBsm(void* pvArgs);
EXTERN void vidPsmServiceLsm(void* pvArgs);
EXTERN U32  u32GslTickCountCallback(void* pvArgs);
EXTERN tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);
EXTERN void vidGslTraceCallback(char* pcTrace);
EXTERN tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuLsmType);
EXTERN void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty);

PUBLIC void vidBtmProcessIdle(void* pvArgs);
PUBLIC void vidBtmProcessTrace(void* pvArgs);

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PUBLIC const tstrPsmCfg gcpstrPsmCfgTbl[PSM_TYPE_MAX] = {
  /* u32Period    pfPsmService        */
  {  GSL_DSM_PRD, vidPsmServiceDsm  },  /* PSM_TYPE_DSM */
  {  GSL_BSM_PRD, vidPsmServiceBsm  },  /* PSM_TYPE_BSM */
  {  GSL_LSM_PRD, vidPsmServiceLsm  },  /* PSM_TYPE_LSM */
};

PUBLIC const tstrBsmCfg gcpstrBsmCfgTbl[BSM_TYPE_MAX] = {
  /* u32Period        u32MatchCount       u32PressedThreshHold  pfBsmEventCallback      */
  {  GSL_BSM_PRD_B1,  GSL_BSM_MATCH_CNT,  GSL_BSM_PRESS_TH,     GSL_BSM_EVT_CB_B1  },  /* BSM_TYPE_B1_BLUE */
};

PUBLIC const tstrLsmCfg gcpstrLsmCfgTbl[LSM_TYPE_MAX] = {
  /* u32Period        tenuBsmType     u32FadeInTimeOut    u32FadeOutTimeOut   pfLsmEventCallback  pfLsmOutputCallback */
  {  GSL_LSM_PRD_L1,  GSL_LSM_BSM_L1, GSL_LSM_FI_TMO_L1,  GSL_LSM_FO_TMO_L1,  GSL_LSM_EVT_CB_B1,  GSL_LSM_OUT_CB_B1 },  /* LSM_TYPE_LD2_GREEN */
};

PUBLIC const tstrDsmCfg gcpstrDsmCfgTbl[DSM_TYPE_MAX] = {
  /* u32Period      */
  {  GSL_LSM_PRD_D1 },  /* DSM_TYPE_KA */
};

PUBLIC const tstrBtmCfg gcpstrBtmCfgTbl[BTM_TYPE_MAX] = {
  /* pfBtmProcess        */
  {  GSL_BTM_PROC_IDLE  },  /* BTM_TYPE_IDLE */
  {  GSL_BTM_PROC_TRACE },  /* BTM_TYPE_TRACE */
};

PUBLIC const tstrGslCallbacks gcstrGslCalbacks = {
  GSL_TUS_CB,
  GSL_TRACE_CB,
};

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC void vidGslInitialize(void* pvArgs) {
  vidPsmInitialize(NULL);
  vidBtmInitialize(NULL);
  vidGslQueInitialize();
}

PUBLIC void vidGslService(void* pvArgs) {
  vidPsmService(NULL);
}

PUBLIC void vidGslProcess(void* pvArgs) {
  vidBtmProcess(NULL);
}

PUBLIC tenuBsmEventNotify enuGslBsmGetEventNotify(tenuBsmType enuType) {
  return enuBsmEventNotify(enuType);
}

PUBLIC void vidGslRefreshTus(void) {
  vidDiagRefreshTus(GSL_TUS_PRD_CNT);
}

PUBLIC __attribute__((weak)) U32 u32GslTickCountCallback(void *pvArgs) {
  return (U32)0;
}

PUBLIC __attribute__((weak)) void vidGslTraceCallback(char* pcTrace) {
}

PUBLIC __attribute__((weak)) tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType) {
  return (tenuBsmEvent)0; /* BSM_TYPE_B1_BLUE */
}

PUBLIC __attribute__((weak)) tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuType, tenuLsmType enuLsmType) {
  return (tenuLsmEvent)0; /* LSM_TYPE_LD2_GREEN */
}

PUBLIC __attribute__((weak)) void enuGslLsmOutputCallback(tenuBsmType enuType, U32 u32PwmDuty) {
  return;
}

/* External variables ---------------------------------------------- */

/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
/* Private functions ----------------------------------------------- */
/* Private variables ----------------------------------------------- */

