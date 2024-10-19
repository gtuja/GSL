/**
 * @file    gsl_bsm.c
 * @brief   This file is used to implement BSM manages button states.
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"
#include "gsl.h"
#include "gsl_bsm.h"
#include "gsl_dsm.h"
#include "gsl_queue.h"
#include <stdio.h>

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
typedef struct {
  const CH*           pcName;               /**< The name of BSM service. */
  U32                 u32Period;            /**< The cycle of execution. */
  U32                 u32CPMC;              /**< The match count of same button state for chattering prevention. */
  U32                 u32PressedThreshHold; /**< The threshold for fetching tenuBsmEventNotify, short or long press. */
  tpfBsmEventCallback pfBsmEventCallback;   /**< Callback for retrieving tenuBsmEvent. */
} tstrBsmCfg;

typedef void (*tpfBsmSttFtn)(tenuBsmType enuType);  /** BSM state functions */
typedef struct {
  tpfBsmSttFtn pfEntry;  /**< BSM state function, entry. */
  tpfBsmSttFtn pfDo;     /**< BSM state function, do. */
  tpfBsmSttFtn pfExit;   /**< BSM state function, exit. */
} tstrBsmSttFtn;

typedef struct {
  const tstrBsmCfg* pcstrBsmCfg;  /**< BSM configuration. */
  U32               u32CPMCCnt;   /**< u32CPMCCnt is used for chattering prevention within BSM state machine. */
  U32               u32PressCnt;  /**< u32PressCnt is used for fetching ISB event(tenuIsbEvent) within BSM state machine. */
  tenuBsmStt        enuSttCur;    /**< The current BSM state. */
  tenuBsmStt        enuSttPrev;   /**< The previous BSM state. */
  tenuBsmNotify     enuNotify;    /**< Event notified by BSM. */
} tstrBsmCtrl;

/* Private function prototypes ------------------------------------- */
PRIVATE void vidBsmTransit(tenuBsmType enuType, tenuBsmStt enuStateNext);

PRIVATE void vidBsmRlsEntry(tenuBsmType enuType);
PRIVATE void vidBsmRlsDo(tenuBsmType enuType);
PRIVATE void vidBsmRlsExit(tenuBsmType enuType);

PRIVATE void vidBsmPshCfmEntry(tenuBsmType enuType);
PRIVATE void vidBsmPshCfmDo(tenuBsmType enuType);
PRIVATE void vidBsmPshCfmExit(tenuBsmType enuType);

PRIVATE void vidBsmPshEntry(tenuBsmType enuType);
PRIVATE void vidBsmPshDo(tenuBsmType enuType);
PRIVATE void vidBsmPshExit(tenuBsmType enuType);

PRIVATE void vidBsmRlsCfmEntry(tenuBsmType enuType);
PRIVATE void vidBsmRlsCfmDo(tenuBsmType enuType);
PRIVATE void vidBsmRlsCfmExit(tenuBsmType enuType);

/* Private variables ----------------------------------------------- */
/** 
 * @brief gpstrBsmCtrl is a private table holding information for BSM.
 * @sa    tenuBsmType
 */
PRIVATE tstrBsmCtrl gpstrBsmCtrl[BSM_TYPE_MAX] = {0};

/** 
 * @brief gcpstrBsmCfgTbl is a private constant table holding BSM features.
 * @sa    tenuBsmType
 */
PRIVATE const tstrBsmCfg gcpstrBsmCfgTbl[BSM_TYPE_MAX] = {
  /* pcName       u32Period    u32CPMC     u32PressedThreshHold  pfBsmEventCallback  */
  {  BSM_NAME_B0, BSM_PRD_B0,  BSM_CP_MC,  BSM_NTF_TH,           BSM_EVT_CB_B0 },  /* BSM_TYPE_B0 */
  {  BSM_NAME_B1, BSM_PRD_B1,  BSM_CP_MC,  BSM_NTF_TH,           BSM_EVT_CB_B1 },  /* BSM_TYPE_B1 */
  {  BSM_NAME_B2, BSM_PRD_B2,  BSM_CP_MC,  BSM_NTF_TH,           BSM_EVT_CB_B2 },  /* BSM_TYPE_B2 */
  {  BSM_NAME_B3, BSM_PRD_B3,  BSM_CP_MC,  BSM_NTF_TH,           BSM_EVT_CB_B3 },  /* BSM_TYPE_B3 */
  {  BSM_NAME_B4, BSM_PRD_B4,  BSM_CP_MC,  BSM_NTF_TH,           BSM_EVT_CB_B4 },  /* BSM_TYPE_B4 */
};

