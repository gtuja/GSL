/**
 * @file    gsl_config.h
 * @brief   gsl_config.h contains contains defines for each modules
 *          in the GSL to satisfy UA requirements.  
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef GSL_CONFIG_H_
#define GSL_CONFIG_H_

/* Includes -------------------------------------------------------- */
#include "gsl_def.h"
#include "gsl_feature.h"

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
#endif /* FEATURE_LSM */

/* Application callback -------------------------------------------- */

#ifdef FEATURE_DIAG
typedef U32 (*tpfDiagCntCallback)(void* pvArgs);
EXTERN U32 u32DiagCntCallback(void* pvArgs);
typedef U32 (*tpfDiagCntPrdCallback)(void* pvArgs);
EXTERN U32 u32DiagCntPrdCallback(void* pvArgs);
typedef void (*tpfDiagTraceCallback)(char* pcTrace);
EXTERN void vidDiagTraceCallback(char* pcTrace);
#endif /* FEATURE_DIAG */

/* Exported defines ------------------------------------------------ */
#ifdef FEATURE_BSM
#define PSM_PRD_BSM   (U32)1    /* The period of BSM service. */

#define BSM_NAME_B0   (const char*)"B1_BLUE" /**< B1_BLUE PC13 on L053R8TX. */
#define BSM_NAME_B1   (const char*)""
#define BSM_NAME_B2   (const char*)""
#define BSM_NAME_B3   (const char*)""
#define BSM_NAME_B4   (const char*)""

#define BSM_PRD_B0    (U32)1  /**< B1_BLUE PC13 on L053R8TX. */
#define BSM_PRD_B1    (U32)0
#define BSM_PRD_B2    (U32)0
#define BSM_PRD_B3    (U32)0
#define BSM_PRD_B4    (U32)0

#define BSM_MCCP_B0   (U32)5    /* The match count of chattering prevention for buttons. */
#define BSM_MCCP_B1   (U32)5    /* The match count of chattering prevention for buttons. */
#define BSM_MCCP_B2   (U32)5    /* The match count of chattering prevention for buttons. */
#define BSM_MCCP_B3   (U32)5    /* The match count of chattering prevention for buttons. */
#define BSM_MCCP_B4   (U32)5    /* The match count of chattering prevention for buttons. */

#define BSM_THN_B0    (U32)1000 /* The threshold between short and long press on buttons. */
#define BSM_THN_B1    (U32)1000 /* The threshold between short and long press on buttons. */
#define BSM_THN_B2    (U32)1000 /* The threshold between short and long press on buttons. */
#define BSM_THN_B3    (U32)1000 /* The threshold between short and long press on buttons. */
#define BSM_THN_B4    (U32)1000 /* The threshold between short and long press on buttons. */

typedef tenuBsmEvent (*tpfBsmEventCallback)(tenuBsmType enuType);
EXTERN tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);
#endif /* FEATURE_BSM */

#ifdef FEATURE_LSM
#define PSM_PRD_LSM   (U32)1  /* The period of LSM service. */

#define LSM_NAME_L0   (const char*)"LD2_GREEN" /**< LD2_GREEN PA5 on L053R8TX. */
#define LSM_NAME_L1   (const char*)""
#define LSM_NAME_L2   (const char*)""
#define LSM_NAME_L3   (const char*)""
#define LSM_NAME_L4   (const char*)""

#define LSM_PRD_L0    (U32)1  /**< LD2_GREEN PA5 on L053R8TX. */
#define LSM_PRD_L1    (U32)0
#define LSM_PRD_L2    (U32)0
#define LSM_PRD_L3    (U32)0
#define LSM_PRD_L4    (U32)0

#define LSM_BSM_TYPE_L0 BSM_TYPE_B0 /**< B1_BLUE PC13 on L053R8TX. */
#define LSM_BSM_TYPE_L1 BSM_TYPE_B0 /**< Dummy */
#define LSM_BSM_TYPE_L2 BSM_TYPE_B0 /**< Dummy */
#define LSM_BSM_TYPE_L3 BSM_TYPE_B0 /**< Dummy */
#define LSM_BSM_TYPE_L4 BSM_TYPE_B0 /**< Dummy */

#define LSM_FI_TMO_L0 (U32)1000 /**< LD2_GREEN PA5 on L053R8TX. */
#define LSM_FI_TMO_L1 (U32)0
#define LSM_FI_TMO_L2 (U32)0
#define LSM_FI_TMO_L3 (U32)0
#define LSM_FI_TMO_L4 (U32)0

#define LSM_FO_TMO_L0 (U32)2000 /**< LD2_GREEN PA5 on L053R8TX. */
#define LSM_FO_TMO_L1 (U32)0
#define LSM_FO_TMO_L2 (U32)0
#define LSM_FO_TMO_L3 (U32)0
#define LSM_FO_TMO_L4 (U32)0

typedef tenuLsmEvent (*tpfLsmEventCallback)(tenuBsmType enuBsmType, tenuLsmType enuType);
EXTERN tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuType);
typedef void (*tpfLsmOutputCallback)(tenuLsmType enuType, U32 u32PwmDuty);
EXTERN void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty);
typedef U32 (*tpfLsmPwmMinCallback)(tenuLsmType enuType);
EXTERN U32 enuGslLsmPwmMinCallback(tenuLsmType enuType);
typedef U32 (*tpfLsmPwmMaxCallback)(tenuLsmType enuType);
EXTERN U32 enuGslLsmPwmMaxCallback(tenuLsmType enuType);

#endif /* FEATURE_LSM */

#ifdef FEATURE_DSM
#define PSM_PRD_DSM   (U32)1 /* The period of DSM service. */
#endif /* FEATURE_DSM */

/* Exported functions ---------------------------------------------- */
/* Exported variables ---------------------------------------------- */

#endif /* GSL_CONFIG_H_ */
