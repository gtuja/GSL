/**
 * @file    gsl_lsm.h
 * @brief   This file provides LSM (LED Service Manager) interfaces.
 * @author  Gtuja
 * @date    Nov 7, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef PSM_INC_GSL_LSM_H_
#define PSM_INC_GSL_LSM_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
typedef enum {
  LSM_STT_NA = 0,   /**< LSM state, not available. */
  LSM_STT_OFF,      /**< LSM state, LED is off. */
  LSM_STT_FADE_IN,  /**< LSM state, LED is under fade in. */
  LSM_STT_ON,       /**< LSM state, LED is on. */
  LSM_STT_FADE_OFF, /**< LSM state, LED is under fade off. */
  LSM_STT_MAX,      /**< LSM maximum state. */
} tenuLsmState;

/* Exported functions prototypes ----------------------------------- */
/* Exported variables ---------------------------------------------- */
PUBLIC void vidLsmInit(void* pvArgs);
PUBLIC void vidLsmSrvc(void* pvArgs);

#endif /* PSM_INC_GSL_LSM_H_ */