/** 
 * @brief gpfBsmSttFtnTbl is a private constant table holding BSM state functions.
 * @sa    tenuBsmStt
 * @sa    tenuXsmSttFtn
 */
PRIVATE const tpfBsmSttFtn gpfBsmSttFtnTbl[BSM_STT_MAX][XSM_STT_FTN_MAX] = {
                        /*  XSM_STT_FTN_ENTRY   XSM_STT_FTN_DO  XSM_STT_FTN_EXIT  */
  /* BSM_STT_NA */      {   gNULL,              gNULL,          gNULL             },
  /* BSM_STT_RLS */     {   vidBsmRlsEntry,     vidBsmRlsDo,    vidBsmRlsExit     },
  /* BSM_STT_PSH_CFM */ {   vidBsmPshCfmEntry,  vidBsmPshCfmDo, vidBsmPshCfmExit  },
  /* BSM_STT_PSH */     {   vidBsmPshEntry,     vidBsmPshDo,    vidBsmPshExit     },
  /* BSM_STT_RLS_CFM */ {   vidBsmRlsCfmEntry,  vidBsmRlsCfmDo, vidBsmRlsCfmExit  },
};

/**
 * @brief gpstrBsmTransTbl is a private constant table holding state transition.
 *        information for each of events.
 * @sa    tenuBsmStt
 * @sa    tenuBsmEvent
 */
PRIVATE const tenuBsmStt gpstrBsmTransTbl[BSM_STT_MAX][BSM_EVT_MAX] = {
                        /* BSM_EVT_NA  BSM_EVT_RLS      BSM_EVT_PSH     */
  /* BSM_STT_NA */      {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_NA      },
  /* BSM_STT_RLS */     {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_PSH_CFM },
  /* BSM_STT_PSH_CFM */ {  BSM_STT_NA, BSM_STT_RLS,     BSM_STT_NA      },
  /* BSM_STT_PSH */     {  BSM_STT_NA, BSM_STT_RLS_CFM, BSM_STT_NA      },
  /* BSM_STT_RLS_CFM */ {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_RLS     },
};

/**
 * @brief gu32BsmCnt is a counter accumulated to control each of BSM periods.
 */
PRIVATE U32 gu32BsmCnt = (S32)0;

/**
 * @brief gcpcBsmName is the name of BSM.
 */
PRIVATE const char* gcpcBsmName = "BSM";

/**
 * @brief gstrBsmDiag is the diagnostic information of BSM.
 */
PRIVATE tstrBsmDiag gstrBsmDiag = {0};

/**
 * @brief gcpcBsmSttNameTbl is a string table holding state names for notification.
 */
PRIVATE const char* gcpcBsmSttNameTbl[BSM_STT_MAX] = {
  "BSM_STT_NA",
  "BSM_STT_RLS",
  "BSM_STT_PSH_CFM",
  "BSM_STT_PSH",
  "BSM_STT_RLS_CFM",
};

/* Public functions ------------------------------------------------ */
/**
 * @brief   A public function that initialize BSM called by ISB.
 * @param   pvArgs  arguments reserved. 
 * @sa      vidPsmInit
 * @return  void
 */
PUBLIC void vidBsmInit(void* pvArgs) {
  U32 i;

  gstrBsmDiag.pcName = gcpcBsmName;
  for (i=0; i<(U32)BSM_TYPE_MAX; i++) {
    gpstrBsmCtrl[i].pcstrBsmCfg = &(gcpstrBsmCfgTbl[i]);
    gpstrBsmCtrl[i].enuSttCur = BSM_STT_RLS;
    gstrBsmDiag.strDiag[i].pcSrvName = gpstrBsmCtrl[i].pcstrBsmCfg->pcName;
    gstrBsmDiag.strDiag[i].pcSttName = gcpcBsmSttNameTbl;
    gstrBsmDiag.strDiag[i].bIsRegistered = (gpstrBsmCtrl[i].pcstrBsmCfg->u32Period != (U32)0) ? gTRUE : gFALSE;

  }
  gu32BsmCnt = (U32)0;
}

/**
 * @brief   A public function that process BSM called by PSM.
 * @param   pvArgs  arguments reserved. 
 * @sa      vidGslPsmService
 * @return  void
 */
