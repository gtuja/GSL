/**
 * @file    gsl_bsm.h
 * @brief   This file provides BSM interfaces. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef XSM_INC_GSL_BSM_H_
#define XSM_INC_GSL_BSM_H_

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

typedef struct {
  const CH* pcSrvName;            /**< BSM service name.  */
  gBOOL bIsRegistered;            /**< BSM is registered or not.  */
  const CH** pcSttName;           /**< BSM State name.  */
  U32   pu32SttCnt[BSM_STT_MAX];  /**< BSM diagnostic : state counter.  */
} tstrBsmSrvDiag;

typedef struct {
  const CH* pcName;                     /**< BSM name. */
  tstrBsmSrvDiag strDiag[BSM_TYPE_MAX]; /**< BSM diagnostic information. */
} tstrBsmDiag;

/* Exported functions prototypes ----------------------------------- */
/* Exported variables ---------------------------------------------- */
PUBLIC void vidBsmInit(void* pvArgs);
PUBLIC void vidBsmSrvc(void* pvArgs);
PUBLIC void vidBsmDiag(void* pvArgs);

#endif /* XSM_INC_GSL_BSM_H_ */
