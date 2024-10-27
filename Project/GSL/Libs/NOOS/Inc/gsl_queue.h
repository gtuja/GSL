/**
 * @file    gsl_queue.h
 * @brief   This file provides GSL queue interfaces. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef NOOS_INC_GSL_QUEUE_H_
#define NOOS_INC_GSL_QUEUE_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
#define QUE_ITM_MAX         10
#define QUE_DIAG_TRACE_LEN  72

/* Exported types -------------------------------------------------- */
typedef enum {
  QUE_DIAG_TRACE = 0,
  QUE_DIAG_TRACE_XSM_STATE,
  QUE_DIAG_KEEP_ALIVE,
  QUE_MAX,
} tenuQueType;

/* Exported functions ---------------------------------------------- */
PUBLIC void  vidQueInit(void* pvArgs);
PUBLIC gBOOL bQueIsEmpty(tenuQueType enuType);
PUBLIC gBOOL bQueIsFull(tenuQueType enuType);
PUBLIC void  vidQueEnqueue(tenuQueType enuType, void* pvItem);
PUBLIC void* pvQueDequeue(tenuQueType enuType);

/* Exported variables ---------------------------------------------- */

#endif /* NOOS_INC_GSL_QUEUE_H_ */
