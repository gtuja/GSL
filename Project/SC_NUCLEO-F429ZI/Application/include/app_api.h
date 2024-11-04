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

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

PUBLIC void vidAppMain(void);
PUBLIC void vidAppService(void);
PUBLIC void vidAppInitialize(void);
PUBLIC U32  u32AppTickCallback(void* pvArgs);
PUBLIC gBOOL bAppBtnExtractEventCallback(void* pvArgs);

#endif /* INCLUDE_APP_API_H_ */
