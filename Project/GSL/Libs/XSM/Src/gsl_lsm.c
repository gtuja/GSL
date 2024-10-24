/**
 * @file    gsl_lsm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_xsm.h"
#include "gsl_lsm.h"
#include "gsl_queue.h"
#include <stdio.h>

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum {
  LSM_STT_NA = 0,   /**< LSM state, not available. */
  LSM_STT_OFF,      /**< LSM state, LED is off. */
  LSM_STT_FADE_IN,  /**< LSM state, LED is under fade in. */
  LSM_STT_ON,       /**< LSM state, LED is on. */
  LSM_STT_FADE_OUT, /**< LSM state, LED is under fade out. */
  LSM_STT_MAX,      /**< LSM maximum state. */
} tenuLsmState;

typedef struct {
  const CH*           pcName;               /**< The name of LSM service. */
  U32                 u32Period;            /**< The periodic cycle for fading. */
  tenuBsmType         enuBsmType;           /**< BSM linked with LSM */
  U32                 u32FadeInTmo;         /**< The fade in timeout. */
  U16                 u32FadeOutTmo;        /**< The fade out timeout. */
  tpfLsmEventCallback pfLsmEventCallback;   /**< Callback for retrieving tenuLlmState. */
  tpfLsmOutputCallback pfLsmOutputCallback; /**< Callback for LED output. */
  tpfLsmPwmMinCallback pfLsmPwmMinCallback; /**< Callback to get the minimum PWM duty. */
  tpfLsmPwmMaxCallback pfLsmPwmMaxCallback; /**< Callback to get the maximum PWM duty. */
} tstrLsmCfg;

typedef void (*tpfLsmSttFtn)(tenuLsmType enuType);  /** LSM state functions */
typedef struct {
  tpfLsmSttFtn pfEntry;  /**< BSM state function, entry. */
  tpfLsmSttFtn pfDo;     /**< BSM state function, do. */
  tpfLsmSttFtn pfExit;   /**< BSM state function, exit. */
} tstrLsmSttFtn;

typedef struct {
  const tstrLsmCfg* pcstrLsmCfg;      /**< LSM configuration. */
  U32               u32FadeCnt;       /**< u32MatchCounter is used for chattering prevention within XLM state machine. */
  U32               u32FadeCntMax;    /**< u32MatchCounter is used for chattering prevention within XLM state machine. */
  U32               u32FadeDutyDiff;  /**< u32MatchCounter is used for chattering prevention within XLM state machine. */
  tenuLsmState      enuSttCur;        /**< The current LSM state. */
  tenuLsmState      enuSttPrev;       /**< The previous LSM state. */
} tstrLsmControl;

/* Private function prototypes -----------------------------------------------*/
PRIVATE void vidLsmTransit(tenuLsmType enuType, tenuLsmState enuStateNext);

PRIVATE void vidLsmOffEntry(tenuLsmType enuType);
PRIVATE void vidLsmOffDo(tenuLsmType enuType);
PRIVATE void vidLsmOffExit(tenuLsmType enuType);

PRIVATE void vidLsmFadeInEntry(tenuLsmType enuType);
PRIVATE void vidLsmFadeInDo(tenuLsmType enuType);
PRIVATE void vidLsmFadeInExit(tenuLsmType enuType);

PRIVATE void vidLsmOnEntry(tenuLsmType enuType);
PRIVATE void vidLsmOnDo(tenuLsmType enuType);
PRIVATE void vidLsmOnExit(tenuLsmType enuType);

PRIVATE void vidLsmFadeOffEntry(tenuLsmType enuType);
PRIVATE void vidLsmFadeOffDo(tenuLsmType enuType);
PRIVATE void vidLsmFadeOffExit(tenuLsmType enuType);

