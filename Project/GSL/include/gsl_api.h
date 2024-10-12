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

typedef struct {
  S32 s32Handle;
} tstrGslBsmExtractEventArgs;

typedef U32   (*tpfGslTickCallback)(void* pvArgs);
typedef void  (*tpfGslLsmOutput)(U32 u32PwmDuty);
typedef void  (*tpfGslPsmServiceInitialize)(void* pvArgs);
typedef void  (*tpfGslPsmService)(void* pvArgs);

typedef enum {
  GSL_SRVC_BSM = 0,
  GSL_SRVC_LSM,
  GSL_TSK_IDLE,
} tenuGslType;

typedef struct {
  const CH*           pchName;
  U32                 u32TickCounterPeriod;
  tpfGslTickCallback  pfGslTickCallback;
} tstrGslInitializeArgs;

typedef struct {
  tenuGslType enuType;
  void*       pvArgs;
} tstrGslRegisterArgs;

typedef struct {
  const CH            pchName[GSL_NAME_LEN];

} tstrGslNoosServiceArgs;

typedef BOOL (*tpfGslBsmExtractEventCallback)(S32 s32Handle);

typedef struct {
  U32 u32Period;                                              /**< The cycle of execution. */
  U32 u32MatchCount;                                          /**< The match count of button for chattering prevention. */
  U32 u32PressedThreshHold;                                   /**< The threshold to distinguish short-press from long-press. */
  tpfGslBsmExtractEventCallback pfGslBsmExtractEventCallback; /**< Callback for retrieving button is pressed or not. */
} tstrGslBsmRegisterArgs;

typedef struct {
  tstrGslInitializeArgs strGslInitializeArgs;
} tstrGslBtmInitializeArgs;

typedef struct {
  tstrGslInitializeArgs strGslInitializeArgs;
} tstrGslBsmInitializeArgs;

typedef struct {
  tstrGslInitializeArgs strGslInitializeArgs;
} tstrGslLsmInitializeArgs;

typedef struct {
  U32             u32HandleLsm;
  U32             u32HandleBsm;
  U32             u32Cycle;
  U32             u32PwmDutyMin;
  U32             u32PwmDutyMax;
  U32             u32FadeInTimeOut;
  U32             u32FadeOffTimeOut;
  tpfGslLsmOutput pfGslLsmOutput;
} tstrGslLsmRegisterArgs;

/* Exported functions prototypes ---------------------------------------------*/
EXTERN void vidGslInitialize(tstrGslInitializeArgs* pstrArgs);
EXTERN S32  s32GslRegister(tstrGslRegisterArgs* pstrArgs);
EXTERN void vidGslService(void* pvArgs);
EXTERN void vidGslProcess(void* pvArgs);

#endif /* GSL_API_H_ */
