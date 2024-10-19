/**
 * @file    gsl_dsm.h
 * @brief   This file provides DSM interfaces. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef XSM_INCLUDE_GSL_DSM_H_
#define XSM_INCLUDE_GSL_DSM_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
/* Exported functions ---------------------------------------------- */
PUBLIC void vidDsmInit(void* pvArgs);
PUBLIC void vidDsmSrvc(void* pvArgs);
PUBLIC void vidDsmTusMeasStart(void *pvArgs);
PUBLIC U32  u32DsmGetTusMeasElapsed(void *pvArgs);
PUBLIC void vidDsmTusAccumulate(void *pvArgs);
PUBLIC U64  u64DsmGetTusTotal(void *pvArgs);

/* Exported variables ---------------------------------------------- */

#endif /* XSM_INCLUDE_GSL_DSM_H_ */
