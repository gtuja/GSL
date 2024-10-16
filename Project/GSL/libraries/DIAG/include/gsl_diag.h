/**
 * @file    gsl_diag.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DIAG_INCLUDE_GSL_DIAG_H_
#define DIAG_INCLUDE_GSL_DIAG_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
PUBLIC void vidGslDiagTimeStart(void);
PUBLIC U32  u32GslDiagTimeElapsed(void);
PUBLIC void vidDiagRefreshTus(U32 u32PrdCnt);
PUBLIC U64  u64DiagGetTusElapsed(void);

#endif /* DIAG_INCLUDE_GSL_DIAG_H_ */
