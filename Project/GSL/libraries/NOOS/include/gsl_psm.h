/**
 * @file    gsl_psm.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NOOS_INCLUDE_GSL_PSM_H_
#define NOOS_INCLUDE_GSL_PSM_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
  GSL_PSM_BSM = 0,
  GSL_PSM_LSM,
} tenuGslPsmType;

/* Exported functions prototypes ---------------------------------------------*/
PUBLIC void vidGslPsmInitialize(void* pvArgs);
PUBLIC S32  s32GslPsmRegister(tenuGslPsmType enuType, void* pvArgs);
PUBLIC void vidGslPsmService(void* pvArgs);

#endif /* NOOS_INCLUDE_GSL_PSM_H_ */
