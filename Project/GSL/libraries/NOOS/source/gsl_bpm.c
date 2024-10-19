/**
 * @file    gsl_bpm.c
 * @brief   This file is used to implement BPM manages background processes.
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_bpm.h"
#include "gsl_queue.h"

/* External variables ----------------------------------------------- */
/* Private define --------------------------------------------------- */
/* Private typedef -------------------------------------------------- */
/* Private function prototypes -------------------------------------- */
PRIVATE void vidBpmProcIdle(void* pvArgs);
PRIVATE void vidBpmProcDiag(void* pvArgs);

/* Private variables ------------------------------------------------ */
/* Public functions ------------------------------------------------- */
/**
 * @brief   A public function that initialize BPM.
 * @param   pvArgs  arguments reserved.
 * @sa      vidGslInitCallback
 * @return  void
 */
PUBLIC void vidBpmInit(void* pvArgs) {
}

/**
 * @brief   A public function that do BPM processes.
 * @param   pvArgs  arguments reserved.
 * @sa      vidGslProcCallback
 * @return  void
 */
PUBLIC void vidBpmProc(void* pvArgs) {
  vidBpmProcIdle(gNULL);
  vidBpmProcDiag(gNULL);
}

/**
 * @brief   A public function that do idle processes.
 * @param   pvArgs  arguments reserved.
 * @sa      vidBpmProc
 * @return  void
 */
PRIVATE void vidBpmProcIdle(void* pvArgs) {
}

/**
 * @brief   A public function that do diagnostic processes.
 * @param   pvArgs  arguments reserved.
 * @sa      vidBpmProc
 * @return  void
 */
PRIVATE void vidBpmProcDiag(void* pvArgs) {
  if (bGslQueIsEmpty(GSL_QUE_TRACE) != gTRUE) {
    vidDsmTraceCallback((char*)pvGslQueDequeue(GSL_QUE_TRACE));
  }
}
