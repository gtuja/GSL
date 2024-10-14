/**
 * @file    gsl_bsm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_feature.h"
#include "gsl_api.h"
#include "gsl.h"
#include "gsl_bsm.h"
#include "gsl_queue.h"
#include "gsl_diag.h"
#include <string.h>
#include <stdio.h>

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/** Private tenuBsmEvt, with in line documents. */
/** Private tenuBsmState, with in line documents. */
typedef enum {
  BSM_STT_NA = 0,   /**< BSM state, not available. */
  BSM_STT_RLS,      /**< BSM state, button is released. */
  BSM_STT_PSH_CFM,  /**< BSM state, button is pushed, but under confirmation. */
  BSM_STT_PSH,      /**< BSM state, button is pushed. */
  BSM_STT_RLS_CFM,  /**< BSM state, button is pushed, but under confirmation. */
  BSM_STT_MAX,      /**< BSM maximum state.  */
} tenuBsmState;

typedef void (*tpfBsmSttFtn)(tenuBsmType enuType);  /** BSM state functions */
typedef struct {
  tpfBsmSttFtn pfEntry;  /**< BSM state function, entry. */
  tpfBsmSttFtn pfDo;     /**< BSM state function, do. */
  tpfBsmSttFtn pfExit;   /**< BSM state function, exit. */
} tstrBsmSttFtn;

typedef struct {
  const tstrBsmCfg*   pcstrBsmConfig;   /**< BSM configuration. */
  U32                 u32MatchCounter;  /**< u32MatchCounter is used for chattering prevention within BSM state machine. */
  U32                 u32PressCounter;  /**< u32PressCounter is used for fetching ISB event(tenuIsbEvent) within BSM state machine. */
  tenuBsmState        enuStateCurrent;  /**< The current BSM state. */
  tenuBsmState        enuStatePrevious; /**< The previous BSM state. */
  tenuBsmEventNotify  enuEventNotify;   /**< Event notified by BSM. */
  U32                 u32TimeProcess;   /**< The process time of BSM. */
} tstrBsmControl;

typedef struct {
  U32 u32TimeProcTotal;
  U32 u32TimeProcMax;
} tstrBsmDiag;

/* Private function prototypes -----------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
PRIVATE void vidBsmTransit(tenuBsmType enuType, tenuBsmState enuStateNext);

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

/* Private variables ---------------------------------------------------------*/
PRIVATE tstrBsmControl gpstrControl[BSM_TYPE_MAX] = {0};  /** gpstrControl is a private variable holding information for BSM. */
PRIVATE tstrBsmDiag gpstrBsmDiagProc[BSM_TYPE_MAX] = {0};
PRIVATE tstrBsmDiag gpstrBsmDiag = {0};

/** gpfBsmSttFtnTbl is a private constant table holding BSM state functions. */
PRIVATE const tpfBsmSttFtn gpfBsmSttFtnTbl[BSM_STT_MAX][XSM_STT_FTN_MAX] = {
                        /*  XSM_STT_FTN_ENTRY   XSM_STT_FTN_DO  XSM_STT_FTN_EXIT  */
  /* BSM_STT_NA */      {   NULL,               NULL,           NULL              },
  /* BSM_STT_RLS */     {   vidBsmRlsEntry,     vidBsmRlsDo,    vidBsmRlsExit     },
  /* BSM_STT_PSH_CFM */ {   vidBsmPshCfmEntry,  vidBsmPshCfmDo, vidBsmPshCfmExit  },
  /* BSM_STT_PSH */     {   vidBsmPshEntry,     vidBsmPshDo,    vidBsmPshExit     },
  /* BSM_STT_RLS_CFM */ {   vidBsmRlsCfmEntry,  vidBsmRlsCfmDo, vidBsmRlsCfmExit  },
};

/** gpstrBsmTransTbl is a private constant table holding state transition information with each event. */
PRIVATE const tenuBsmState gpstrBsmTransTbl[BSM_STT_MAX][BSM_EVT_MAX] = {
                        /* BSM_EVT_NA  BSM_EVT_RLS      BSM_EVT_PSH     */
  /* BSM_STT_NA */      {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_NA      },
  /* BSM_STT_RLS */     {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_PSH_CFM },
  /* BSM_STT_PSH_CFM */ {  BSM_STT_NA, BSM_STT_RLS,     BSM_STT_NA      },
  /* BSM_STT_PSH */     {  BSM_STT_NA, BSM_STT_RLS_CFM, BSM_STT_NA      },
  /* BSM_STT_RLS_CFM */ {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_RLS     },
};

