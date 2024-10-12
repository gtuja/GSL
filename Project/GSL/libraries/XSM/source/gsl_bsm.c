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
#include <string.h>

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/** Private tenuBsmEvt, with in line documents. */
typedef enum {
  BSM_EVT_NA = 0, /**< BSM event, not available. */
  BSM_EVT_RLS,    /**< BSM event, button is released. */
  BSM_EVT_PSH,    /**< BSM event, button is pushed. */
  BSM_EVT_MAX,    /**< BSM maximum event.  */
} tenuBsmEvt;

/** Private tenuBsmState, with in line documents. */
typedef enum {
  BSM_STT_NA = 0,   /**< BSM state, not available. */
  BSM_STT_RLS,      /**< BSM state, button is released. */
  BSM_STT_PSH_CFM,  /**< BSM state, button is pushed, but under confirmation. */
  BSM_STT_PSH,      /**< BSM state, button is pushed. */
  BSM_STT_RLS_CFM,  /**< BSM state, button is pushed, but under confirmation. */
  BSM_STT_MAX,      /**< BSM maximum state.  */
} tenuBsmState;

typedef struct {
  tstrGslBsmRegisterArgs  strArgs;          /**< strArgs is BSM feature set by UA through s32GslRegister and s32GslPsmRegister. */
  U32                     u32MatchCounter;  /**< u32MatchCounter is used for chattering prevention within BSM state machine. */
  U32                     u32PressCounter;  /**< u32PressCounter is used for fetching ISB event(tenuIsbEvent) within BSM state machine. */
  tenuBsmState            enuStateCurrent;  /**< The current BSM state. */
  tenuBsmState            enuStatePrevious; /**< The previous BSM state. */
  tenuBsmEventNotify      enuEventNotify;   /**< Event notified by BSM. */
} tstrBsmControl;

/* Private function prototypes -----------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
PRIVATE void vidBsmTransit(S32 s32Handle, tenuBsmState enuStateNext, void* pvArgs);

PRIVATE void vidBsmRlsEntry(S32 s32Handle);
PRIVATE void vidBsmRlsDo(S32 s32Handle);
PRIVATE void vidBsmRlsExit(S32 s32Handle);

PRIVATE void vidBsmPshCfmEntry(S32 s32Handle);
PRIVATE void vidBsmPshCfmDo(S32 s32Handle);
PRIVATE void vidBsmPshCfmExit(S32 s32Handle);

PRIVATE void vidBsmPshEntry(S32 s32Handle);
PRIVATE void vidBsmPshDo(S32 s32Handle);
PRIVATE void vidBsmPshExit(S32 s32Handle);

PRIVATE void vidBsmRlsCfmEntry(S32 s32Handle);
PRIVATE void vidBsmRlsCfmDo(S32 s32Handle);
PRIVATE void vidBsmRlsCfmExit(S32 s32Handle);

/* Private variables ---------------------------------------------------------*/
PRIVATE tstrBsmControl gpstrControl[GSL_BSM_MAX]; /** gpstrControl is a private variable holding information for BSM. */

/** gpfBsmStateFunctionTable is a private constant table holding BSM state functions. */
PRIVATE const tpfXsmStateFunction gpfBsmStateFunctionTable[BSM_STT_MAX][XSM_STT_FTN_MAX] = {
                        /*  XSM_STT_FTN_ENTRY   XSM_STT_FTN_DO  XSM_STT_FTN_EXIT  */
  /* BSM_STT_NA */      {   NULL,               NULL,           NULL              },
  /* BSM_STT_RLS */     {   vidBsmRlsEntry,     vidBsmRlsDo,    vidBsmRlsExit     },
  /* BSM_STT_PSH_CFM */ {   vidBsmPshCfmEntry,  vidBsmPshCfmDo, vidBsmPshCfmExit  },
  /* BSM_STT_PSH */     {   vidBsmPshEntry,     vidBsmPshDo,    vidBsmPshExit     },
  /* BSM_STT_RLS_CFM */ {   vidBsmRlsCfmEntry,  vidBsmRlsCfmDo, vidBsmRlsCfmExit  },
};

