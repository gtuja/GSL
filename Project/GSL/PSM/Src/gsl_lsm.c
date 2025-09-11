/**
 * @file    gsl_lsm.c
 * @brief   This file is used to implement LSM manages LED states.
 * @author  Gtuja
 * @date    Nov 7, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_psm.h"
#include "gsl_lsm.h"
#include "gsl_diag.h"
#include "gsl_queue.h"

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct {
  const tstrLsmCfg* pcstrLsmCfg;        /**< LSM configuration. */
  U32               u32FadeInCnt;       /**< u32FadeInCnt is used for fade in. */
  U32               u32FadeInCntMax;    /**< u32FadeInCntMax is used for fade in. */
  U32               u32FadeInDutyDiff;  /**< u32FadeInDutyDiff is used for fade in. */
  U32               u32FadeOffCnt;      /**< u32FadeOffCnt is used for fade off. */
  U32               u32FadeOffCntMax;   /**< u32FadeOffCntMax is used for fade off. */
  U32               u32FadeOffDutyDiff; /**< u32FadeOffDutyDiff is used  for fade off. */
  tenuLsmState      enuSttCur;          /**< The current LSM state. */
  tenuLsmState      enuSttPrev;         /**< The previous LSM state. */
} tstrLsmControl;

typedef void (*tpfLsmSttFtn)(tenuLsmType enuType);  /** LSM state functions */

/* Private function prototypes -----------------------------------------------*/
PRIVATE void vidLsmTransit(tenuLsmType enuType, tenuLsmState enuStateNext, tenuLsmEvent enuEvent);

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
PRIVATE tstrLsmControl gpstrLsmCtrl[LSM_LED_MAX] = {0};  /** gpstrLsmCtrl is a private variable holding information for BSM. */
PRIVATE tstrLsmCfg gcpstrLsmCfgTbl[LSM_TYPE_MAX] = {0};
#if 0
  /* pcName       u32Period    tenuBsmType       u32FadeInTimeOut  u32FadeOffTimeOut  u32PwmMin        u32PwmMin       pfLsmEventCallback      pfLsmOutputCallback    */
  {  LSM_NAME_L0, LSM_PRD_L0,  LSM_BSM_TYPE_L0,  LSM_FI_TMO_L0,    LSM_FO_TMO_L0,     LSM_PWM_MIN_L0,  LSM_PWM_MAX_L0, enuGslLsmEventCallback, vidGslLsmOutputCallback },  /* LSM_TYPE_LD2_GREEN */
  {  LSM_NAME_L1, LSM_PRD_L1,  LSM_BSM_TYPE_L1,  LSM_FI_TMO_L1,    LSM_FO_TMO_L1,     LSM_PWM_MIN_L1,  LSM_PWM_MAX_L1, enuGslLsmEventCallback, vidGslLsmOutputCallback },  /* Dummy */
  {  LSM_NAME_L2, LSM_PRD_L2,  LSM_BSM_TYPE_L2,  LSM_FI_TMO_L2,    LSM_FO_TMO_L2,     LSM_PWM_MIN_L2,  LSM_PWM_MAX_L2, enuGslLsmEventCallback, vidGslLsmOutputCallback },  /* Dummy */
  {  LSM_NAME_L3, LSM_PRD_L3,  LSM_BSM_TYPE_L3,  LSM_FI_TMO_L3,    LSM_FO_TMO_L3,     LSM_PWM_MIN_L3,  LSM_PWM_MAX_L3, enuGslLsmEventCallback, vidGslLsmOutputCallback },  /* Dummy */
  {  LSM_NAME_L4, LSM_PRD_L4,  LSM_BSM_TYPE_L4,  LSM_FI_TMO_L4,    LSM_FO_TMO_L4,     LSM_PWM_MIN_L4,  LSM_PWM_MAX_L4, enuGslLsmEventCallback, vidGslLsmOutputCallback },  /* Dummy */
};
#endif

/** gpfLsmSttFtnTbl is a private constant table holding LSM state functions. */
PRIVATE const tpfLsmSttFtn gpfLsmSttFtnTbl[LSM_STT_MAX][PSM_STT_FTN_MAX] = {
                          /*  PSM_STT_FTN_ENTRY   PSM_STT_FTN_DO     PSM_STT_FTN_EXIT  */
  /* LSM_STT_NA */        {   gNULL,              gNULL,            gNULL             },
  /* LSM_STT_OFF */       {   vidLsmOffEntry,     vidLsmOffDo,      vidLsmOffExit     },
  /* LSM_STT_FADE_IN */   {   vidLsmFadeInEntry,  vidLsmFadeInDo,   vidLsmFadeInExit  },
  /* LSM_STT_ON */        {   vidLsmOnEntry,      vidLsmOnDo,       vidLsmOnExit      },
  /* LSM_STT_FADE_OFF */  {   vidLsmFadeOffEntry, vidLsmFadeOffDo,  vidLsmFadeOffExit },
};

