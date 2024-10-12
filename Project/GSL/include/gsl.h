/**
 * @file    gsl.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GSL_H_
#define GSL_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/** tenuXsmStateFunction, with in line comments. */
typedef enum {
  XSM_STT_FTN_ENTRY = 0,  /**< XSM state function, entry. */
  XSM_STT_FTN_DO,         /**< XSM state function, do. */
  XSM_STT_FTN_EXIT,       /**< XSM state function, exit. */
  XSM_STT_FTN_MAX,        /**< XSM maximum state function. */
} tenuXsmStateFunction;

/** tpfXsmStateFunction XSM state functions */
typedef void (*tpfXsmStateFunction)(S32 Handle);

/** Private tstrXsmStateFunction, with in line comments. */
typedef struct {
  tpfXsmStateFunction pfEntry;  /**< XSM state function, entry. */
  tpfXsmStateFunction pfDo;     /**< XSM state function, do. */
  tpfXsmStateFunction pfExit;   /**< XSM state function, exit. */
} tstrXsmStateFunction;

/* Exported functions prototypes ---------------------------------------------*/

PUBLIC U32  u32GslGetTick(void);
PUBLIC U32  u32GslGetTickCounterPeriod(void);

#endif /* GSL_H_ */
