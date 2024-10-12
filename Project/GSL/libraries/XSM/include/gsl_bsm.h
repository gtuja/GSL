/**
 * @file    gsl_bsm.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef XSM_INCLUDE_GSL_BSM_H_
#define XSM_INCLUDE_GSL_BSM_H_

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/** tenuBsmEvent, with in line documents. */
typedef enum {
  BSM_EVT_NTF_NA = 0, /**< BSM event notified, not available. */
  BSM_EVT_NTF_SHORT,  /**< BSM event notified, short-press. */
  BSM_EVT_NTF_LONG,   /**< BSM event notified, long-press. */
  BSM_EVT_NTF_MAX,    /**< BSM event notified, maximum event. */
} tenuBsmEventNotify;

/* Exported functions prototypes ---------------------------------------------*/
PUBLIC void vidBsmInitialize(void* pvArgs);
PUBLIC void vidGslBsmService(S32 s32Handle);
PUBLIC void vidGslBsmRegister(S32 s32Handle, tstrGslBsmRegisterArgs* pstrArgs);

#endif /* XSM_INCLUDE_GSL_BSM_H_ */