/** gpstrBsmTransitionTbl is a private constant table holding state transition information with each event. */
PRIVATE const tenuBsmState gpstrBsmTransitionTbl[BSM_STT_MAX][BSM_EVT_MAX] = {
                        /* BSM_EVT_NA  BSM_EVT_PSH      BSM_EVT_RLS */
  /* BSM_STT_NA */      {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_NA      },
  /* BSM_STT_RLS */     {  BSM_STT_NA, BSM_STT_PSH_CFM, BSM_STT_NA      },
  /* BSM_STT_PSH_CFM */ {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_RLS     },
  /* BSM_STT_PSH */     {  BSM_STT_NA, BSM_STT_NA,      BSM_STT_RLS_CFM },
  /* BSM_STT_RLS_CFM */ {  BSM_STT_NA, BSM_STT_RLS,     BSM_STT_NA      },
};

#if 0 /* TBD */
/** gpcBsmStateNameTable is state name for notification. */
PRIVATE const char* gpcBsmStateNameTable[BSM_STT_MAX] = {
  "BSM_STT_NA",
  "BSM_STT_RLS",
  "BSM_STT_PSH_CFM",
  "BSM_STT_PSH",
  "BSM_STT_RLS_CFM",
};
#endif /* TBD */

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

  for (i=0; i<GSL_BSM_MAX; i++) {
    gpstrControl[i].enuStateCurrent = BSM_STT_RLS;
  }
}

/**
 * @brief   Public function that register BSM features by ISB.
 * @param   pstrArgs  BSM features set by ISB.
 * @sa      vidIsbInitialize
 * @sa      tstrBsmRegisterArgs
 * @return  void
 */
PUBLIC void vidGslBsmRegister(S32 s32Handle, tstrGslBsmRegisterArgs* pstrArgs) {
  if (s32Handle != GSL_HNDL_NA && s32Handle < GSL_BSM_MAX) {
    memset(&(gpstrControl[s32Handle]), 0, sizeof(tstrBsmControl));
    memcpy(&(gpstrControl[s32Handle].strArgs), pstrArgs, sizeof(tstrGslBsmRegisterArgs));
  } else {
    /* Soft Error. */
  }
}

/**
 * @brief   Public function that process BSM called by PSM.
 * @param   void
 * @sa      vidGslPsmService
 * @return  void
 */
