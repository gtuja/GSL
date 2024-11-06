/**
 * @file    gsl_psm.h
 * @brief   This file provides PSM interfaces. 
 * @author  Gtuja
 * @date    Nov 7, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef PSM_INC_GSL_PSM_H_
#define PSM_INC_GSL_PSM_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */

typedef enum {
  PSM_TYPE_BSM = 0, /**< PSM type : BSM service. */
  PSM_TYPE_LSM,     /**< PSM type : LSM service. */
  PSM_TYPE_MAX,     /**< PSM type maximum. */
} tenuPsmType;

typedef enum {
  PSM_STT_FTN_ENTRY = 0,  /**< XSM state function, entry. */
  PSM_STT_FTN_DO,         /**< XSM state function, do. */
  PSM_STT_FTN_EXIT,       /**< XSM state function, exit. */
  PSM_STT_FTN_MAX,        /**< XSM maximum state function. */
} tenuPsmSttFtn;

/* Exported functions prototypes ----------------------------------- */
PUBLIC void vidPsmInit(void* pvArgs);
PUBLIC void vidPsmSrvc(void* pvArgs);

/* Exported variables ---------------------------------------------- */

#endif /* PSM_INC_GSL_PSM_H_ */
