/**
 * @file    gsl_dsm.c
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 16, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Includes -------------------------------------------------------- */
#include "gsl_def.h"
#include "gsl_bsm.h"
#include "gsl.h"
#include "gsl_queue.h"
#include <string.h>
#include <stdio.h>

/* External variables ---------------------------------------------- */
/* Private define -------------------------------------------------- */
/* Private typedef ------------------------------------------------- */
/* Private functions ----------------------------------------------- */
/* Private variables ----------------------------------------------- */

/* Public functions ------------------------------------------------ */
/* Private functions ----------------------------------------------- */

/**
 * @brief   A public function that xxx.
 * @param   xxParam  A xxx parameter used for xxxx from Xxx.
 * @sa      vidXxx
 * @return  void
 */

/**
 * @brief   Public function that process BSM called by PSM.
 * @param   void
 * @sa      vidGslPsmService
 * @return  void
 */
PUBLIC void vidDsmService(void* pvArgs) {
  U32 i;
  CH pchTrace[GSL_TRACE_MAX];

  for (i=0; i<(U32)DSM_TYPE_MAX; i++) {
    snprintf(pchTrace, GSL_TRACE_MAX, "keep alive...");
    vidGslQueEnqueue(GSL_QUE_TRACE, (void*)pchTrace);
  }
}