PUBLIC void vidGslBsmService(S32 s32Handle) {
  tenuBsmEvt enuEvent;
  tenuBsmState enuStateNext;
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  /* Get the next state with data passed and event extracted. */
  enuEvent = (pstrControl->strArgs.pfGslBsmExtractEventCallback(s32Handle) == TRUE) ? BSM_EVT_PSH : BSM_EVT_RLS;
  enuStateNext = gpstrBsmTransitionTbl[(U32)(pstrControl->enuStateCurrent)][(U32)enuEvent];

  /* Transit states if needed. */
  if (enuStateNext != BSM_STT_NA || enuStateNext != BSM_STT_MAX) {
    vidBsmTransit(s32Handle, enuStateNext, NULL);
  }

  /* Process the do state function. */
  if (gpfBsmStateFunctionTable[(U32)(pstrControl->enuStateCurrent)][(U32)XSM_STT_FTN_DO] != NULL) {
    gpfBsmStateFunctionTable[(U32)(pstrControl->enuStateCurrent)][(U32)XSM_STT_FTN_DO](s32Handle);
  }
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief   Private function that transit state on BSM.
 * @param   enuStateNext  The next state to transit.
 * @param   pvArgs        Arguments shall be set if needed.
 * @return  void
 */
PRIVATE void vidBsmTransit(S32 s32Handle, tenuBsmState enuStateNext, void* pvArgs) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  if (enuStateNext != BSM_STT_NA || enuStateNext != BSM_STT_MAX) {
    /* Process the exit state function of the current state. */
    if (gpfBsmStateFunctionTable[(U32)(pstrControl->enuStateCurrent)][(U32)XSM_STT_FTN_EXIT] != NULL) {
      gpfBsmStateFunctionTable[(U32)(pstrControl->enuStateCurrent)][(U32)XSM_STT_FTN_EXIT](s32Handle);
    }
    /* Transit states. */
    pstrControl->enuStatePrevious = pstrControl->enuStateCurrent;
    pstrControl->enuStateCurrent = enuStateNext;

    /* Process the entry state function of the next state. */
    if (gpfBsmStateFunctionTable[(U32)(pstrControl->enuStateCurrent)][(U32)XSM_STT_FTN_ENTRY] != NULL) {
      gpfBsmStateFunctionTable[(U32)(pstrControl->enuStateCurrent)][(U32)XSM_STT_FTN_ENTRY](s32Handle);
    }

#if 0 /* TBD */
    /* Notify state transition. */
    strArgs.enuNotify = BSM_NTF_LOG;
    snprintf(strArgs.pu8Log, SWD_LOG_LEN, "State changed [%s]->[%s]", \
                                          gpcBsmStateNameTable[gstrControl.enuStatePrevious], \
                                          gpcBsmStateNameTable[gstrControl.enuStateCurrent]);
    if (gstrControl.strArgs.pfBsmNotifyCallback != NULL) {
      gstrControl.strArgs.pfBsmNotifyCallback(&strArgs);
    }
#endif /* 0 */
  }
}

/**
 * @brief   Private entry function that is processed when the state transit to BSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsEntry(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  if ((pstrControl->u32PressCounter != (U32)0) && (pstrControl->u32PressCounter <= pstrControl->strArgs.u32PressedThreshHold)) {
    pstrControl->enuEventNotify = BSM_EVT_NTF_SHORT;
  }
  pstrControl->u32PressCounter = (U32)0;
}

/**
 * @brief   Private do function that is processed on BSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsDo(S32 s32Handle) {
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsExit(S32 s32Handle) {
}

/**
 * @brief   Private entry function that is processed when the state transit to BSM_STT_PRS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshCfmEntry(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);
  pstrControl->u32MatchCounter = (U32)0;
}

/**
 * @brief   Private do function that is processed on BSM_STT_PRS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshCfmDo(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  pstrControl->u32MatchCounter++;

  if (pstrControl->u32MatchCounter > pstrControl->strArgs.u32MatchCount)
  {
    vidBsmTransit(s32Handle, BSM_STT_PSH, NULL);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_PRS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshCfmExit(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  pstrControl->u32MatchCounter = (U32)0;
}

/**
 * @brief   Private entry function that is processed when the state transit to BSM_STT_PRS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshEntry(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  pstrControl->u32PressCounter = (U32)0;
}

/**
 * @brief   Private do function that is processed on BSM_STT_PRS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshDo(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  pstrControl->u32PressCounter++;
  if (pstrControl->u32PressCounter > pstrControl->strArgs.u32PressedThreshHold) {
    pstrControl->enuEventNotify = BSM_EVT_NTF_LONG;
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_PRS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmPshExit(S32 s32Handle) {
}

/**
 * @brief   Private entry function that is processed when the state is transited to BSM_STT_RLS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmEntry(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  pstrControl->u32MatchCounter = (U32)0;
}

/**
 * @brief   Private do function that is processed on BSM_STT_RLS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmDo(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  pstrControl->u32MatchCounter++;

  if (pstrControl->u32MatchCounter > pstrControl->strArgs.u32MatchCount)
  {
    vidBsmTransit(s32Handle, BSM_STT_RLS, NULL);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from BSM_STT_RLS_CFM.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidBsmRlsCfmExit(S32 s32Handle) {
  tstrBsmControl* pstrControl = &(gpstrControl[s32Handle]);

  pstrControl->u32MatchCounter = (U32)0;
}



