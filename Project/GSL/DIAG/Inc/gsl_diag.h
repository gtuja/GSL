/**
 * @file    gsl_diag.h
 * @brief   This file is comprised of interfaces, i.e., types, function prototypes for DIAG.
 * @author  Gtuja
 * @date    Nov 7, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef DIAG_INC_GSL_DIAG_H_
#define DIAG_INC_GSL_DIAG_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"
#include "gsl_psm.h"

/* Exported defines ------------------------------------------------ */
#define DIAG_PRD_KA   (U32)5000

/* Exported types -------------------------------------------------- */
typedef struct {
  U32 u32TmsElapsed;
  U32 u32TusOtMax;
} tstrDiagKeepAlive;

typedef struct {
  tenuPsmType enuType;
  const CH*   pcName;
  U32         u32SttPrevious;
  U32         u32SttCurrent;
  U32         u32Event;
} tstrDiagTracePsmState;

/* Exported functions ---------------------------------------------- */
PUBLIC void vidDiagInit(void* pvArgs);
PUBLIC void vidDiagTusStart(void* pvArgs);
PUBLIC U32  u32DiagTusElapsed(void* pvArgs);
PUBLIC void vidDiagTrace(CH* pcTrace);
PUBLIC void vidDiagTracePsmState(tenuPsmType enuType, const CH* pcName, U32 u32SttPrevious, U32 u32SttCurrent, U32 u32Event);
PUBLIC void vidDiagKeepAlive(U32 u32TmsCnt, U32 u32TusOrtMax);

/* Exported variables ---------------------------------------------- */

#endif /* DIAG_INC_GSL_DIAG_H_ */