/** gpcBsmStateNameTable is a string table holding state names for notification. */
PRIVATE const char* gcpcBsmSttNameTbl[BSM_STT_MAX] = {
  "BSM_STT_NA",
  "BSM_STT_RLS",
  "BSM_STT_PSH_CFM",
  "BSM_STT_PSH",
  "BSM_STT_RLS_CFM",
};

PRIVATE const char* gcpcBsm = "BSM";

/* Public functions ----------------------------------------------------------*/
/**
 * @brief   Public function that initialize BSM called by ISB.
 * @param   void
 * @sa      vidIsbInitialize
 * @return  void
 */
PUBLIC void vidBsmInitialize(void* pvArgs) {
  U32 i;

  memset(gpstrControl, 0, sizeof(gpstrControl));
  memset(gpstrBsmDiagProc, 0, sizeof(gpstrBsmDiagProc));
  memset(&gpstrBsmDiag, 0, sizeof(gpstrBsmDiag));

  for (i=0; i<(U32)BSM_TYPE_MAX; i++) {
    gpstrControl[i].pcstrBsmConfig = &(gcpstrBsmCfgTbl[i]);
    gpstrControl[i].enuStateCurrent = BSM_STT_RLS;
  }
}

/**
 * @brief   Public function that process BSM called by PSM.
 * @param   void
 * @sa      vidGslPsmService
 * @return  void
 */
