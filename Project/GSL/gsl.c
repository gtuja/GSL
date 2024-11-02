/**
 * @file    gsl.c
 * @brief   This file is used to implement GSL that
 *          plays a counter role between GSL and UA.
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include <gsl_tpm.h>
#include "gsl_config.h"
#include "gsl_psm.h"
#include "gsl_bsm.h"
#include "gsl_queue.h"
#include "gsl_diag.h"

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
/* Private functions ----------------------------------------------- */
/* Private variables ----------------------------------------------- */
/* Public functions ------------------------------------------------ */
/**
 * @brief   A public function that initialize GSL features.
 * @param   pvArgs  arguments reserved.
 * @note    UA shall call this callback at the initialize process.
 * @return  void
 */
PUBLIC void vidGslInitCallback(void* pvArgs) {
  vidPsmInit(gNULL);
  vidBpmInit(gNULL);
  vidQueInit(gNULL);
  vidDiagInit(gNULL);
}

/**
 * @brief   A public function shall do GSL periodic services (PSM).
 * @param   pvArgs  arguments reserved.
 * @note    UA shall call this callback every 1ms with device specific
 *          feature, e.g., TIM.
 * @return  void
 */
PUBLIC void vidGslSrvcCallback(void* pvArgs) {
  vidPsmSrvc(gNULL);
}

/**
 * @brief   A public function shall do GSL background processes (BPM).
 * @param   pvArgs  arguments reserved.
 * @note    UA shall call this callback in the thread loop, main().
 * @return  void
 */
PUBLIC void vidGslProcCallback(void* pvArgs) {
  vidBpmProc(gNULL);
}

/**
 * @brief   A public function shall get the BSM notification.
 * @param   enuType The type of BSM.
 * @note    UA shall call this callback while extracting LSM event.
 * @return  tenuBsmNotify The notification of BSM.
 */
PUBLIC tenuBsmNotify enuGslBsmNotifyCallback(tenuBsmType enuType) {
  return enuBsmNotifyCallback(enuType);
}
