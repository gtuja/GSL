/**
 * @file    gsl_nos_api.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 9, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GSL_NOS_API_H_
#define GSL_NOS_API_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef void (*tpfGnosInitialize)(void* pvArgs);
typedef void (*tpfGnosService)(void* pvArgs);

typedef struct {
  U16               u16Period;
  tpfGnosInitialize pfGnosInitialize;
  tpfGnosService    pfGnosService;
} tstrGnosRegisterArgs;

/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

extern BOOL bGnosRegister(tstrGnosRegisterArgs* pstrArgs);
extern void vidGnosService(void* pvArgs);

#endif /* GSL_NOS_API_H_ */
