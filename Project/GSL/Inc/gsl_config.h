/**
 * @file    gsl_config.h
 * @brief   gsl_config.h contains contains defines for each modules
 *          in the GSL to satisfy UA requirements.  
 * @author  Gtuja
 * @date    Nov 7, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef GSL_CONFIG_H_
#define GSL_CONFIG_H_

/* Includes -------------------------------------------------------- */
#include "gsl_def.h"
#include "gsl_feature.h"

/* Exported defines ------------------------------------------------ */
#ifdef FEATURE_BSM
#define PSM_PRD_BSM   (U32)1  /* The period of BSM service. */
#define BSM_BTN_MAX   (U32)5  /**< The maximum of buttons managed by BSM. */
#endif /* FEATURE_BSM */

#ifdef FEATURE_LSM
#define PSM_PRD_LSM   (U32)1  /* The period of LSM service. */
#define LSM_LED_MAX   (U32)5  /**< The maximum of LEDs managed by LSM. */
#endif /* FEATURE_LSM */

/* Exported types -------------------------------------------------- */
#ifdef FEATURE_BSM
typedef enum {
  BSM_EVT_NA = 0, /**< BSM event not available. */
  BSM_EVT_RLS,    /**< BSM event that button is released. */
  BSM_EVT_PSH,    /**< BSM event that button is pushed. */
  BSM_EVT_MAX,    /**< Maximum BSM event. */
} tenuBsmEvent;

typedef enum {
  BSM_NTF_NA = 0, /**< BSM event notified not available. */
  BSM_NTF_SHORT,  /**< BSM event notified that is short pressed. */
  BSM_NTF_LONG,   /**< BSM event notified that is long pressed. */
  BSM_NTF_MAX,    /**< Maximum BSM event notified. */
} tenuBsmNotify;

typedef enum {
  BSM_TYPE_B0 = 0,
  BSM_TYPE_B1,
  BSM_TYPE_B2,
  BSM_TYPE_B3,
  BSM_TYPE_B4,
  BSM_TYPE_MAX,
} tenuBsmType;

typedef tenuBsmEvent (*tpfBsmEventCallback)(tenuBsmType enuType);
EXTERN tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);
typedef struct {
  const CH*           pcName;               /**< The name of BSM service. */
  U32                 u32Period;            /**< The cycle of execution. */
  U32                 u32MCCP;              /**< The match count of same button state for chattering prevention. */
  U32                 u32PressedThreshHold; /**< The threshold for fetching tenuBsmEventNotify, short or long press. */
  tpfBsmEventCallback pfBsmEventCallback;   /**< Callback for retrieving tenuBsmEvent. */
} tstrBsmCfg;

EXTERN void vidGslBsmConfigCallback(tstrBsmCfg* pstBsmCfg);
#endif /* FEATURE_BSM */

#ifdef FEATURE_LSM
typedef enum {
  LSM_TYPE_L0 = 0,
  LSM_TYPE_L1,
  LSM_TYPE_L2,
  LSM_TYPE_L3,
  LSM_TYPE_L4,
  LSM_TYPE_MAX,
} tenuLsmType;

typedef enum {
  LSM_EVT_NA = 0,   /**< LSM event not available. */
  LSM_EVT_ON,       /**< LSM event that requests ON. */
  LSM_EVT_OFF,      /**< LSM event that requests OFF. */
  LSM_EVT_FRC_ON,   /**< LSM event that requests FORCE ON. */
  LSM_EVT_FRC_OFF,  /**< LSM event that requests FORCE OFF. */
  LSM_EVT_MAX,      /**< Maximum LSM event. */
} tenuLsmEvent;

typedef tenuLsmEvent (*tpfLsmEventCallback)(tenuBsmType enuBsmType, tenuLsmType enuType);
EXTERN tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuType);
typedef void (*tpfLsmOutputCallback)(tenuLsmType enuType, U32 u32PwmDuty);
EXTERN void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty);

typedef struct {
  const CH*             pcName;           /**< The name of LSM service. */
  U32                   u32Period;        /**< The periodic cycle for LSM. */
  tenuBsmType           enuBsmType;       /**< BSM linked with LSM */
  U32                   u32FadeInTmo;     /**< The fade in timeout. */
  U32                   u32FadeOffTmo;    /**< The fade off timeout. */
  U32                   u32PwmMin;        /**< The minimum PWM duty. */
  U32                   u32PwmMax;        /**< The maximum PWM duty. */
  tpfLsmEventCallback   pfEventCallback;  /**< Callback for retrieving tenuLlmState. */
  tpfLsmOutputCallback  pfOutputCallback; /**< Callback for LED output. */
} tstrLsmCfg;

EXTERN void vidGslLsmConfigCallback(tstrLsmCfg* pstLsmCfg);
#endif /* FEATURE_LSM */
/* Application callback -------------------------------------------- */

#ifdef FEATURE_DIAG
EXTERN U32 u32DiagTusCntCallback(void* pvArgs);
EXTERN U32 u32DiagTusCntPrdCallback(void* pvArgs);
EXTERN void vidDiagTraceCallback(char* pcTrace);
#endif /* FEATURE_DIAG */

/* Exported functions ---------------------------------------------- */
/* Exported variables ---------------------------------------------- */

#endif /* GSL_CONFIG_H_ */
