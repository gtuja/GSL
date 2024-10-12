/**
 * @file    gsl.c
 * @brief   This file is used to implement GSL plays a counter role between GSL and UA. 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_api.h"
#include "gsl_psm.h"
#include "gsl_btm.h"
#include "gsl_bsm.h"
#include <string.h>

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct {
  tstrGslInitializeArgs strArgs;
} tstrGslControl;

/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PRIVATE tstrGslControl gstGslControl = {0};

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */
PUBLIC void vidGslInitialize(tstrGslInitializeArgs* pstrArgs) {
  memcpy(&(gstGslControl.strArgs), pstrArgs, sizeof(tstrGslInitializeArgs));
  vidGslPsmInitialize(NULL);
  vidGslBtmInitialize(NULL);
  vidBsmInitialize(NULL);
}

PUBLIC S32 s32GslRegister(tstrGslRegisterArgs* pstrArgs) {
  S32 s32Handle = GSL_HNDL_NA;

  switch(pstrArgs->enuType) {
  case GSL_SRVC_BSM :
    s32Handle = s32GslPsmRegister(GSL_PSM_BSM, pstrArgs->pvArgs);
    break;
  case GSL_SRVC_LSM :
    s32Handle = s32GslPsmRegister(GSL_PSM_LSM, pstrArgs->pvArgs);
    break;
  case GSL_TSK_IDLE :
    s32Handle = s32GslBtmRegister(GSL_BTM_IDLE, pstrArgs->pvArgs);
    break;
  default :
    break;
  }
  return s32Handle;
}

PUBLIC void vidGslService(void* pvArgs) {
  vidGslPsmService(NULL);
}

PUBLIC void vidGslProcess(void* pvArgs) {
  vidGslBtmProcess(NULL);
}

PUBLIC U32 u32GslGetTick(void) {
  return (gstGslControl.strArgs.pfGslTickCallback != NULL) ? gstGslControl.strArgs.pfGslTickCallback(NULL) : (U32)0;
}

PUBLIC U32 u32GslGetTickCounterPeriod(void) {
  return gstGslControl.strArgs.u32TickCounterPeriod;
}

