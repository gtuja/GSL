/**
 * @file    gsl_bsm.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef XSM_INCLUDE_GSL_BSM_H_
#define XSM_INCLUDE_GSL_BSM_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_feature.h"
#include "gsl_api.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
PUBLIC void vidBsmInitialize(void* pvArgs);
PUBLIC void vidBsmService(void* pvArgs);
PUBLIC tenuBsmEventNotify enuBsmEventNotify(tenuBsmType enuType);
#endif /* XSM_INCLUDE_GSL_BSM_H_ */