/** gstrXlmTransitionTbl is a private constant table holding state transition information with each event. */
PRIVATE const tenuLsmState gpstrLsmTransTbl[LSM_STT_MAX][LSM_EVT_MAX] =
{
                          /* LSM_EVT_NA   LSM_EVT_ON        LSM_EVT_OFF       LSM_EVT_FRC_ON  LSM_EVT_FRC_OFF  */
  /* LSM_STT_NA */        {  LSM_STT_NA,  LSM_STT_NA,       LSM_STT_NA,       LSM_STT_NA,     LSM_STT_NA  },
  /* LSM_STT_OFF */       {  LSM_STT_NA,  LSM_STT_FADE_IN,  LSM_STT_NA,       LSM_STT_ON,     LSM_STT_NA  },
  /* LSM_STT_FADE_IN */   {  LSM_STT_NA,  LSM_STT_NA,       LSM_STT_OFF,      LSM_STT_ON,     LSM_STT_OFF },
  /* LSM_STT_ON */        {  LSM_STT_NA,  LSM_STT_NA,       LSM_STT_FADE_OFF, LSM_STT_NA,     LSM_STT_OFF },
  /* LSM_STT_FADE_OFF */  {  LSM_STT_NA,  LSM_STT_NA,       LSM_STT_OFF,      LSM_STT_ON,     LSM_STT_OFF },
};

/**
 * @brief gu32LsmCnt is a counter accumulated to control each of LSM periods.
 */
PRIVATE U32 gu32LsmCnt;

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
  
  vidGslLsmConfigCallback(gcpstrLsmCfgTbl);
  for (i=0; i<(U32)LSM_LED_MAX; i++) {
    gpstrLsmCtrl[i].pcstrLsmCfg = &(gcpstrLsmCfgTbl[i]);
    gpstrLsmCtrl[i].enuSttCur = LSM_STT_OFF;
    gpstrLsmCtrl[i].u32FadeInDutyDiff = gpstrLsmCtrl[i].pcstrLsmCfg->u32PwmMax - gpstrLsmCtrl[i].pcstrLsmCfg->u32PwmMin;
    gpstrLsmCtrl[i].u32FadeInCntMax = (U32)(((gpstrLsmCtrl[i].pcstrLsmCfg->u32FadeInTmo<<(U32)10)/gpstrLsmCtrl[i].pcstrLsmCfg->u32Period)>>(U32)10);
    gpstrLsmCtrl[i].u32FadeOffDutyDiff = gpstrLsmCtrl[i].pcstrLsmCfg->u32PwmMax - gpstrLsmCtrl[i].pcstrLsmCfg->u32PwmMin;
    gpstrLsmCtrl[i].u32FadeOffCntMax = (U32)(((gpstrLsmCtrl[i].pcstrLsmCfg->u32FadeOffTmo<<(U32)10)/gpstrLsmCtrl[i].pcstrLsmCfg->u32Period)>>(U32)10);
  }
}

