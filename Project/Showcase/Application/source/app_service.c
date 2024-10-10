/**
 * @file    app_main.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 10, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "gsl_def.h"
#include "gsl_api.h"
#include "gsl_nos_api.h"

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
PRIVATE void vidAppButtonServiceInitialize(void* pvArgs);
PRIVATE void vidAppButtonService(void* pvArgs);
PRIVATE void vidAppLedServiceInitialize(void* pvArgs);
PRIVATE void vidAppLedService(void* pvArgs);

/* Private variables ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

EXTERN U32 u32AppTickCallback(void);
EXTERN void vidAppTraceCallback(char* pcTrace);

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */

PUBLIC void vidAppInitialize(void) {
  tstrGslInitializeArgs strGslInitializeArgs;
  tstrGnosRegisterArgs  strGnosRegisterArgs;

  strGslInitializeArgs.strAppCallbacks.pfTick = u32AppTickCallback;
  strGslInitializeArgs.strAppCallbacks.pfTrace = vidAppTraceCallback;

  vidGslInitialize(&strGslInitializeArgs);

  strGnosRegisterArgs.u16Period = (U16)1;
  strGnosRegisterArgs.pfGnosInitialize = vidAppButtonServiceInitialize;
  strGnosRegisterArgs.pfGnosService = vidAppButtonService;
  bGnosRegister(&strGnosRegisterArgs);

  strGnosRegisterArgs.u16Period = (U16)1;
  strGnosRegisterArgs.pfGnosInitialize = vidAppLedServiceInitialize;
  strGnosRegisterArgs.pfGnosService = vidAppLedService;
  bGnosRegister(&strGnosRegisterArgs);
}

PRIVATE void vidAppButtonServiceInitialize(void* pvArgs) {
}

PRIVATE void vidAppButtonService(void* pvArgs) {
}

PRIVATE void vidAppLedServiceInitialize(void* pvArgs) {
}

PRIVATE void vidAppLedService(void* pvArgs) {
}


