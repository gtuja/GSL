![latest tag](https://img.shields.io/github/v/tag/gtuja/CSC_MS.svg?color=brightgreen)
[![Language](https://img.shields.io/badge/Language-%E6%97%A5%E6%9C%AC%E8%AA%9E-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/README.md)

# GSL(G? Static Library)

<div id="toc"></div>
<details open>
<summary><font size="5"><b>Table of Contents</b></font></summary>

- [History](#History)
- [Abbreviation](#Abbreviation)
- [Concept](#Concept)
- [Environment](#Environment)
- [Features](#Features)
- [Reference](#Reference)

</details>

<div id="History"></div>
<details open>
<summary><font size="5"><b>History</b></font></summary> 

- [TOC](#toc)
- v0.1 2024/10/11 Seho.Seo Brand new repository..

</details>

<div id="Abbreviation"></div>
<details open>
<summary><font size="5"><b>Abbreviation</b></font></summary>

- [TOC](#toc)

| Abbrev. | Meaning |
|:--|:--|
|GSL|G? Static Library|
|UA|User Application|
|NOOS|NO Operating System|
|PSM|Periodic Service Manager|
|BPM|Background Process Manager|
|XSM|eXclusive Service Manager|
|BSM|Button Service Manager|
|LSM|LED Service Manager|
|DSM|Diagnostic Service Manager|

<br>
</details>

<div id="Concept"></div>
<details open>
<summary><font size="5"><b>Concept</b></font></summary>

- [TOC](#toc)
- GSL is a collection of C libraries for embedded devices.
- GSL is comprised of NOOS and modules that implement general purpose features.
- GSL shall be deployed as a platform independent static library. 
- Keep in mind [Golden ratio](https://en.m.wikipedia.org/wiki/Golden_ratio) and [Affordance](https://en.m.wikipedia.org/wiki/Affordance), divide and conquer with [Occam's razor](https://en.m.wikipedia.org/wiki/Occam%27s_razor). 

</details>

<div id="Environment"></div>
<details open>
<summary><font size="5"><b>Environment</b></font></summary>

- [TOC](#toc)
- Nucleo Evaluation Board<br>
\- [NUCLEO-L053R8](https://www.st.com/en/evaluation-tools/nucleo-l053r8.html)
- PC
- Wifi
- [Github account](https://github.com)
- [STM account](https://www.st.com)
- Application<br>
The latest version is always a good choice, but let's use CubeIDE with ***1.16.0***, which does not have any minor features.<br>
\- [![Git](https://img.shields.io/badge/Git-brightgreen?style=flat&logo=Git&logoColor=%23F05032&labelColor=white)](https://git-scm.com/)<br>
\- [![TortoiseGit](https://img.shields.io/badge/TortoiseGit-brightgreen?style=flat)](https://tortoisegit.org/)<br>
\- [![Winmerge](https://img.shields.io/badge/Winmerge-brightgreen?style=flat)](https://winmerge.org/)<br>
\- [![Stm Cube IDE](https://img.shields.io/badge/Stm-brightgreen?style=flat&logo=stmicroelectronics&logoColor=%2303234B&labelColor=white)](https://www.st.com/en/development-tools/stm32cubeide.html)　***1.16.0***<br>
\- [![draw.io](https://img.shields.io/badge/Drawio-brightgreen?style=flat&logo=diagramsdotnet&logoColor=%23F08705&labelColor=white)](https://app.diagrams.net/)<br>
\- [![Doxygen](https://img.shields.io/badge/Doxygen-brightgreen?style=flat)](https://www.doxygen.nl/)<br>
\- [![Graphviz](https://img.shields.io/badge/Graphviz-brightgreen?style=flat)](https://graphviz.org/)<br>

</details>

<div id="Features"></div>
<details open>
<summary><font size="5"><b>Features</b></font></summary>

- GSL is comprised of features below.
- [GSL](#GSL)
- [NOOS](#NOOS)
- [BSM](#BSM)
- [LSM](#LSM)
- [DSM](#DSM)
- [DIAG](#DIAG)

</details>

<div id="GSL"></div>
<details open>
<summary><font size="5"><b>GSL</b></font></summary>

- [Features](#Features)
- GSL plays a counter role between UA and GSL.
- GSL is comprised of APIs, configuration, and callbacks.
- GSL also provide define and data types stride over GSL modules.
- Folder structure

| Path | File Name |
|:--|:--|
|include|gsl_def.h|
||gsl_feature.h|
||gsl_config.h|
||gsl_api.h|
|GSL/Inc|gsl.h|
|GSL/Src|gsl.c|

\+ ***include***<br>
Each of files in include folder provides shared features between GSL and UA.<br>

\* ***gsl_def.h***<br>
gsl_def.h defines GSL types.<br>
GSL is aims to be platform independent, gsl_def.h might change according to the UA environment, e.g., tool chain.<br>

```C
#ifdef U32
#undef U32
#define U32 unsigned long
#endif 

#ifdef gBOOL
#undef gBOOL
#define gBOOL U32
#endif
...

```

\* ***gsl_feature.h***<br>
gsl_feature.h specify GSL features.<br>
gsl_feature.h defines macros that control GSL.<br>

```C
#define FEATURE_DSM
#define FEATURE_BSM
#define FEATURE_LSM
#define FEATURE_DIAG
...

```

\* ***gsl_config.h***<br>
gsl_config.h contains defines, types and callbacks.<br>
UA shall redefine defines according to requirement specifications and implement callbacks for device specific features.<br>

```C
/* Application callback -------------------------------------------- */
#ifdef FEATURE_BSM
typedef tenuBsmEvent (*tpfBsmEventCallback)(tenuBsmType enuType);
EXTERN tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);
#endif /* FEATURE_BSM */

#ifdef FEATURE_LSM
typedef tenuLsmEvent (*tpfLsmEventCallback)(tenuBsmType enuBsmType, tenuLsmType enuType);
EXTERN tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuType);
typedef void (*tpfLsmOutputCallback)(tenuLsmType enuType, U32 u32PwmDuty);
EXTERN void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty);
typedef U32 (*tpfLsmPwmMaxCallback)(tenuLsmType enuType);
EXTERN U32 enuGslLsmPwmMaxCallback(tenuLsmType enuType);
#endif /* FEATURE_LSM */

#ifdef FEATURE_DIAG
typedef U32 (*tpfDiagCntCallback)(void* pvArgs);
EXTERN U32 u32DiagCntCallback(void* pvArgs);
typedef U32 (*tpfDiagCntPrdCallback)(void* pvArgs);
EXTERN U32 u32DiagCntPrdCallback(void* pvArgs);
typedef void (*tpfDiagTraceCallback)(char* pcTrace);
EXTERN void vidDiagTraceCallback(char* pcTrace);
#endif /* FEATURE_DIAG */

/* Exported defines ------------------------------------------------ */
#ifdef FEATURE_BSM
#define PSM_PRD_BSM   (U32)1    /* The period of BSM service. */
#define BSM_CP_MC     (U32)5    /* The match count of chattering prevention for buttons. */
#define BSM_NTF_TH    (U32)1000 /* The threshold between short and long press on buttons. */

#define BSM_PRD_B0    (U32)1  /**< B1_BLUE PC13 on L053R8TX. */
#define BSM_PRD_B1    (U32)0
#define BSM_PRD_B2    (U32)0
#define BSM_PRD_B3    (U32)0
#define BSM_PRD_B4    (U32)0

#define BSM_NAME_B0   (const char*)"B1_BLUE" /**< B1_BLUE PC13 on L053R8TX. */
#define BSM_NAME_B1   (const char*)""
#define BSM_NAME_B2   (const char*)""
#define BSM_NAME_B3   (const char*)""
#define BSM_NAME_B4   (const char*)""

#define BSM_EVT_CB_B0 enuGslBsmEventCallback
#define BSM_EVT_CB_B1 gNULL
#define BSM_EVT_CB_B2 gNULL
#define BSM_EVT_CB_B3 gNULL
#define BSM_EVT_CB_B4 gNULL

#define BSM_NTF_CB_B0 enuGslBsmNotifyCallback
#define BSM_NTF_CB_B1 gNULL
#define BSM_NTF_CB_B2 gNULL
#define BSM_NTF_CB_B3 gNULL
#define BSM_NTF_CB_B4 gNULL
#endif /* FEATURE_BSM */
...

```
\* ***gsl_api.h***<br>
gsl_api.h provides callback APIs shall be called by UA.<br>

```C
PUBLIC void vidGslInitCallback(void* pvArgs);
PUBLIC void vidGslSrvcCallback(void* pvArgs);
PUBLIC void vidGslProcCallback(void* pvArgs);
PUBLIC void vidGslDiagElapsedCallback(void* pvArgs);
EXTERN tenuBsmNotify enuGslBsmNotifyCallback(tenuBsmType enuType);
...

```
\+ ***GSL/Inc***<br>
\* ***gsl.h***<br>
gsl.h provides defines and data types for GSL modules.<br>

```C
typedef enum {
  XSM_STT_FTN_ENTRY = 0,  /**< XSM state function, entry. */
  XSM_STT_FTN_DO,         /**< XSM state function, do. */
  XSM_STT_FTN_EXIT,       /**< XSM state function, exit. */
  XSM_STT_FTN_MAX,        /**< XSM maximum state function. */
} tenuXsmSttFtn;
...

```
\+ ***GSL/Src***<br>
\* gsl.c<br>
gsl.c implements GSL APIs.<br>
UA shall call them comply with GSL specification.<br>

</details>

<div id="NOOS"></div>
<details open>
<summary><font size="5"><b>NOOS</b></font></summary>

- [Features](#Features)
- NOOS provides OS-like features below.<br>
\- PSM (Periodic Service Manager)<br>
\- BPM (Background Process Manager)<br>
\- Simple queue provides IPC among GCL features.<br>

- Folder structure

| Path | File Name |
|:--|:--|
|NOOS/Inc|gsl_psm.h|
||gsl_bpm.h|
||gsl_queue.h|
|NOOS/Src|gsl_psm.c|
||gsl_bpm.c|
||gsl_queue.c|

\+ ***NOOS/Inc***<br>
There are 3 preset NOOS features, i.e., PSM, BPM, and a Queue.<br>
APIs for each of features shall privide through header files below.<br>
\* ***gsl_psm.h***<br>

```C
typedef enum {
  PSM_TYPE_DSM = 0, /**< PSM type : DIAG service. */
  PSM_TYPE_BSM,     /**< PSM type : BSM service. */
  PSM_TYPE_LSM,     /**< PSM type : LSM service. */
  PSM_TYPE_MAX,     /**< PSM type maximum. */
} tenuPsmType;

typedef struct {
  const CH* pcSrvName;  /**< PSM service name.  */
  gBOOL bIsRegistered;  /**< PSM is registered or not.  */
  U64   u64TusElapsed;  /**< PSM elapsed time.  */
} tstrPsmSrvDiag;

typedef struct {
  const CH* pcName;                     /**< PSM name. */
  U64   u64TusElapsed;                  /**< PSM elapsed time.  */
  tstrPsmSrvDiag strDiag[PSM_TYPE_MAX]; /**< PSM diagnostic information. */
} tstrPsmDiag;

typedef void (*tpfPsmInit)(void* pvArgs);
typedef void (*tpfPsmSrvc)(void* pvArgs);
typedef struct {
  U32         u32Period;  /**< PSM period. */
  tpfPsmInit  pfPsmInit;  /**< PSM Initialize. */
  tpfPsmSrvc  pfPsmSrvc;  /**< PSM service. */
} tstrPsmCfg;

/* Exported functions prototypes ----------------------------------- */
PUBLIC void vidPsmInit(void* pvArgs);
PUBLIC void vidPsmSrvc(void* pvArgs);
PUBLIC tstrPsmDiag* pstrPsmGetDiag(void* pvArgs);
...

```

\* ***gsl_bpm.h***<br>

```C
PUBLIC void vidBpmInit(void* pvArgs);
PUBLIC void vidBpmProc(void* pvArgs);
...

```

\* ***gsl_queue.h***<br>

```C
#define GSL_QUE_LEN       10
#define GSL_QUE_TRACE_LEN 72

/* Exported types -------------------------------------------------- */
typedef enum {
  GSL_QUE_TRACE = 0,
  GSL_QUE_DIAG,
  GSL_QUE_MAX,
} tenuGslQueType;

/* Exported functions ---------------------------------------------- */
PUBLIC void vidGslQueInit(void* pvArgs);
PUBLIC gBOOL bGslQueIsEmpty(tenuGslQueType enuType);
//PRIVATE gBOOL bGslQueIsFull(tenuGslQueType enuType);
PUBLIC void vidGslQueEnqueue(tenuGslQueType enuType, void* pvItem);
PUBLIC void* pvGslQueDequeue(tenuGslQueType enuType);
...

```
\+ ***NOOS/Src***<br>
\* ***gsl_psm.c***<br>
PSM is responsible for periodic services, e.g., DSM, BSM, LSM, and shall be invoked by GSL API, vidGslSrvc.<br>
PSM also provide diagnostic information for measuring occupancy time of PSM.<br>
Preset services are below and might change according to the UA specification.<br>

\* ***gsl_bpm.c***<br>
BPM is responsible for background processes and shall be invoked by GSL API, vidGslProc.<br>
Each of processes might have more than one queue and handle enqueued requests as a background task.<br>
Preset processes are below and might change according to the UA specification.<br>

```C
PUBLIC const tstrBtmCfg gcpstrBtmCfgTbl[BTM_TYPE_MAX] = {
  /* pfBtmProcess        */
  {  vidBtmProcIdle  },  /* BTM_TYPE_IDLE */
  {  vidBtmProcDiag },   /* BTM_TYPE_DIAG */
  {  vidBtmProcTrace },  /* BTM_TYPE_TRACE */
};
...

```
\* ***gsl_queue.c***<br>
Queue provides generic features, i.e., enqueue, dequeue.<br>
Enqueue is available for evert features of GSL.<br>
Dequeue shall be processed by BTM, background task.<br>
Natural born simple library, there is no context switching on dequeue process, just polling in the background tasks.<br>
Preset queues are below and might change according to the UA specification.<br>

</details>

<div id="BSM"></div>
<details open>
<summary><font size="5"><b>BSM</b></font></summary>

- [Features](#Features)
- BSM provides button service management.<br>
- 
- Folder structure

| Path | File Name |
|:--|:--|
|NOOS/Inc|gsl_psm.h|
||gsl_bpm.h|
||gsl_queue.h|
|NOOS/Src|gsl_psm.c|
||gsl_bpm.c|
||gsl_queue.c|

\+ ***NOOS/Inc***<br>
There are 3 preset NOOS features, i.e., PSM, BPM, and a Queue.<br>
APIs for each of features shall privide through header files below.<br>
\* ***gsl_psm.h***<br>

```C
typedef enum {
  PSM_TYPE_DSM = 0, /**< PSM type : DIAG service. */
  PSM_TYPE_BSM,     /**< PSM type : BSM service. */
  PSM_TYPE_LSM,     /**< PSM type : LSM service. */
  PSM_TYPE_MAX,     /**< PSM type maximum. */
} tenuPsmType;

typedef struct {
  const CH* pcSrvName;  /**< PSM service name.  */
  gBOOL bIsRegistered;  /**< PSM is registered or not.  */
  U64   u64TusElapsed;  /**< PSM elapsed time.  */
} tstrPsmSrvDiag;

typedef struct {
  const CH* pcName;                     /**< PSM name. */
  U64   u64TusElapsed;                  /**< PSM elapsed time.  */
  tstrPsmSrvDiag strDiag[PSM_TYPE_MAX]; /**< PSM diagnostic information. */
} tstrPsmDiag;

typedef void (*tpfPsmInit)(void* pvArgs);
typedef void (*tpfPsmSrvc)(void* pvArgs);
typedef struct {
  U32         u32Period;  /**< PSM period. */
  tpfPsmInit  pfPsmInit;  /**< PSM Initialize. */
  tpfPsmSrvc  pfPsmSrvc;  /**< PSM service. */
} tstrPsmCfg;

/* Exported functions prototypes ----------------------------------- */
PUBLIC void vidPsmInit(void* pvArgs);
PUBLIC void vidPsmSrvc(void* pvArgs);
PUBLIC tstrPsmDiag* pstrPsmGetDiag(void* pvArgs);
...

```

\* ***gsl_bpm.h***<br>

```C
PUBLIC void vidBpmInit(void* pvArgs);
PUBLIC void vidBpmProc(void* pvArgs);
...

```

\* ***gsl_queue.h***<br>

```C
#define GSL_QUE_LEN       10
#define GSL_QUE_TRACE_LEN 72

/* Exported types -------------------------------------------------- */
typedef enum {
  GSL_QUE_TRACE = 0,
  GSL_QUE_DIAG,
  GSL_QUE_MAX,
} tenuGslQueType;

/* Exported functions ---------------------------------------------- */
PUBLIC void vidGslQueInit(void* pvArgs);
PUBLIC gBOOL bGslQueIsEmpty(tenuGslQueType enuType);
//PRIVATE gBOOL bGslQueIsFull(tenuGslQueType enuType);
PUBLIC void vidGslQueEnqueue(tenuGslQueType enuType, void* pvItem);
PUBLIC void* pvGslQueDequeue(tenuGslQueType enuType);
...

```
\+ ***NOOS/Src***<br>
\* ***gsl_psm.c***<br>
PSM is responsible for periodic services, e.g., DSM, BSM, LSM, and shall be invoked by GSL API, vidGslSrvc.<br>
PSM also provide diagnostic information for measuring occupancy time of PSM.<br>
Preset services are below and might change according to the UA specification.<br>

\* ***gsl_bpm.c***<br>
BPM is responsible for background processes and shall be invoked by GSL API, vidGslProc.<br>
Each of processes might have more than one queue and handle enqueued requests as a background task.<br>
Preset processes are below and might change according to the UA specification.<br>

```C
PUBLIC const tstrBtmCfg gcpstrBtmCfgTbl[BTM_TYPE_MAX] = {
  /* pfBtmProcess        */
  {  vidBtmProcIdle  },  /* BTM_TYPE_IDLE */
  {  vidBtmProcDiag },   /* BTM_TYPE_DIAG */
  {  vidBtmProcTrace },  /* BTM_TYPE_TRACE */
};
...

```
\* ***gsl_queue.c***<br>
Queue provides generic features, i.e., enqueue, dequeue.<br>
Enqueue is available for evert features of GSL.<br>
Dequeue shall be processed by BTM, background task.<br>
Natural born simple library, there is no context switching on dequeue process, just polling in the background tasks.<br>
Preset queues are below and might change according to the UA specification.<br>

</details>


<div id="Reference"></div>
<details open>
<summary><font size="5"><b>Reference</b></font></summary>

- [TOC](#toc)

</details>
<br>

![latest tag](https://img.shields.io/github/v/tag/gtuja/CSC_MS.svg?color=brightgreen)
[![Language](https://img.shields.io/badge/Language-%E6%97%A5%E6%9C%AC%E8%AA%9E-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/README.md)