PUBLIC void vidBsmService(void* pvArgs) {
  tenuBsmEvent enuEvent;
  tenuBsmState enuStateNext;
  U32 i;
  U32 u32TimeElapsed;

  for (i=0; i<(U32)BSM_TYPE_MAX; i++) {
    vidGslDiagTimeStart();

    /* Get the next state with data passed and event extracted. */
    enuEvent = gpstrControl[i].pcstrBsmConfig->pfBsmEventCallback((tenuBsmType)i);
    enuStateNext = gpstrBsmTransTbl[(U32)(gpstrControl[i].enuStateCurrent)][(U32)enuEvent];

    /* Transit states if needed. */
    if (enuStateNext != BSM_STT_NA) {
      vidBsmTransit((tenuBsmType)i, enuStateNext);
    }
    /* Process the do state function. */
    if (gpfBsmSttFtnTbl[(U32)(gpstrControl[i].enuStateCurrent)][(U32)XSM_STT_FTN_DO] != NULL) {
      gpfBsmSttFtnTbl[(U32)(gpstrControl[i].enuStateCurrent)][(U32)XSM_STT_FTN_DO]((tenuBsmType)i);
    }

    u32TimeElapsed = u32GslDiagTimeElapsed();
    if (u32TimeElapsed > gpstrBsmDiagProc[i].u32TimeProcMax) {
      gpstrBsmDiagProc[i].u32TimeProcMax = u32TimeElapsed;
    }
    gpstrBsmDiagProc[i].u32TimeProcTotal += u32TimeElapsed;
  }

  for (i=0; i<(U32)BSM_TYPE_MAX; i++) {
    if (gpstrBsmDiagProc[i].u32TimeProcMax > gpstrBsmDiag.u32TimeProcMax) {
      gpstrBsmDiag.u32TimeProcMax = gpstrBsmDiagProc[i].u32TimeProcMax;
    }
    gpstrBsmDiag.u32TimeProcTotal += gpstrBsmDiagProc[i].u32TimeProcTotal;
  }
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief   Private function that transit state on BSM.
 * @param   enuStateNext  The next state to transit.
 * @param   pvArgs        Arguments shall be set if needed.
 * @return  void
 */
PRIVATE void vidBsmTransit(tenuBsmType enuType, tenuBsmState enuStateNext) {
  CH pchTrace[GSL_TRACE_MAX];

  if (enuStateNext != BSM_STT_NA) {
    /* Process the exit state function of the current state. */
    if (gpfBsmSttFtnTbl[(U32)(gpstrControl[(U32)enuType].enuStateCurrent)][(U32)XSM_STT_FTN_EXIT] != NULL) {
      gpfBsmSttFtnTbl[(U32)(gpstrControl[(U32)enuType].enuStateCurrent)][(U32)XSM_STT_FTN_EXIT](enuType);
    }
    /* Transit states. */
    gpstrControl[(U32)enuType].enuStatePrevious = gpstrControl[(U32)enuType].enuStateCurrent;
    gpstrControl[(U32)enuType].enuStateCurrent = enuStateNext;

    /* Process the entry state function of the next state. */
    if (gpfBsmSttFtnTbl[(U32)(gpstrControl[(U32)enuType].enuStateCurrent)][(U32)XSM_STT_FTN_ENTRY] != NULL) {
      gpfBsmSttFtnTbl[(U32)(gpstrControl[(U32)enuType].enuStateCurrent)][(U32)XSM_STT_FTN_ENTRY](enuType);
    }
    snprintf(pchTrace, GSL_TRACE_MAX, "%s: State changed [%s]->[%s]", \
        gcpcBsm,  \
        gcpcBsmSttNameTbl[gpstrControl[(U32)enuType].enuStatePrevious], \
        gcpcBsmSttNameTbl[gpstrControl[(U32)enuType].enuStateCurrent]);
    vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
  }
}

/**
 * @brief   Private entry function that is processed when the state transit to BSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsEntry(tenuBsmType enuType) {
  if ((gpstrControl[(U32)enuType].u32PressCounter != (U32)0) && (gpstrControl[(U32)enuType].u32PressCounter <= gpstrControl[(U32)enuType].pcstrBsmConfig->u32PressedThreshHold)) {
    gpstrControl[(U32)enuType].enuEventNotify = BSM_EVT_NTF_SHORT;
  }
  gpstrControl[(U32)enuType].u32PressCounter = (U32)0;
}

/**
 * @brief   Private do function that is processed on BSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsDo(tenuBsmType enuType) {
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsExit(tenuBsmType enuType) {
}

/**
 * @brief   Private entry function that is processed when the state transit to BSM_STT_PRS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshCfmEntry(tenuBsmType enuType) {
  gpstrControl[(U32)enuType].u32MatchCounter = (U32)0;
}

/**
 * @brief   Private do function that is processed on BSM_STT_PRS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshCfmDo(tenuBsmType enuType) {
  gpstrControl[(U32)enuType].u32MatchCounter++;
  if (gpstrControl[(U32)enuType].u32MatchCounter > gpstrControl[(U32)enuType].pcstrBsmConfig->u32MatchCount)
  {
    vidBsmTransit(enuType, BSM_STT_PSH);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_PRS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshCfmExit(tenuBsmType enuType) {
  gpstrControl[(U32)enuType].u32MatchCounter = (U32)0;
}

/**
 * @brief   Private entry function that is processed when the state transit to BSM_STT_PRS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshEntry(tenuBsmType enuType) {
  gpstrControl[(U32)enuType].u32PressCounter = (U32)0;
}

/**
 * @brief   Private do function that is processed on BSM_STT_PRS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshDo(tenuBsmType enuType) {
  gpstrControl[(U32)enuType].u32PressCounter++;
  if (gpstrControl[(U32)enuType].u32PressCounter > gpstrControl[(U32)enuType].pcstrBsmConfig->u32PressedThreshHold) {
    gpstrControl[(U32)enuType].enuEventNotify = BSM_EVT_NTF_LONG;
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_PRS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshExit(tenuBsmType enuType) {
}

/**
 * @brief   Private entry function that is processed when the state is transited to BSM_STT_RLS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmEntry(tenuBsmType enuType) {
  gpstrControl[(U32)enuType].u32MatchCounter = (U32)0;
}

/**
 * @brief   Private do function that is processed on BSM_STT_RLS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmDo(tenuBsmType enuType) {
  gpstrControl[(U32)enuType].u32MatchCounter++;

  if (gpstrControl[(U32)enuType].u32MatchCounter > gpstrControl[(U32)enuType].pcstrBsmConfig->u32MatchCount)
  {
    vidBsmTransit(enuType, BSM_STT_RLS);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_RLS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmExit(tenuBsmType enuType) {
  gpstrControl[(U32)enuType].u32MatchCounter = (U32)0;
}

PUBLIC tenuBsmEventNotify enuBsmEventNotify(tenuBsmType enuType) {
  return gpstrControl[(U32)enuType].enuEventNotify;
}

