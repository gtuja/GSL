/**
 * @file    gsl_lsm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_api.h"
#include "gsl.h"
#include "gsl_lsm.h"
#include "gsl_diag.h"
#include "gsl_queue.h"
#include <string.h>
#include <stdio.h>

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

typedef void (*tpfLsmSttFtn)(tenuLsmType enuType);  /** LSM state functions */
typedef struct {
  tpfLsmSttFtn pfEntry;  /**< BSM state function, entry. */
  tpfLsmSttFtn pfDo;     /**< BSM state function, do. */
  tpfLsmSttFtn pfExit;   /**< BSM state function, exit. */
} tstrLsmSttFtn;

typedef struct {
  const tstrLsmCfg*   pcstrLsmConfig;     /**< LSM configuration. */
  U32                 u32FadeCounter;     /**< u32MatchCounter is used for chattering prevention within XLM state machine. */
  U32                 u32FadeCounterMax;  /**< u32MatchCounter is used for chattering prevention within XLM state machine. */
  U32                 u32FadeDutyDiff;    /**< u32MatchCounter is used for chattering prevention within XLM state machine. */
  tenuLsmState        enuStateCurrent;    /**< The current LSM state. */
  tenuLsmState        enuStatePrevious;   /**< The previous LSM state. */
  tenuBsmEventNotify  enuEventCurrent;    /**< The current BSM notify event. */
  tenuBsmEventNotify  enuEventPrevious;   /**< The previous BSM notify event. */
} tstrLsmControl;

typedef struct {
  U32 u32TimeProcTotal;
  U32 u32TimeProcMax;
} tstrLsmDiag;


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
PRIVATE tstrLsmControl gpstrControl[LSM_TYPE_MAX] = {0};  /** gpstrControl is a private variable holding information for BSM. */
PRIVATE tstrLsmDiag gpstrLsmDiagProc[LSM_TYPE_MAX] = {0};
PRIVATE tstrLsmDiag gpstrLsmDiag = {0};

