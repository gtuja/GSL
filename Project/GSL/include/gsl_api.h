/**
 * @file    gsl_api.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 9, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GSL_API_H_
#define GSL_API_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"

/* Exported defines ----------------------------------------------------------*/
#define GSL_NAME_LEN  (U32)31
#define GSL_HNDL_NA   (S32)(-1)

/* Exported types ------------------------------------------------------------*/
typedef enum {
  BSM_EVT_NA = 0, /**< BSM event not available. */
  BSM_EVT_RLS,    /**< BSM event that button is released. */
  BSM_EVT_PSH,    /**< BSM event that button is pushed. */
  BSM_EVT_MAX,    /**< Maximum BSM event. */
} tenuBsmEvent;

typedef enum {
  BSM_EVT_NTF_NA = 0, /**< BSM notification not available. */
  BSM_EVT_NTF_SHORT,  /**< BSM notification short press. */
  BSM_EVT_NTF_LONG,   /**< BSM notification long press. */
  BSM_EVT_NTF_MAX,    /**< BSM notification maximum. */
} tenuBsmEventNotify;

typedef enum {
  BSM_TYPE_B1_BLUE = 0, /**< BSM type for B1_BLUE button on the Nucleo-L053R8. */
  BSM_TYPE_MAX,         /**< BSM type maximum. */
} tenuBsmType;

typedef enum {
  LSM_EVT_NA = 0,   /**< LSM event not available. */
  LSM_EVT_REQ_ON,   /**< LSM request on is raised, when the LED is off. */
  LSM_EVT_REQ_OFF,  /**< LSM request off is raised, when the LED is on. */
  LSM_EVT_ON_FRC,   /**< LSM request force on is raised, when the LED is off. */
  LSM_EVT_OFF_FRC,  /**< LSM request force off is raised, when the LED is on. */
  LSM_EVT_MAX,      /**< Maximum LSM event. */
} tenuLsmEvent;

typedef enum {
  LSM_STT_NA = 0,   /**< LSM state, not available. */
  LSM_STT_OFF,      /**< LSM state, LED is off. */
  LSM_STT_FADE_IN,  /**< LSM state, LED is under fade in. */
  LSM_STT_ON,       /**< LSM state, LED is on. */
  LSM_STT_FADE_OUT, /**< LSM state, LED is under fade out. */
  LSM_STT_MAX,      /**< LSM maximum state. */
} tenuLsmState;

typedef enum {
  LSM_TYPE_LD2_GREEN = 0, /**< LSM type for B2_GREEN LED on the Nucleo-L053R8. */
  LSM_TYPE_MAX,           /**< BSM type maximum. */
} tenuLsmType;

typedef enum {
  PSM_TYPE_CLK = 0, /**< PSM type : clock service. */
  PSM_TYPE_BSM,     /**< PSM type : BSM service. */
  PSM_TYPE_LSM,     /**< PSM type : LSM service. */
  PSM_TYPE_MAX,     /**< PSM type maximum. */
} tenuPsmType;

typedef void (*tpfPsmService)(void* pvArgs);
typedef struct {
  U32           u32Period;    /**< PSM period. */
  tpfPsmService pfPsmService; /**< PSM service. */
} tstrPsmCfg;

typedef enum {
  BTM_TYPE_IDLE = 0,  /**< BTM type : idle processing. */
  BTM_TYPE_TRACE,     /**< BTM type : trace processing. */
  BTM_TYPE_MAX,       /**< BTM type maximum. */
} tenuBtmType;

typedef void (*tpfBtmProcess)(void* pvArgs);
typedef struct {
  tpfBtmProcess pfBtmProcess; /**< BTM process. */
} tstrBtmCfg;

typedef tenuBsmEvent (*tpfBsmEventCallback)(tenuBsmType enuType);
typedef struct {
  U32                 u32Period;              /**< The cycle of execution. */
  U32                 u32MatchCount;          /**< The match count of same button state for chattering prevention. */
  U32                 u32PressedThreshHold;   /**< The threshold for fetching tenuBsmEventNotify, short or long press. */
  tpfBsmEventCallback pfBsmEventCallback;     /**< Callback for retrieving tenuBsmEvent. */
} tstrBsmCfg;

typedef tenuLsmEvent (*tpfLsmEventCallback)(tenuBsmType enuBsmType, tenuLsmType enuLsmType);
typedef void (*tpfLsmOutputCallback)(tenuLsmType enuType, U32 u32PwmDuty);
typedef struct {
  tenuBsmType enuBsmType;                   /**< BSM linked with LSM */
  U32 u32Period;                            /**< The periodic cycle for fading. */
  U32 u32FadeInTimeOut;                     /**< The fade in timeout. */
  U16 u32FadeOutTimeOut;                    /**< The fade out timeout. */
  tpfLsmEventCallback pfLsmEventCallback;   /**< Callback for retrieving tenuLlmState. */
  tpfLsmOutputCallback pfLsmOutputCallback; /**< Callback for LED output. */
} tstrLsmCfg;

typedef U32 (*tpfGslTickCountCallback)(void* pvArgs);
typedef U32 (*tpfGslTickPeriodCallback)(void* pvArgs);
typedef void (*tpfGslTraceCallback)(char* pcTrace);
typedef struct {
  tpfGslTickCountCallback   pfGslTickCountCallback;
  tpfGslTickPeriodCallback  pfGslTickPeriodCallback;
  tpfGslTraceCallback       pfGslTraceCallback;
} tstrGslCallbacks;

/* Exported functions ---------------------------------------------- */
PUBLIC void vidGslInitialize(void* pvArgs);
PUBLIC void vidGslService(void* pvArgs);
PUBLIC void vidGslProcess(void* pvArgs);

PUBLIC U32 u32GslTickCountCallback(void *pvArgs);
PUBLIC U32 u32GslTickPeriodCallback(void *pvArgs);
PUBLIC tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);
PUBLIC tenuBsmEventNotify enuGslBsmGetEventNotify(tenuBsmType enuType);

/* Exported variables ---------------------------------------------- */

EXTERN const tstrPsmCfg gcpstrPsmCfgTbl[PSM_TYPE_MAX];
EXTERN const tstrBtmCfg gcpstrBtmCfgTbl[BTM_TYPE_MAX];
EXTERN const tstrBsmCfg gcpstrBsmCfgTbl[BSM_TYPE_MAX];
EXTERN const tstrLsmCfg gcpstrLsmCfgTbl[LSM_TYPE_MAX];
EXTERN const tstrGslCallbacks gcstrGslCalbacks;
#endif /* GSL_API_H_ */
