/**
 * @file    gsl_api.h
 * @brief   This file is comprised of interfaces between UA and GSL.
 * @author  Gtuja
 * @date    Oct 9, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef GSL_API_H_
#define GSL_API_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
/* Exported functions ---------------------------------------------- */
EXTERN void vidGslInitCallback(void* pvArgs);
EXTERN void vidGslSrvcCallback(void* pvArgs);
EXTERN void vidGslProcCallback(void* pvArgs);
EXTERN tenuBsmNotify enuGslBsmNotifyCallback(tenuBsmType enuType);

/* Exported variables ---------------------------------------------- */
#endif /* GSL_API_H_ */
