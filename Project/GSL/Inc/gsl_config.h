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
EXTERN U32 u32DiagTusCntCallback(void* pvArgs);
EXTERN U32 u32DiagTusCntPrdCallback(void* pvArgs);
EXTERN void vidDiagTraceCallback(char* pcTrace);
#endif /* FEATURE_DIAG */

/* Exported defines ------------------------------------------------ */
#ifdef FEATURE_BSM
#define PSM_PRD_BSM   (U32)1  /* The period of BSM service. */

#if defined FEATURE_L053R8
#define BSM_BTN_MAX   (U32)1  /**< The maximum of buttons managed by BSM. */

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
#elif defined FEATURE_G0B1RE
/* TBD */
#elif defined FEATURE_F429ZI
#define BSM_BTN_MAX   (U32)1  /**< The maximum of buttons managed by BSM. */

#define BSM_NAME_B0   (const char*)"B1(B)" /**< B1_BLUE PC13 on F429ZI. */
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
#endif

typedef tenuBsmEvent (*tpfBsmEventCallback)(tenuBsmType enuType);
EXTERN tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);
#endif /* FEATURE_BSM */

#ifdef FEATURE_LSM
#define PSM_PRD_LSM   (U32)1  /* The period of LSM service. */

#if defined FEATURE_L053R8
#define LSM_LED_MAX   (U32)1  /**< The maximum of buttons managed by BSM. */

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

#define LSM_PWM_MIN_L0   (U32)0
#define LSM_PWM_MIN_L1   (U32)0
#define LSM_PWM_MIN_L2   (U32)0
#define LSM_PWM_MIN_L3   (U32)0
#define LSM_PWM_MIN_L4   (U32)0

#define LSM_PWM_MAX_L0   (U32)31999  /**< LD2_GREEN PA5 on L053R8TX. */
#define LSM_PWM_MAX_L1   (U32)0
#define LSM_PWM_MAX_L2   (U32)0
#define LSM_PWM_MAX_L3   (U32)0
#define LSM_PWM_MAX_L4   (U32)0

#elif defined FEATURE_G0B1RE
/* TBD */
#elif defined FEATURE_F429ZI
#define LSM_LED_MAX   (U32)3  /**< The maximum of buttons managed by BSM. */

#define LSM_NAME_L0   (const char*)"LD1(G)" /**< LD1(G) PB0, TIM3_CH3 on F429ZI. */
#define LSM_NAME_L1   (const char*)"LD2(B)" /**< LD2(B) PB7, TIM4_CH2 on F429ZI. */
#define LSM_NAME_L2   (const char*)"LD3(R)" /**< LD3(R) PB14, TIM12_CH1 on F429ZI. */
#define LSM_NAME_L3   (const char*)""
#define LSM_NAME_L4   (const char*)""

#define LSM_PRD_L0    (U32)1  /**< LD1(G) PB0, TIM3_CH3 on F429ZI. */
#define LSM_PRD_L1    (U32)1  /**< LD2(B) PB7, TIM4_CH2 on F429ZI. */
#define LSM_PRD_L2    (U32)1  /**< LD3(R) PB14, TIM12_CH1 on F429ZI. */
#define LSM_PRD_L3    (U32)0
#define LSM_PRD_L4    (U32)0

#define LSM_BSM_TYPE_L0 BSM_TYPE_B0 /**< B(B) PC13 on F429ZI. */
#define LSM_BSM_TYPE_L1 BSM_TYPE_B0 /**< B(B) PC13 on F429ZI. */
#define LSM_BSM_TYPE_L2 BSM_TYPE_B0 /**< B(B) PC13 on F429ZI. */
#define LSM_BSM_TYPE_L3 BSM_TYPE_B0 /**< Dummy */
#define LSM_BSM_TYPE_L4 BSM_TYPE_B0 /**< Dummy */

#define LSM_FI_TMO_L0 (U32)1000 /**< LD1(G) PB0, TIM3_CH3 on F429ZI. */
#define LSM_FI_TMO_L1 (U32)500  /**< LD2(B) PB7, TIM4_CH2 on F429ZI. */
#define LSM_FI_TMO_L2 (U32)100  /**< LD3(R) PB14, TIM12_CH1 on F429ZI. */
#define LSM_FI_TMO_L3 (U32)0
#define LSM_FI_TMO_L4 (U32)0

#define LSM_FO_TMO_L0 (U32)2000 /**< LD1(G) PB0, TIM3_CH3 on F429ZI. */
#define LSM_FO_TMO_L1 (U32)1000 /**< LD2(B) PB7, TIM4_CH2 on F429ZI. */
#define LSM_FO_TMO_L2 (U32)200  /**< LD3(R) PB14, TIM12_CH1 on F429ZI. */
#define LSM_FO_TMO_L3 (U32)0
#define LSM_FO_TMO_L4 (U32)0

#define LSM_PWM_MIN_L0   (U32)0
#define LSM_PWM_MIN_L1   (U32)0
#define LSM_PWM_MIN_L2   (U32)0
#define LSM_PWM_MIN_L3   (U32)0
#define LSM_PWM_MIN_L4   (U32)0

#define LSM_PWM_MAX_L0   (U32)16799 /**< LD1(G) PB0, TIM3_CH3 on F429ZI. */
#define LSM_PWM_MAX_L1   (U32)16799 /**< LD2(B) PB7, TIM4_CH2 on F429ZI. */
#define LSM_PWM_MAX_L2   (U32)16799 /**< LD3(R) PB14, TIM12_CH1 on F429ZI. */
#define LSM_PWM_MAX_L3   (U32)0
#define LSM_PWM_MAX_L4   (U32)0
#endif

typedef tenuLsmEvent (*tpfLsmEventCallback)(tenuBsmType enuBsmType, tenuLsmType enuType);
EXTERN tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuType);
typedef void (*tpfLsmOutputCallback)(tenuLsmType enuType, U32 u32PwmDuty);
EXTERN void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty);

#endif /* FEATURE_LSM */

/* Exported functions ---------------------------------------------- */
/* Exported variables ---------------------------------------------- */

#endif /* GSL_CONFIG_H_ */