PUBLIC void vidBsmSrvc(void* pvArgs) {
  tenuBsmEvent enuEvent;
  tenuBsmStt enuStateNext;
  U32 i;
  
  gu32BsmCnt++;
  for (i=0; i<(U32)BSM_TYPE_MAX; i++) {
    if (gpstrBsmCtrl[i].pcstrBsmCfg->u32Period != (U32)0) {
      if (gu32BsmCnt % gpstrBsmCtrl[i].pcstrBsmCfg->u32Period == (U32)0) {
        /* Get the next state with data passed and event extracted. */
        enuEvent = gpstrBsmCtrl[i].pcstrBsmCfg->pfBsmEventCallback((tenuBsmType)i);
        enuStateNext = gpstrBsmTransTbl[(U32)(gpstrBsmCtrl[i].enuSttCur)][(U32)enuEvent];

        /* Transit states if needed. */
        if (enuStateNext != BSM_STT_NA) {
          vidBsmTransit((tenuBsmType)i, enuStateNext);
        }
        /* Process the do state function. */
        if (gpfBsmSttFtnTbl[(U32)(gpstrBsmCtrl[i].enuSttCur)][(U32)XSM_STT_FTN_DO] != gNULL) {
          gpfBsmSttFtnTbl[(U32)(gpstrBsmCtrl[i].enuSttCur)][(U32)XSM_STT_FTN_DO]((tenuBsmType)i);
        }
      }
    }
  }
}

/**
 * @brief   A public function getting BSM notification.
 * @param   enuType BSM type for each of buttons.
 * @note    UA shall call this callback while extracting LSM events.
 * @return  tenuBsmNotify
 */
PUBLIC tenuBsmNotify enuGslBsmNotifyCallback(tenuBsmType enuType) {
  return gpstrBsmCtrl[(U32)enuType].enuNotify;
}

/**
 * @brief   A public function to trace BSM features.
 * @param   pvArgs  arguments reserved.
 * @return  void
 */
PUBLIC void vidBsmDiag(void* pvArgs) {
  CH pchTrace[GSL_QUE_TRACE_LEN];
  U32 i, j;

  for (i=0; i<BSM_TYPE_MAX; i++) {
    if (gstrBsmDiag.strDiag[i].bIsRegistered == gTRUE) {
      snprintf(pchTrace, GSL_QUE_TRACE_LEN, "## %s[%s] ------------------", gstrBsmDiag.pcName, gstrBsmDiag.strDiag[i].pcSrvName);
      vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
      for (j=0; j<BSM_STT_MAX; j++) {
        snprintf(pchTrace, GSL_QUE_TRACE_LEN, "## %s[%08ld]", gstrBsmDiag.strDiag[i].pcSttName[j], gstrBsmDiag.strDiag[i].pu32SttCnt[j]);
        vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
      }
    }
  }
}

/* Private functions ----------------------------------------------- */
/**
 * @brief   A private function that transit states on BSM.
 * @param   enuType       The BSM type for each of buttons.
 * @param   enuStateNext  The next state to transit.
 * @param   pvArgs        Arguments shall be set if needed.
 * @return  void
 */
PRIVATE void vidBsmTransit(tenuBsmType enuType, tenuBsmStt enuStateNext) {
  CH pchTrace[GSL_QUE_TRACE_LEN];

  if (enuStateNext != BSM_STT_NA) {
    /* Process the exit state function of the current state. */
    if (gpfBsmSttFtnTbl[(U32)(gpstrBsmCtrl[(U32)enuType].enuSttCur)][(U32)XSM_STT_FTN_EXIT] != gNULL) {
      gpfBsmSttFtnTbl[(U32)(gpstrBsmCtrl[(U32)enuType].enuSttCur)][(U32)XSM_STT_FTN_EXIT](enuType);
    }
    /* Transit states. */
    gpstrBsmCtrl[(U32)enuType].enuSttPrev = gpstrBsmCtrl[(U32)enuType].enuSttCur;
    gpstrBsmCtrl[(U32)enuType].enuSttCur = enuStateNext;

    /* Process the entry state function of the next state. */
    if (gpfBsmSttFtnTbl[(U32)(gpstrBsmCtrl[(U32)enuType].enuSttCur)][(U32)XSM_STT_FTN_ENTRY] != gNULL) {
      gpfBsmSttFtnTbl[(U32)(gpstrBsmCtrl[(U32)enuType].enuSttCur)][(U32)XSM_STT_FTN_ENTRY](enuType);
    }
    snprintf(pchTrace, GSL_QUE_TRACE_LEN, "%s: State changed [%s]->[%s]", \
        gpstrBsmCtrl[(U32)enuType].pcstrBsmCfg->pcName,  \
        gcpcBsmSttNameTbl[gpstrBsmCtrl[(U32)enuType].enuSttPrev], \
        gcpcBsmSttNameTbl[gpstrBsmCtrl[(U32)enuType].enuSttCur]);
    vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
  }
}

