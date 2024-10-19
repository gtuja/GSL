/**
 * @file    gsl.h
 * @brief   gsl.h provides defines, data types and extern configuration
 *          tables for GSL modules, e.g., gcpstrPsmCfgTbl.
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef GSL_H_
#define GSL_H_

/* Includes -------------------------------------------------------- */
#include "gsl_def.h"
#include "gsl_feature.h"
#include "gsl_config.h"
#include "gsl_bsm.h"
#include "gsl_lsm.h"
#include "gsl_dsm.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */

typedef enum {
  XSM_STT_FTN_ENTRY = 0,  /**< XSM state function, entry. */
  XSM_STT_FTN_DO,         /**< XSM state function, do. */
  XSM_STT_FTN_EXIT,       /**< XSM state function, exit. */
  XSM_STT_FTN_MAX,        /**< XSM maximum state function. */
} tenuXsmSttFtn;

typedef struct {
  U32 u32TusElapsedTotal; /**< Total elapsed time for each of GSL modules. */
  U32 u32TusElapsedMax;   /**< Max elapsed time for each of GSL modules. */
} tstrGslDiag;

/* Exported functions prototypes ----------------------------------- */
/* Exported variables ---------------------------------------------- */

#endif /* GSL_H_ */