PUBLIC void vidLsmSrvc(void* pvArgs) {
  tenuLsmEvent enuEvent;
  tenuLsmState enuStateNext;
  U32 i;
  
  gu32LsmCnt++;
  for (i=0; i<(U32)LSM_LED_MAX; i++) {
    if (gpstrLsmCtrl[i].pcstrLsmCfg->u32Period != (U32)0) {
      if (gu32LsmCnt % gpstrLsmCtrl[i].pcstrLsmCfg->u32Period == (U32)0) {

        /* Get the next state with data passed and event extracted. */
        enuEvent = gpstrLsmCtrl[i].pcstrLsmCfg->pfEventCallback(gpstrLsmCtrl[i].pcstrLsmCfg->enuBsmType, (tenuLsmType)i);
        enuStateNext = gpstrLsmTransTbl[(U32)(gpstrLsmCtrl[i].enuSttCur)][(U32)enuEvent];

        /* Transit states if needed. */
        if (enuStateNext != LSM_STT_NA) {
          vidLsmTransit((tenuLsmType)i, enuStateNext, enuEvent);
        }
        /* Process the do state function. */
        if (gpfLsmSttFtnTbl[(U32)(gpstrLsmCtrl[i].enuSttCur)][(U32)PSM_STT_FTN_DO] != gNULL) {
          gpfLsmSttFtnTbl[(U32)(gpstrLsmCtrl[i].enuSttCur)][(U32)PSM_STT_FTN_DO]((tenuLsmType)i);
        }
      }
    }
  }
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Private function that transit state on XLM.
 * @param   enuStateNext  The next state to transit.
 * @param   pvArgs        Arguments shall be set if needed.
 * @return  void
 */
PRIVATE void vidLsmTransit(tenuLsmType enuType, tenuLsmState enuStateNext, tenuLsmEvent enuEvent) {
  if (enuStateNext != LSM_STT_NA) {
    /* Process the exit state function of the current state. */
    if (gpfLsmSttFtnTbl[(U32)gpstrLsmCtrl[(U32)enuType].enuSttCur][(U32)PSM_STT_FTN_EXIT] != gNULL) {
      gpfLsmSttFtnTbl[(U32)gpstrLsmCtrl[(U32)enuType].enuSttCur][(U32)PSM_STT_FTN_EXIT](enuType);
    }

    /* Transit states. */
    gpstrLsmCtrl[(U32)enuType].enuSttPrev = gpstrLsmCtrl[(U32)enuType].enuSttCur;
    gpstrLsmCtrl[(U32)enuType].enuSttCur = enuStateNext;

    /* Process the entry state function of the next state. */
    if (gpfLsmSttFtnTbl[(U32)gpstrLsmCtrl[(U32)enuType].enuSttCur][(U32)PSM_STT_FTN_ENTRY] != gNULL) {
      gpfLsmSttFtnTbl[(U32)gpstrLsmCtrl[(U32)enuType].enuSttCur][(U32)PSM_STT_FTN_ENTRY](enuType);
    }

    vidDiagTracePsmState(PSM_TYPE_LSM,
                         gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pcName,
                         (U32)gpstrLsmCtrl[(U32)enuType].enuSttPrev,
                         (U32)gpstrLsmCtrl[(U32)enuType].enuSttCur,
                         (U32)enuEvent
                        );
  }
}

/**
 * @brief   Private entry function that is processed when the state is transit to LSM_STT_RLS.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOffEntry(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfOutputCallback(enuType, 0);
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
 * @brief   Private entry function that is processed when the state is transit to LSM_STT_FADE_IN.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeInEntry(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].u32FadeInCnt = (U32)0;
}

/**
 * @brief   Private do function that is processed on LSM_STT_FADE_IN.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeInDo(tenuLsmType enuType) {
  U32 u32Duty;

  gpstrLsmCtrl[(U32)enuType].u32FadeInCnt++;
  if (gpstrLsmCtrl[(U32)enuType].u32FadeInCnt >= gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32FadeInTmo)
  {
    vidLsmTransit(enuType, LSM_STT_ON, LSM_EVT_NA);
  } else {
    u32Duty = (U32)((((gpstrLsmCtrl[(U32)enuType].u32FadeInDutyDiff<<(U32)10)/gpstrLsmCtrl[(U32)enuType].u32FadeInCntMax)*gpstrLsmCtrl[(U32)enuType].u32FadeInCnt)>>(U32)10);
    if (u32Duty > gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMax) {
      u32Duty = gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMax;
    } else if (u32Duty < gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMin) {
      u32Duty = gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMin;
    }
    
    gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfOutputCallback(enuType, u32Duty);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from LSM_STT_FADE_IN.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeInExit(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].u32FadeInCnt = (U32)0;
}

/**
 * @brief   Private entry function that is processed when the state is transit to LSM_STT_ON.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmOnEntry(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfOutputCallback(enuType, gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMax);
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
 * @brief   Private entry function that is processed when the state is transit to LSM_STT_FADE_OFF.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeOffEntry(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].u32FadeOffCnt = (U32)0;
}

/**
 * @brief   Private do function that is processed on LSM_STT_FADE_OFF.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeOffDo(tenuLsmType enuType) {
  U32 u32Duty;

  gpstrLsmCtrl[(U32)enuType].u32FadeOffCnt++;
  if (gpstrLsmCtrl[(U32)enuType].u32FadeOffCnt >= gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32FadeOffTmo)
  {
    vidLsmTransit(enuType, LSM_STT_OFF, LSM_EVT_NA);
  } else {
    u32Duty = (U32)((((gpstrLsmCtrl[(U32)enuType].u32FadeOffDutyDiff<<(U32)10)/gpstrLsmCtrl[(U32)enuType].u32FadeOffCntMax)*gpstrLsmCtrl[(U32)enuType].u32FadeOffCnt)>>(U32)10);
    if (u32Duty > gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMax) {
      u32Duty = gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMax;
    } else if (u32Duty < gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMin) {
      u32Duty = gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMin;
    } else {
      u32Duty = gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->u32PwmMax - u32Duty;
    }

    gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfOutputCallback(enuType, u32Duty);
  }
}

/**
 * @brief   Private exit function that is processed when the state go out from LSM_STT_FADE_OFF.
 * @param   pvArgs  Arguments reserved.
 * @return  void
 */
PRIVATE void vidLsmFadeOffExit(tenuLsmType enuType) {
  gpstrLsmCtrl[(U32)enuType].pcstrLsmCfg->pfOutputCallback(enuType, 0);
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
 * @brief   A public weak function for initializing LSM configuration.
 * @param   tstrBsmCfg* The pointer of LSM configuration table.
 * @note    This is an weak function!
 *          UA shall override this and implement device specific features.
 * @return  void
 */
PUBLIC __attribute__((weak)) void vidGslLsmConfigCallback(tstrLsmCfg* pstLsmCfg) {
  return;
}
