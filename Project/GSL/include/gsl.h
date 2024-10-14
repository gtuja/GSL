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
#define GSL_TRACE_MAX   72

/* Exported types ------------------------------------------------------------*/

/** tenuXsmStateFunction, with in line comments. */
typedef enum {
  XSM_STT_FTN_ENTRY = 0,  /**< XSM state function, entry. */
  XSM_STT_FTN_DO,         /**< XSM state function, do. */
  XSM_STT_FTN_EXIT,       /**< XSM state function, exit. */
  XSM_STT_FTN_MAX,        /**< XSM maximum state function. */
} tenuXsmSttFtn;

/* Exported functions prototypes ---------------------------------------------*/

#endif /* GSL_H_ */
