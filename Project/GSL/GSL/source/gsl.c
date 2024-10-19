/**
 * @file    gsl.c
 * @brief   This file is used to implement GSL that
 *          plays a counter role between GSL and UA. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_def.h"
#include "gsl_feature.h"
#include "gsl_config.h"
#include "gsl_api.h"
#include "gsl_psm.h"
#include "gsl_bpm.h"
#include "gsl_bsm.h"
#include "gsl_dsm.h"
#include "gsl_queue.h"
#include <string.h>

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
  vidGslQueInit(gNULL);
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
 * @brief   A public function fetching diagnostic information (DSM).
 * @param   pvArgs  arguments reserved.
 * @note    UA shall call this callback when us unit timer elapsed.
 *          The period counter shall be set in gsl_config.h
 * @sa      u32DsmCntPrdCallback
 * @return  void
 */
PUBLIC void vidGslDsmElapsedCallback(void* pvArgs) {
  vidDsmTusAccumulate(gNULL);
}

