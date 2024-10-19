/**
 * @file    gsl_psm.h
 * @brief   This file provides PSM interfaces. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef NOOS_INCLUDE_GSL_PSM_H_
#define NOOS_INCLUDE_GSL_PSM_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
typedef enum {
  PSM_TYPE_DSM = 0, /**< PSM type : DIAG service. */
  PSM_TYPE_BSM,     /**< PSM type : BSM service. */
  PSM_TYPE_LSM,     /**< PSM type : LSM service. */
  PSM_TYPE_MAX,     /**< PSM type maximum. */
} tenuPsmType;

typedef void (*tpfPsmInit)(void* pvArgs);
typedef void (*tpfPsmSrvc)(void* pvArgs);
typedef struct {
  U32         u32Period;  /**< PSM period. */
  tpfPsmInit  pfPsmInit;  /**< PSM Initialize. */
  tpfPsmSrvc  pfPsmSrvc;  /**< PSM service. */
} tstrPsmCfg;

/* Exported functions prototypes ----------------------------------- */
PUBLIC void vidPsmInit(void* pvArgs);
PUBLIC void vidPsmSrvc(void* pvArgs);

/* Exported variables ---------------------------------------------- */

#endif /* NOOS_INCLUDE_GSL_PSM_H_ */
