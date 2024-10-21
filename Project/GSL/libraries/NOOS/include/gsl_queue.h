/**
 * @file    gsl_queue.h
 * @brief   This file provides GSL queue interfaces. 
 * @author  Gtuja
 * @date    Oct 18, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef NOOS_INCLUDE_GSL_QUEUE_H_
#define NOOS_INCLUDE_GSL_QUEUE_H_

/* Includes -------------------------------------------------------- */
#include "gsl_config.h"

/* Exported defines ------------------------------------------------ */
#define GSL_QUE_LEN       20
#define GSL_QUE_TRACE_LEN 72

/* Exported types -------------------------------------------------- */
typedef enum {
  GSL_QUE_TRACE = 0,
  GSL_QUE_KEEP_ALIVE,
  GSL_QUE_DIAG,
  GSL_QUE_MAX,
} tenuGslQueType;

/* Exported functions ---------------------------------------------- */
PUBLIC void vidGslQueInit(void* pvArgs);
PUBLIC gBOOL bGslQueIsEmpty(tenuGslQueType enuType);
PUBLIC gBOOL bGslQueIsFull(tenuGslQueType enuType);
PUBLIC void vidGslQueEnqueue(tenuGslQueType enuType, void* pvItem);
PUBLIC void* pvGslQueDequeue(tenuGslQueType enuType);

/* Exported variables ---------------------------------------------- */

#endif /* NOOS_INCLUDE_GSL_QUEUE_H_ */
