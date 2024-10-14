/**
 * @file    gsl_queue.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 14, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef NOOS_INCLUDE_GSL_QUEUE_H_
#define NOOS_INCLUDE_GSL_QUEUE_H_

/* Includes -------------------------------------------------------- */
#include "gsl_def.h"

/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
typedef enum {
  GSL_QUE_TRACE = 0,
  GSL_QUE_REPORT,
  GSL_QUE_MAX,
} tenuGslQueType;

/* Exported functions ---------------------------------------------- */
PUBLIC void vidGslQueInitialize(void);
PUBLIC BOOL bGslQueIsEmpty(tenuGslQueType enuType);
PUBLIC BOOL bGslQueIsFull(tenuGslQueType enuType);
PUBLIC void vidGslQueEnqueue(tenuGslQueType enuType, void* pvItem);
PUBLIC void* u32GslQueDequeue(tenuGslQueType enuType);

/* Exported variables ---------------------------------------------- */

#endif /* NOOS_INCLUDE_GSL_QUEUE_H_ */
