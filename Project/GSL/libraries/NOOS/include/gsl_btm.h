/**
 * @file    gsl_btm.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NOOS_INCLUDE_GSL_BTM_H_
#define NOOS_INCLUDE_GSL_BTM_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
  GSL_BTM_IDLE = 0,
} tenuGslBtmType;

/* Exported functions prototypes ---------------------------------------------*/
PUBLIC void vidBtmInitialize(void* pvArgs);
PUBLIC void vidBtmProcess(void* pvArgs);

#endif /* NOOS_INCLUDE_GSL_BTM_H_ */