/**
 * @brief   A private entry function that is processed
 *          when the state transit to BSM_STT_RLS.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmRlsEntry(tenuBsmType enuType) {
  if ((gpstrBsmCtrl[(U32)enuType].u32PressCnt != (U32)0) && (gpstrBsmCtrl[(U32)enuType].u32PressCnt <= gpstrBsmCtrl[(U32)enuType].pcstrBsmCfg->u32PressedThreshHold)) {
    gpstrBsmCtrl[(U32)enuType].enuNotify = BSM_NTF_SHORT;
  }
  gpstrBsmCtrl[(U32)enuType].u32PressCnt = (U32)0;
}

/**
 * @brief   A private do function that is processed on BSM_STT_RLS.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmRlsDo(tenuBsmType enuType) {
  gstrBsmDiag.strDiag[enuType].pu32SttCnt[BSM_STT_RLS]++;
}

/**
 * @brief   A private exit function that is processed when the state go out from BSM_STT_RLS.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmRlsExit(tenuBsmType enuType) {
}

/**
 * @brief   A private entry function that is processed when the state transit to BSM_STT_PRS_CFM.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmPshCfmEntry(tenuBsmType enuType) {
  gpstrBsmCtrl[(U32)enuType].u32CPMCCnt = (U32)0;
}

/**
 * @brief   A private do function that is processed on BSM_STT_PRS_CFM.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmPshCfmDo(tenuBsmType enuType) {
  gstrBsmDiag.strDiag[enuType].pu32SttCnt[BSM_STT_PSH_CFM]++;
  gpstrBsmCtrl[(U32)enuType].u32CPMCCnt++;
  
  if (gpstrBsmCtrl[(U32)enuType].u32CPMCCnt >= gpstrBsmCtrl[(U32)enuType].pcstrBsmCfg->u32CPMC)
  {
    vidBsmTransit(enuType, BSM_STT_PSH);
  }
}

/**
 * @brief   A private exit function that is processed when the state go out from BSM_STT_PRS_CFM.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmPshCfmExit(tenuBsmType enuType) {
  gpstrBsmCtrl[(U32)enuType].u32CPMCCnt = (U32)0;
}

/**
 * @brief   A private entry function that is processed when the state transit to BSM_STT_PRS.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmPshEntry(tenuBsmType enuType) {
  gpstrBsmCtrl[(U32)enuType].u32PressCnt = (U32)0;
}

/**
 * @brief   A private do function that is processed on BSM_STT_PRS.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmPshDo(tenuBsmType enuType) {
  gstrBsmDiag.strDiag[enuType].pu32SttCnt[BSM_STT_PSH]++;
  gpstrBsmCtrl[(U32)enuType].u32PressCnt++;
  
  if (gpstrBsmCtrl[(U32)enuType].u32PressCnt >= gpstrBsmCtrl[(U32)enuType].pcstrBsmCfg->u32PressedThreshHold) {
    gpstrBsmCtrl[(U32)enuType].enuNotify = BSM_NTF_LONG;
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_PRS.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmPshExit(tenuBsmType enuType) {
}

/**
 * @brief   A private entry function that is processed when the state is transited to BSM_STT_RLS_CFM.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmEntry(tenuBsmType enuType) {
  gpstrBsmCtrl[(U32)enuType].u32CPMCCnt = (U32)0;
}

/**
 * @brief   A private do function that is processed on BSM_STT_RLS_CFM.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmDo(tenuBsmType enuType) {
  gstrBsmDiag.strDiag[enuType].pu32SttCnt[BSM_STT_RLS_CFM]++;
  gpstrBsmCtrl[(U32)enuType].u32CPMCCnt++;

  if (gpstrBsmCtrl[(U32)enuType].u32CPMCCnt >= gpstrBsmCtrl[(U32)enuType].pcstrBsmCfg->u32CPMC)
  {
    vidBsmTransit(enuType, BSM_STT_RLS);
  }
}

/**
 * @brief   A private exit function that is processed when the state go out from BSM_STT_RLS_CFM.
 * @param   enuType The BSM type for each of buttons.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmExit(tenuBsmType enuType) {
  gpstrBsmCtrl[(U32)enuType].u32CPMCCnt = (U32)0;
}

/* Weak functions -------------------------------------------------- */
/**
 * @brief   A public weak function for retrieving BSM event.
 * @param   enuType The BSM type for each of buttons.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  tenuBsmEvent  BSM button event.
 */
PUBLIC __attribute__((weak)) tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType) {
  return BSM_TYPE_B0;
}
