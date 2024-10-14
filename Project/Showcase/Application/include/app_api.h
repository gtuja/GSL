/**
 * @file    app_api.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 10, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INCLUDE_APP_API_H_
#define INCLUDE_APP_API_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"
#include "stm32l0xx_hal.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct {
  GPIO_TypeDef  *pGpiox;
  uint16_t      u16GpioPin;
} tstrAppBtnHandle;

/* Exported functions prototypes ---------------------------------------------*/

PUBLIC void vidAppMain(void);
PUBLIC void vidAppService(void);
PUBLIC void vidAppInitialize(void);
PUBLIC U32  u32AppTickCallback(void* pvArgs);
PUBLIC BOOL bAppBtnExtractEventCallback(void* pvArgs);

extern tstrAppBtnHandle gpstrAppBtnHandle[];

#endif /* INCLUDE_APP_API_H_ */
