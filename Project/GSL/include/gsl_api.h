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
#include "gsl_feature.h"
#include "gsl_config.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions ---------------------------------------------- */
PUBLIC void vidGslInitCallback(void* pvArgs);
PUBLIC void vidGslSrvcCallback(void* pvArgs);
PUBLIC void vidGslProcCallback(void* pvArgs);
PUBLIC void vidGslDsmElapsedCallback(void* pvArgs);
EXTERN tenuBsmNotify enuGslBsmNotifyCallback(tenuBsmType enuType);

/* Exported variables ---------------------------------------------- */
#endif /* GSL_API_H_ */