/** gpfLsmSttFtnTbl is a private constant table holding LSM state functions. */
PRIVATE const tpfLsmSttFtn gpfLsmSttFtnTbl[LSM_STT_MAX][XSM_STT_FTN_MAX] = {
                          /*  LSM_STT_FTN_ENTRY   LM_STT_FTN_DO     LSM_STT_FTN_EXIT  */
  /* LSM_STT_NA */        {   NULL,               NULL,             NULL              },
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

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC void vidLsmInitialize(void* pvArgs) {
  U32 i;

  memset(gpstrControl, 0, sizeof(gpstrControl));
  memset(gpstrLsmDiagProc, 0, sizeof(gpstrLsmDiagProc));
  memset(&gpstrLsmDiag, 0, sizeof(gpstrLsmDiag));

  for (i=0; i<(U32)LSM_TYPE_MAX; i++) {
    gpstrControl[i].pcstrLsmConfig = &(gcpstrLsmCfgTbl[i]);
    gpstrControl[i].enuStateCurrent = LSM_STT_OFF;
  }
}

PUBLIC void vidLsmService(void* pvArgs) {
  tenuLsmEvent enuEvent;
  tenuLsmState enuStateNext;
  U32 i;
  U32 u32TimeElapsed;

  for (i=0; i<(U32)LSM_TYPE_MAX; i++) {
    vidGslDiagTimeStart();

    /* Get the next state with data passed and event extracted. */
    enuEvent = gpstrControl[i].pcstrLsmConfig->pfLsmEventCallback(gpstrControl[i].pcstrLsmConfig->enuBsmType, (tenuLsmType)i);
    enuStateNext = gpstrLsmTransTbl[(U32)(gpstrControl[i].enuStateCurrent)][(U32)enuEvent];

    /* Transit states if needed. */
    if (enuStateNext != LSM_STT_NA) {
      vidLsmTransit((tenuLsmType)i, enuStateNext);
    }
    /* Process the do state function. */
    if (gpfLsmSttFtnTbl[(U32)(gpstrControl[i].enuStateCurrent)][(U32)XSM_STT_FTN_DO] != NULL) {
      gpfLsmSttFtnTbl[(U32)(gpstrControl[i].enuStateCurrent)][(U32)XSM_STT_FTN_DO]((tenuLsmType)i);
    }

    u32TimeElapsed = u32GslDiagTimeElapsed();
    if (u32TimeElapsed > gpstrLsmDiagProc[i].u32TimeProcMax) {
      gpstrLsmDiagProc[i].u32TimeProcMax = u32TimeElapsed;
    }
    gpstrLsmDiagProc[i].u32TimeProcTotal += u32TimeElapsed;
  }

  for (i=0; i<(U32)LSM_TYPE_MAX; i++) {
    if (gpstrLsmDiagProc[i].u32TimeProcMax > gpstrLsmDiag.u32TimeProcMax) {
      gpstrLsmDiag.u32TimeProcMax = gpstrLsmDiagProc[i].u32TimeProcMax;
    }
    gpstrLsmDiag.u32TimeProcTotal += gpstrLsmDiagProc[i].u32TimeProcTotal;
  }
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Private function that transit state on XLM.
 * @param   enuStateNext  The next state to transit.
 * @param   pvArgs        Arguments shall be set if needed.
 * @return  void
 */
PRIVATE void vidLsmTransit(tenuLsmType enuType, tenuLsmState enuStateNext) {
  CH pchTrace[GSL_TRACE_MAX];

  if (enuStateNext != LSM_STT_NA) {
    /* Process the exit state function of the current state. */
    if (gpfLsmSttFtnTbl[(U32)gpstrControl[(U32)enuType].enuStateCurrent][(U32)XSM_STT_FTN_EXIT] != NULL) {
      gpfLsmSttFtnTbl[(U32)gpstrControl[(U32)enuType].enuStateCurrent][(U32)XSM_STT_FTN_EXIT](enuType);
    }

    /* Transit states. */
    gpstrControl[(U32)enuType].enuStatePrevious = gpstrControl[(U32)enuType].enuStateCurrent;
    gpstrControl[(U32)enuType].enuStateCurrent = enuStateNext;

    /* Process the entry state function of the next state. */
    if (gpfLsmSttFtnTbl[(U32)gpstrControl[(U32)enuType].enuStateCurrent][(U32)XSM_STT_FTN_ENTRY] != NULL) {
      gpfLsmSttFtnTbl[(U32)gpstrControl[(U32)enuType].enuStateCurrent][(U32)XSM_STT_FTN_ENTRY](enuType);
    }

    snprintf(pchTrace, GSL_TRACE_MAX, "%s: State changed [%s]->[%s]", \
        gcpcLsm,  \
        gcpcLsmSttNameTbl[gpstrControl[(U32)enuType].enuStatePrevious], \
        gcpcLsmSttNameTbl[gpstrControl[(U32)enuType].enuStateCurrent]);
    vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
  }
}

/**
 * @brief   Private entry function that is processed when the state is transited to LSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOffEntry(tenuLsmType enuType) {
  gpstrControl[(U32)enuType].pcstrLsmConfig->pfLsmOutputCallback(enuType, 0);
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
  gpstrControl[(U32)enuType].u32FadeCounter = (U32)0;
  gpstrControl[(U32)enuType].u32FadeDutyDiff = (U32)((U32)31999 - (U32)0);
  gpstrControl[(U32)enuType].u32FadeCounterMax = (U32)((((U32)gpstrControl[(U32)enuType].pcstrLsmConfig->u32FadeInTimeOut<<10)/(U32)1)>>10);
}

/**
 * @brief   Private do function that is processed on LSM_STT_FADE_IN.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeInDo(tenuLsmType enuType) {
  U16 u16Duty;

  gpstrControl[(U32)enuType].u32FadeCounter++;
  if (gpstrControl[(U32)enuType].u32FadeCounter >= gpstrControl[(U32)enuType].pcstrLsmConfig->u32FadeInTimeOut)
  {
    vidLsmTransit(enuType, LSM_STT_ON);
  } else {
    u16Duty = (U32)((((gpstrControl[(U32)enuType].u32FadeDutyDiff<<10)/gpstrControl[(U32)enuType].u32FadeCounterMax)*gpstrControl[(U32)enuType].u32FadeCounter)>>10);
    gpstrControl[(U32)enuType].pcstrLsmConfig->pfLsmOutputCallback(enuType, u16Duty);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from LSM_STT_FADE_IN.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeInExit(tenuLsmType enuType) {
  gpstrControl[(U32)enuType].u32FadeCounter = (U32)0;
}

/**
 * @brief   Private entry function that is processed when the state is transited to LSM_STT_ON.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOnEntry(tenuLsmType enuType) {
  gpstrControl[(U32)enuType].pcstrLsmConfig->pfLsmOutputCallback(enuType, 31999);
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
  gpstrControl[(U32)enuType].u32FadeCounter = (U32)0;
  gpstrControl[(U32)enuType].u32FadeDutyDiff = (U32)((U32)31999 - (U32)0);
  gpstrControl[(U32)enuType].u32FadeCounterMax = (U32)((((U32)gpstrControl[(U32)enuType].pcstrLsmConfig->u32FadeInTimeOut<<10)/(U32)1)>>10);
}

/**
 * @brief   Private do function that is processed on LSM_STT_FADE_OFF.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeOffDo(tenuLsmType enuType) {
  U32 u32Duty;

  gpstrControl[(U32)enuType].u32FadeCounter++;
  if (gpstrControl[(U32)enuType].u32FadeCounter >= gpstrControl[(U32)enuType].pcstrLsmConfig->u32FadeOutTimeOut)
  {
    gpstrControl[(U32)enuType].pcstrLsmConfig->pfLsmOutputCallback(enuType, 0);
  } else {
    u32Duty = (U32)((((gpstrControl[(U32)enuType].u32FadeDutyDiff<<10)/gpstrControl[(U32)enuType].pcstrLsmConfig->u32FadeOutTimeOut)*gpstrControl[(U32)enuType].pcstrLsmConfig->u32FadeOutTimeOut)>>10);
    u32Duty = (u32Duty < (U32)31999) ? ((U32)31999 - u32Duty) : (U32)0;
    gpstrControl[(U32)enuType].pcstrLsmConfig->pfLsmOutputCallback(enuType, u32Duty);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from LSM_STT_FADE_OFF.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeOffExit(tenuLsmType enuType) {
  gpstrControl[(U32)enuType].pcstrLsmConfig->pfLsmOutputCallback(enuType, 0);
}
