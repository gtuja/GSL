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
#define GNOS_PS_INTRVL_LOW    100
#define GNOS_PS_INTRVL_NORMAL 10
#define GNOS_PS_INTRVL_HIGH   1

/* Exported types ------------------------------------------------------------*/
typedef enum {
  GNOS_PRRT_NA = 0,
  GNOS_PRRT_LOW,
  GNOS_PRRT_NORMAL,
  GNOS_PRRT_HIGH,
} tenuGnosPriority;

typedef void (*tpfGnosInitialize)(void* pvArgs);
typedef void (*tpfGnosService)(void* pvArgs);

/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

extern BOOL bGnosRegister(tenuGnosPriority enuPriority, tpfGnosInitialize pfInitialize, tpfGnosService pfService);
extern void vidGnosService(void* pvArgs);

#endif /* GSL_NOS_API_H_ */
