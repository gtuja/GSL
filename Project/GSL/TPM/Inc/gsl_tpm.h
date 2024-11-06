/**
 * @file    gsl_tpm.h
 * @brief   This file provides TPM interfaces. 
 * @author  Gtuja
 * @date    Nov 7, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef TPM_INC_GSL_TPM_H_
#define TPM_INC_GSL_TPM_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
/* Exported functions prototypes ----------------------------------- */
PUBLIC void vidTpmInit(void* pvArgs);
PUBLIC void vidTpmProc(void* pvArgs);

/* Exported variables ---------------------------------------------- */

#endif /* TPM_INC_GSL_TPM_H_ */
