/**
 * @file    gsl_lsm.h
 * @brief   This file provides LSM interfaces. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef XSM_INC_GSL_LSM_H_
#define XSM_INC_GSL_LSM_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
/* Exported functions prototypes ----------------------------------- */
/* Exported variables ---------------------------------------------- */

PUBLIC void vidLsmInit(void* pvArgs);
PUBLIC void vidLsmSrvc(void* pvArgs);

#endif /* XSM_INC_GSL_LSM_H_ */
