/**
 * @file    gsl.c
 * @brief   This file is used to implement GSL plays a counter role between GSL and UA. 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_feature.h"
#include "gsl_api.h"
#include "gsl_btm.h"
#include "gsl_psm.h"
#include "gsl_bsm.h"
#include "gsl_queue.h"
#include <string.h>

/* External variables --------------------------------------------------------*/
EXTERN void vidPsmServiceClock(void* pvArgs);
EXTERN void vidPsmServiceBsm(void* pvArgs);
EXTERN void vidPsmServiceLsm(void* pvArgs);
EXTERN U32  u32GslTickCountCallback(void* pvArgs);
EXTERN U32  u32GslTickPeriodCallback(void* pvArgs);
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
  /* u32Period  pfPsmService        */
  {  (U32)1,    vidPsmServiceClock  },  /* PSM_TYPE_CLK */
  {  (U32)1,    vidPsmServiceBsm    },  /* PSM_TYPE_BSM */
  {  (U32)1,    vidPsmServiceLsm    },  /* PSM_TYPE_LSM */
};

PUBLIC const tstrBsmCfg gcpstrBsmCfgTbl[BSM_TYPE_MAX] = {
  /* u32Period  u32MatchCount u32PressedThreshHold  pfBsmEventCallback  */
  {  (U32)1,    (U32)5,       (U32)1000,            enuGslBsmEventCallback  },  /* BSM_TYPE_B1_BLUE */
};

PUBLIC const tstrLsmCfg gcpstrLsmCfgTbl[LSM_TYPE_MAX] = {
  /* tenuBsmType        u32Period u32FadeInTimeOut  u32FadeOutTimeOut pfLsmEventCallback      pfLsmOutputCallback    */
  {  BSM_TYPE_B1_BLUE,  (U32)1,   (U32)1000,        (U32)2000,        enuGslLsmEventCallback, vidGslLsmOutputCallback },  /* LSM_TYPE_LD2_GREEN */
};

PUBLIC const tstrBtmCfg gcpstrBtmCfgTbl[BTM_TYPE_MAX] = {
  /* pfBtmProcess        */
  {  vidBtmProcessIdle  },  /* BTM_TYPE_IDLE */
  {  vidBtmProcessTrace },  /* BTM_TYPE_TRACE */
};

PUBLIC const tstrGslCallbacks gcstrGslCalbacks = {
  u32GslTickCountCallback,
  u32GslTickPeriodCallback,
  vidGslTraceCallback,
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

PUBLIC __attribute__((weak)) U32 u32GslTickCountCallback(void *pvArgs) {
  return (U32)0;
}

PUBLIC __attribute__((weak)) U32 u32GslTickPeriodCallback(void *pvArgs) {
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

