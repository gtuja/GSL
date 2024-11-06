/**
 * @file    gsl_bsm.h
 * @brief   This file provides BSM interfaces. 
 * @author  Gtuja
 * @date    Nov 7, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef PSM_INC_GSL_BSM_H_
#define PSM_INC_GSL_BSM_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
typedef enum {
  BSM_STT_NA = 0,   /**< BSM state, not available. */
  BSM_STT_RLS,      /**< BSM state, button is released. */
  BSM_STT_PSH_CFM,  /**< BSM state, button is pushed, but under confirmation. */
  BSM_STT_PSH,      /**< BSM state, button is pushed. */
  BSM_STT_RLS_CFM,  /**< BSM state, button is pushed, but under confirmation. */
  BSM_STT_MAX,      /**< BSM maximum state.  */
} tenuBsmStt;

/* Exported functions prototypes ----------------------------------- */
/* Exported variables ---------------------------------------------- */
PUBLIC void vidBsmInit(void* pvArgs);
PUBLIC void vidBsmSrvc(void* pvArgs);
PUBLIC tenuBsmNotify enuBsmNotifyCallback(tenuBsmType enuType);

#endif /* PSM_INC_GSL_BSM_H_ */
