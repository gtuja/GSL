/**
 * @file    gsl_diag.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 19, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef DIAG_INC_GSL_DIAG_H_
#define DIAG_INC_GSL_DIAG_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
typedef struct {
  U32 u32TusElapsedTotal;
  U32 u32TusElapsed;
  U32 u32TusElapsedMax;
} tstrDiagKeepAlive;

/* Exported functions ---------------------------------------------- */
PUBLIC void vidDiagInit(void* pvArgs);
PUBLIC void vidDiagTusAccumulate(void* pvArgs);
PUBLIC void vidDiagTusStart(void* pvArgs);
PUBLIC U32  u32DiagTusElapsed(void* pvArgs);
PUBLIC U32  u32DiagGetTusTotal(void *pvArgs);
PUBLIC void vidDiagTrace(CH* pcTrace);
PUBLIC void vidDiagKeepAlive(tstrDiagKeepAlive* pstrKeepAlive);

/* Exported variables ---------------------------------------------- */

#endif /* DIAG_INC_GSL_DIAG_H_ */
