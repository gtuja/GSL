/**
 * @file    gsl_def.h
 * @brief   This file is used to ... 
 * @author  Gtuja
 * @date    Oct 9, 2024
 * @note    Copyleft, All rights reversed.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GSL_DEF_H_
#define GSL_DEF_H_

/* Includes ------------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
#ifdef PRIVATE
#undef PRIVATE
#endif /* PRIVATE */
#define PRIVATE static

#ifdef PUBLIC
#undef PUBLIC
#endif /* PUBLIC */
#define PUBLIC

#ifdef EXTERN
#undef EXTERN
#endif /* PUBLIC */
#define EXTERN  extern

#ifdef U8
#undef U8
#endif /* U8 */
#define U8  unsigned char

#ifdef U16
#undef U16
#endif /* U16 */
#define U16 unsigned short

#ifdef U32
#undef U32
#endif /* U32 */
#define U32 unsigned long

#ifdef U64
#undef U64
#endif /* U64 */
#define U64 unsigned long long

#ifdef S8
#undef S8
#endif /* S8 */
#define S8  char

#ifdef CH
#undef CH
#endif /* CH */
#define CH  char

#ifdef S16
#undef S16
#endif /* S16 */
#define S16 short

#ifdef S32
#undef S32
#endif /* S32 */
#define S32 long

#ifdef S64
#undef S64
#endif /* S64 */
#define S64 long long

#ifdef gBOOL
#undef gBOOL
#endif /* gBOOL */
#define gBOOL  U32

#ifdef gTRUE
#undef gTRUE
#endif /* gTRUE */
#define gTRUE  (U32)1

#ifdef gFALSE
#undef gFALSE
#endif /* gFALSE */
#define gFALSE  (U32)0

#ifdef gNULL
#undef gNULL
#endif /* gNULL */
#define gNULL   0

/* Exported types ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

#endif /* GSL_DEF_H_ */
