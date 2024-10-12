/**
 * @file    application.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 12, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes ------------------------------------------------------------------*/
#include "app_feature.h"
#include "app_api.h"
#include "gsl_api.h"
#include "stm32l0xx_hal.h"

/* External variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PRIVATE const CH* pchName = "Application";

/* Public functions ----------------------------------------------------------*/

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */

PUBLIC void vidAppInitialize(void) {
  tstrGslInitializeArgs strArgs;
  strArgs.pchName = pchName;
  strArgs.pfGslTickCallback = u32AppTickCallback;
  strArgs.u32TickCounterPeriod = (U32)(TIM22->ARR);
  vidGslInitialize(&strArgs);


}

PUBLIC void vidAppRegisterService(void) {
  tstrGslRegisterArgs     strGslArgs;
  tstrGslBsmRegisterArgs  strGslBsmArgs;
  S32                     s32Handle;

  strGslArgs.enuType = GSL_SRVC_BSM;
  strGslBsmArgs.u32Period = (U32)1;
  strGslBsmArgs.u32MatchCount = (U32)APP_BTN_MATCH_MAX;
  strGslBsmArgs.u32PressedThreshHold = (U32)APP_BTN_PRESS_TH;
  strGslBsmArgs.pfGslBsmExtractEventCallback = bAppBtnExtractEventCallback;
  strGslArgs.pvArgs = (void*)(&strGslBsmArgs);
  s32Handle = s32GslRegister(&strGslArgs);
  if (s32Handle != GSL_HNDL_NA) {
    gpstrAppBtnHandle[s32Handle].pGpiox = GPIOC;
    gpstrAppBtnHandle[s32Handle].u16GpioPin = GPIO_PIN_13;
  }
}

PUBLIC void vidAppRegisterProcess(void) {

}
