/**
 * @file    gsl_psm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_psm.h"
#include "gsl_api.h"
#include "gsl_feature.h"
#include "gsl_bsm.h"

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef void (*tpfPsmService)(S32 s32Handle);

typedef struct {
  U32           u32Period;
  tpfPsmService pfService;
} tstrPsmService;

/* Private function prototypes -----------------------------------------------*/
PRIVATE S32 s32GslPsmGetFreeHandle(void);

/* Private variables ---------------------------------------------------------*/
PRIVATE U32 gulPsmCounter;
PRIVATE tstrPsmService gpstrPsmService[GSL_PSM_MAX] = {0};

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */

PUBLIC void vidGslPsmInitialize(void* pvArgs) {
  gulPsmCounter = 0;
}

PUBLIC S32 s32GslPsmRegister(tenuGslPsmType enuType, void* pvArgs) {
  S32 s32Handle = GSL_HNDL_NA;
  tstrGslBsmRegisterArgs* pstrGslBsmRegisterArgs;

  switch(enuType) {
  case GSL_PSM_BSM :
    s32Handle = s32GslPsmGetFreeHandle();
    if (s32Handle != GSL_HNDL_NA) {
      pstrGslBsmRegisterArgs = (tstrGslBsmRegisterArgs*)pvArgs;
      gpstrPsmService[s32Handle].u32Period = pstrGslBsmRegisterArgs->u32Period;
      gpstrPsmService[s32Handle].pfService = vidGslBsmService;
      vidGslBsmRegister(s32Handle, pstrGslBsmRegisterArgs);
    }
    break;
  case GSL_PSM_LSM :
    break;
  default :
    break;
  }
  return s32Handle;
}

PUBLIC void vidGslPsmService(void* pvArgs) {
  U32 i;

  gulPsmCounter++;
  for (i=0; i<GSL_PSM_MAX; i++)  {
    if (gpstrPsmService[i].u32Period != (U16)0) {
      if ((gulPsmCounter % gpstrPsmService[i].u32Period) == (U32)0) {
        if (gpstrPsmService[i].pfService != NULL) {
          gpstrPsmService[i].pfService(NULL);
        }
      }
    }
  }
}

PRIVATE S32 s32GslPsmGetFreeHandle(void) {
  S32 s32Handle = GSL_HNDL_NA;
  U32 i;

  for (i=0; i<GSL_PSM_MAX; i++)  {
    if (gpstrPsmService[i].u32Period == (U16)0) {
      s32Handle = i;
      break;
    }
  }
  return s32Handle;
}