/* Private variables ---------------------------------------------------------*/
PRIVATE tstrLsmControl gpstrLsmCtrl[LSM_TYPE_MAX] = {0};  /** gpstrLsmCtrl is a private variable holding information for BSM. */
PRIVATE const tstrLsmCfg gcpstrLsmCfgTbl[LSM_TYPE_MAX] = {
  /* pcName       u32Period    tenuBsmType       u32FadeInTimeOut  u32FadeOutTimeOut pfLsmEventCallback      pfLsmOutputCallback       tpfLsmPwmMinCallback      tpfLsmPwmMaxCallback    */
  {  LSM_NAME_L0, LSM_PRD_L0,  LSM_BSM_TYPE_L0,  LSM_FI_TMO_L0,    LSM_FO_TMO_L0,    enuGslLsmEventCallback, vidGslLsmOutputCallback,  enuGslLsmPwmMinCallback,  enuGslLsmPwmMaxCallback },  /* LSM_TYPE_LD2_GREEN */
  {  LSM_NAME_L1, LSM_PRD_L1,  LSM_BSM_TYPE_L1,  LSM_FI_TMO_L1,    LSM_FO_TMO_L1,    enuGslLsmEventCallback, vidGslLsmOutputCallback,  enuGslLsmPwmMinCallback,  enuGslLsmPwmMaxCallback },  /* Dummy */
  {  LSM_NAME_L2, LSM_PRD_L2,  LSM_BSM_TYPE_L2,  LSM_FI_TMO_L2,    LSM_FO_TMO_L2,    enuGslLsmEventCallback, vidGslLsmOutputCallback,  enuGslLsmPwmMinCallback,  enuGslLsmPwmMaxCallback },  /* Dummy */
  {  LSM_NAME_L3, LSM_PRD_L3,  LSM_BSM_TYPE_L3,  LSM_FI_TMO_L3,    LSM_FO_TMO_L3,    enuGslLsmEventCallback, vidGslLsmOutputCallback,  enuGslLsmPwmMinCallback,  enuGslLsmPwmMaxCallback },  /* Dummy */
  {  LSM_NAME_L4, LSM_PRD_L4,  LSM_BSM_TYPE_L4,  LSM_FI_TMO_L4,    LSM_FO_TMO_L4,    enuGslLsmEventCallback, vidGslLsmOutputCallback,  enuGslLsmPwmMinCallback,  enuGslLsmPwmMaxCallback },  /* Dummy */
};

/** gpfLsmSttFtnTbl is a private constant table holding LSM state functions. */
PRIVATE const tpfLsmSttFtn gpfLsmSttFtnTbl[LSM_STT_MAX][XSM_STT_FTN_MAX] = {
                          /*  LSM_STT_FTN_ENTRY   LM_STT_FTN_DO     LSM_STT_FTN_EXIT  */
  /* LSM_STT_NA */        {   gNULL,              gNULL,            gNULL             },
  /* LSM_STT_OFF */       {   vidLsmOffEntry,     vidLsmOffDo,      vidLsmOffExit     },
  /* LSM_STT_FADE_IN */   {   vidLsmFadeInEntry,  vidLsmFadeInDo,   vidLsmFadeInExit  },
  /* LSM_STT_ON */        {   vidLsmOnEntry,      vidLsmOnDo,       vidLsmOnExit      },
  /* LSM_STT_FADE_OUT */  {   vidLsmFadeOffEntry, vidLsmFadeOffDo,  vidLsmFadeOffExit },
};

/** gstrXlmTransitionTbl is a private constant table holding state transition information with each event. */
PRIVATE const tenuLsmState gpstrLsmTransTbl[LSM_STT_MAX][LSM_EVT_MAX] =
{
                          /* ISB_EVT_NA  ISB_EVT_SHORT      ISB_EVT_LONG  */
  /* LSM_STT_NA */        {  LSM_STT_NA, LSM_STT_NA,        LSM_STT_NA,   },
  /* LSM_STT_OFF */       {  LSM_STT_NA, LSM_STT_FADE_IN,   LSM_STT_ON,   },
  /* LSM_STT_FADE_IN */   {  LSM_STT_NA, LSM_STT_NA,        LSM_STT_OFF,  },
  /* LSM_STT_ON */        {  LSM_STT_NA, LSM_STT_FADE_OUT,  LSM_STT_OFF,  },
  /* LSM_STT_FADE_OUT */  {  LSM_STT_NA, LSM_STT_NA,        LSM_STT_OFF,  },
};

/** gcpcLsmSttNameTbl is a string table holding state names for notification. */
PRIVATE const char* gcpcLsmSttNameTbl[LSM_STT_MAX] = {
  "LSM_STT_NA",
  "LSM_STT_OFF",
  "LSM_STT_FADE_IN",
  "LSM_STT_ON",
  "LSM_STT_FADE_OUT",
};

PRIVATE const char* gcpcLsm = "LSM";

/**
 * @brief gu32LsmCnt is a counter accumulated to control each of LSM periods.
 */
PRIVATE U32 gu32LsmCnt;

