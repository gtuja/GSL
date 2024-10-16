/**
 * @file    gsl_config.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 16, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion --------------------------- */
#ifndef GSL_CONFIG_H_
#define GSL_CONFIG_H_

/* Includes -------------------------------------------------------- */
//#include "gsl_def.h"

//EXTERN tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);

/* Exported defines ------------------------------------------------ */
#define GSL_DSM_PRD   (U32)10000
#define GSL_BSM_PRD   (U32)1
#define GSL_LSM_PRD   (U32)1

#define GSL_BSM_PRD_B1    (U32)1 /* BSM_TYPE_B1_BLUE */
#define GSL_BSM_MATCH_CNT (U32)5
#define GSL_BSM_PRESS_TH  (U32)1000
#define GSL_BSM_EVT_CB_B1 enuGslBsmEventCallback

#define GSL_LSM_PRD_L1    (U32)1            /* LSM_TYPE_LD2_GREEN */
#define GSL_LSM_BSM_L1    BSM_TYPE_B1_BLUE  /* LSM_TYPE_LD2_GREEN */
#define GSL_LSM_FI_TMO_L1 (U32)1000         /* LSM_TYPE_LD2_GREEN */
#define GSL_LSM_FO_TMO_L1 (U32)2000         /* LSM_TYPE_LD2_GREEN */
#define GSL_LSM_EVT_CB_B1 enuGslLsmEventCallback
#define GSL_LSM_OUT_CB_B1 vidGslLsmOutputCallback

#define GSL_LSM_PRD_D1    (U32)10000        /* DSM_TYPE_KA */

#define GSL_BTM_PROC_IDLE   vidBtmProcessIdle
#define GSL_BTM_PROC_TRACE  vidBtmProcessTrace

#define GSL_TUS_CB          u32GslTickCountCallback
#define GSL_TRACE_CB        vidGslTraceCallback

/* Exported types -------------------------------------------------- */
/* Exported functions ---------------------------------------------- */
/* Exported variables ---------------------------------------------- */

#endif /* GSL_CONFIG_H_ */
