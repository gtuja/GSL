/**
 * @file    gsl_diag.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 19, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef DIAG_INCLUDE_GSL_DIAG_H_
#define DIAG_INCLUDE_GSL_DIAG_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
/* Exported functions ---------------------------------------------- */
PUBLIC void vidDiagTusAccumulate(void* pvArgs);
PUBLIC void vidDiagTusStart(void* pvArgs);
PUBLIC U32  u32DiagTusElapsed(void* pvArgs);
PUBLIC U64  u64DiagGetTusTotal(void *pvArgs);
PUBLIC void vidDiagKeepAlive(void* pvArgs);
PUBLIC void vidDiagPsm(void* pvArgs);
PUBLIC void vidDiagBsm(void* pvArgs);
/* Exported variables ---------------------------------------------- */

#endif /* DIAG_INCLUDE_GSL_DIAG_H_ */