/**
 * @brief gu32LsmTusElapsed is holding LSM elapsed time accumulated.
 */
PRIVATE U64 gu32LsmTusElapsed;

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC void vidLsmInit(void* pvArgs) {
  U32 i;

  gu32LsmCnt = (U32)0;
  gu32LsmTusElapsed = 0;

  for (i=0; i<(U32)LSM_TYPE_MAX; i++) {
    gpstrLsmCtrl[i].pcstrLsmCfg = &(gcpstrLsmCfgTbl[i]);
    gpstrLsmCtrl[i].enuSttCur = LSM_STT_OFF;
  }
}

PUBLIC void vidLsmSrvc(void* pvArgs) {
#if 0  
  tenuLsmEvent enuEvent;
  tenuLsmState enuStateNext;
  U32 i;
  
  gu32LsmCnt++;
  for (i=0; i<(U32)LSM_TYPE_MAX; i++) {
    if (gpstrLsmCtrl[i].pcstrLsmCfg->u32Period != (U32)0) {
      if (gu32LsmCnt % gpstrLsmCtrl[i].pcstrLsmCfg->u32Period == (U32)0) {

        /* Get the next state with data passed and event extracted. */
        enuEvent = gpstrLsmCtrl[i].pcstrLsmCfg->pfLsmEventCallback(gpstrLsmCtrl[i].pcstrLsmCfg->enuBsmType, (tenuLsmType)i);
        enuStateNext = gpstrLsmTransTbl[(U32)(gpstrLsmCtrl[i].enuSttCur)][(U32)enuEvent];

        /* Transit states if needed. */
        if (enuStateNext != LSM_STT_NA) {
          vidLsmTransit((tenuLsmType)i, enuStateNext);
        }
        /* Process the do state function. */
        if (gpfLsmSttFtnTbl[(U32)(gpstrLsmCtrl[i].enuSttCur)][(U32)XSM_STT_FTN_DO] != gNULL) {
          gpfLsmSttFtnTbl[(U32)(gpstrLsmCtrl[i].enuSttCur)][(U32)XSM_STT_FTN_DO]((tenuLsmType)i);
        }
      }
    }
  }
#endif
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Private function that transit state on XLM.
 * @param   enuStateNext  The next state to transit.
 * @param   pvArgs        Arguments shall be set if needed.
 * @return  void
 */
PRIVATE void vidLsmTransit(tenuLsmType enuType, tenuLsmState enuStateNext) {
  CH pchTrace[QUE_TRACE_LEN];

  if (enuStateNext != LSM_STT_NA) {
    /* Process the exit state function of the current state. */
    if (gpfLsmSttFtnTbl[(U32)gpstrLsmCtrl[(U32)enuType].enuSttCur][(U32)XSM_STT_FTN_EXIT] != gNULL) {
      gpfLsmSttFtnTbl[(U32)gpstrLsmCtrl[(U32)enuType].enuSttCur][(U32)XSM_STT_FTN_EXIT](enuType);
    }

    /* Transit states. */
    gpstrLsmCtrl[(U32)enuType].enuSttPrev = gpstrLsmCtrl[(U32)enuType].enuSttCur;
    gpstrLsmCtrl[(U32)enuType].enuSttCur = enuStateNext;

    /* Process the entry state function of the next state. */
    if (gpfLsmSttFtnTbl[(U32)gpstrLsmCtrl[(U32)enuType].enuSttCur][(U32)XSM_STT_FTN_ENTRY] != gNULL) {
      gpfLsmSttFtnTbl[(U32)gpstrLsmCtrl[(U32)enuType].enuSttCur][(U32)XSM_STT_FTN_ENTRY](enuType);
    }

    snprintf(pchTrace, QUE_TRACE_LEN, "%s: State changed [%s]->[%s]", \
        gcpcLsm,  \
        gcpcLsmSttNameTbl[gpstrLsmCtrl[(U32)enuType].enuSttPrev], \
        gcpcLsmSttNameTbl[gpstrLsmCtrl[(U32)enuType].enuSttCur]);
    vidQueEnqueue(QUE_TRACE, (void*)pchTrace);
  }
}

/**
 * @brief   Private entry function that is processed when the state is transited to LSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOffEntry(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfLsmOutputCallback(enuType, 0);
}

/**
 * @brief   Private do function that is processed on LSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOffDo(tenuLsmType enuType) {
}

/**
 * @brief   Private exit function that is processed when the state go out from LSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOffExit(tenuLsmType enuType) {
}

/**
 * @brief   Private entry function that is processed when the state is transited to LSM_STT_FADE_IN.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeInEntry(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].u32FadeCnt = (U32)0;
  gpstrLsmCtrl[(U32)enuType].u32FadeDutyDiff = (U32)((U32)31999 - (U32)0);
  gpstrLsmCtrl[(U32)enuType].u32FadeCntMax = (U32)((((U32)gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32FadeInTmo<<10)/(U32)1)>>10);
}

/**
 * @brief   Private do function that is processed on LSM_STT_FADE_IN.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeInDo(tenuLsmType enuType) {
  U16 u16Duty;

  gpstrLsmCtrl[(U32)enuType].u32FadeCnt++;
  if (gpstrLsmCtrl[(U32)enuType].u32FadeCnt >= gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32FadeInTmo)
  {
    vidLsmTransit(enuType, LSM_STT_ON);
  } else {
    u16Duty = (U32)((((gpstrLsmCtrl[(U32)enuType].u32FadeDutyDiff<<10)/gpstrLsmCtrl[(U32)enuType].u32FadeCntMax)*gpstrLsmCtrl[(U32)enuType].u32FadeCnt)>>10);
    gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfLsmOutputCallback(enuType, u16Duty);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from LSM_STT_FADE_IN.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeInExit(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].u32FadeCnt = (U32)0;
}

/**
 * @brief   Private entry function that is processed when the state is transited to LSM_STT_ON.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOnEntry(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfLsmOutputCallback(enuType, 31999);
}

/**
 * @brief   Private do function that is processed on LSM_STT_ON.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOnDo(tenuLsmType enuType) {
}

/**
 * @brief   Private exit function that is processed when the state go out from LSM_STT_ON.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOnExit(tenuLsmType enuType) {
}

/**
 * @brief   Private entry function that is processed when the state is transited to LSM_STT_FADE_OFF.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeOffEntry(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].u32FadeCnt = (U32)0;
  gpstrLsmCtrl[(U32)enuType].u32FadeDutyDiff = (U32)((U32)31999 - (U32)0);
  gpstrLsmCtrl[(U32)enuType].u32FadeCntMax = (U32)((((U32)gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32FadeOutTmo<<10)/(U32)1)>>10);
}

/**
 * @brief   Private do function that is processed on LSM_STT_FADE_OFF.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeOffDo(tenuLsmType enuType) {
  U32 u32Duty;

  gpstrLsmCtrl[(U32)enuType].u32FadeCnt++;
  if (gpstrLsmCtrl[(U32)enuType].u32FadeCnt >= gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32FadeOutTmo)
  {
    gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfLsmOutputCallback(enuType, 0);
  } else {
    u32Duty = (U32)((((gpstrLsmCtrl[(U32)enuType].u32FadeDutyDiff<<10)/gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32FadeOutTmo)*gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32FadeOutTmo)>>10);
    u32Duty = (u32Duty < (U32)31999) ? ((U32)31999 - u32Duty) : (U32)0;
    gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfLsmOutputCallback(enuType, u32Duty);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from LSM_STT_FADE_OFF.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeOffExit(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfLsmOutputCallback(enuType, 0);
}

/* Weak functions -------------------------------------------------- */
/**
 * @brief   A public weak function for fetching LSM event.
 * @param   enuType The LSM type for each of LEDs.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  tenuLsmEvent  LSM event.
 */
PUBLIC __attribute__((weak)) tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuLsmType) {
  return LSM_EVT_NA;
}

/**
 * @brief   A public weak function to output LED.
 * @param   enuType The LSM type for each of LEDs.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  void.
 */
PUBLIC __attribute__((weak)) void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty) {
}

/**
 * @brief   A public weak function for getting the minimum counter period for each of LEDs.
 * @param   enuType The LSM type for each of LEDs.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  U32 The minimum counter period for each of LEDs.
 */
PUBLIC __attribute__((weak)) U32 enuGslLsmPwmMinCallback(tenuLsmType enuType) {
  return (U32)0;
}

/**
 * @brief   A public weak function for getting the maximum counter period for each of LEDs.
 * @param   enuType The LSM type for each of LEDs.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  U32 The maximum counter period for each of LEDs.
 */
PUBLIC __attribute__((weak)) U32 enuGslLsmPwmMaxCallback(tenuLsmType enuType) {
  return (U32)0;
}
