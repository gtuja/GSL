/**
 * @file    gsl_xsm.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 21, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef XSM_INC_GSL_XSM_H_
#define XSM_INC_GSL_XSM_H_

/* Includes -------------------------------------------------------- */
/* Exported defines ------------------------------------------------ */
/* Exported types -------------------------------------------------- */
typedef enum {
  XSM_STT_FTN_ENTRY = 0,  /**< XSM state function, entry. */
  XSM_STT_FTN_DO,         /**< XSM state function, do. */
  XSM_STT_FTN_EXIT,       /**< XSM state function, exit. */
  XSM_STT_FTN_MAX,        /**< XSM maximum state function. */
} tenuXsmSttFtn;

/* Exported functions ---------------------------------------------- */
/* Exported variables ---------------------------------------------- */

#endif /* XSM_INC_GSL_XSM_H_ */
