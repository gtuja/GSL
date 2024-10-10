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
/* Exported types ------------------------------------------------------------*/
typedef void (*tpfAppTraceCallback)(char* pcTrace);
typedef U32 (*tpfAppTickCallback)(void);

typedef struct {
  tpfAppTraceCallback pfTrace;
  tpfAppTickCallback  pfTick;
} tstrAppCallbacks;

typedef struct {
  tstrAppCallbacks  strAppCallbacks;
} tstrGslInitializeArgs;

/* Exported functions prototypes ---------------------------------------------*/
EXTERN void vidGslInitialize(tstrGslInitializeArgs* pstrArgs);

#endif /* GSL_API_H_ */
