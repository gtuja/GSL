![latest tag](https://img.shields.io/github/v/tag/gtuja/GSL.svg?color=brightgreen)
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
- v0.2 2024/10/24 Seho.Seo Update overall.
  
</details>

<div id="Abbreviation"></div>
<details open>
<summary><font size="5"><b>Abbreviation</b></font></summary>

- [TOC](#toc)<br>

| Abbrev. | Meaning |
|:--|:--|
|GSL|G? Static Library|
|UA|User Application|
|DIAG|Diagnostics|
|IPC|Inter Process Communication|
|NOOS|NO Operating System|
|BPM|Background Process Manager|
|PSM|Periodic Service Manager|
|XPM|eXtended Process Manager|
|DPM|Diagnostic Process Manager|
|IPM|Idle Process Manage
|XSM|eXtended Service Manager|
|BSM|Button Service Manager|
|DSM|Diagnostic Service Manager|
|LSM|LED Service Manager|

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
  - [NUCLEO-L053R8](https://www.st.com/en/evaluation-tools/nucleo-l053r8.html)
  - [NUCLEO-L053R8](https://www.st.com/en/evaluation-tools/nucleo-f429zi.html)
- PC
- Wifi
- [Github account](https://github.com)
- [STM account](https://www.st.com)
- Application<br>
The latest version is always a good choice, but let's use CubeIDE with ***1.16.0***, which does not have any minor features.<br>
  [![Git](https://img.shields.io/badge/Git-brightgreen?style=flat&logo=Git&logoColor=%23F05032&labelColor=white)](https://git-scm.com/)<br>
  [![TortoiseGit](https://img.shields.io/badge/TortoiseGit-brightgreen?style=flat)](https://tortoisegit.org/)<br>
  [![Winmerge](https://img.shields.io/badge/Winmerge-brightgreen?style=flat)](https://winmerge.org/)<br>
  [![Stm Cube IDE](https://img.shields.io/badge/Stm-brightgreen?style=flat&logo=stmicroelectronics&logoColor=%2303234B&labelColor=white)](https://www.st.com/en/development-tools/stm32cubeide.html)　***1.16.0***<br>
  [![draw.io](https://img.shields.io/badge/Drawio-brightgreen?style=flat&logo=diagramsdotnet&logoColor=%23F08705&labelColor=white)](https://app.diagrams.net/)<br>
  [![Doxygen](https://img.shields.io/badge/Doxygen-brightgreen?style=flat)](https://www.doxygen.nl/)<br>
  [![Graphviz](https://img.shields.io/badge/Graphviz-brightgreen?style=flat)](https://graphviz.org/)<br>

</details>

<div id="Features"></div>
<details open>
<summary><font size="5"><b>Features</b></font></summary>

- GSL is comprised of features below.
- [GSL](#GSL)
- [DIAG](#DIAG)
- [IPC](#IPC)
- [NOOS](#NOOS)
- [XPM](#XPM)
- [XSM](#XSM)

</details>

<div id="GSL"></div>
<details open>
<summary><font size="5"><b>GSL</b></font></summary>

- [Features](#Features)
- GSL plays a counter role between UA and GSL modules.
- GSL is comprised of APIs, configuration, and callbacks.
- GSL also provide define and data types stride over GSL modules.
- Folder structure

| Path | File Name |
|:--|:--|
|Inc|gsl_api.h|
||gsl_config.h|
||gsl_def.h|
||gsl_feature.h|
|gsl.c||
|||

- ***Inc***<br>
Each of files in this folder provides interfaces between GSL and UA (User Application).<br>

- ***gsl_api.h***<br>
gsl_api.h provides GSL callback APIs below shall be called by UA.<br>

```C
PUBLIC void vidGslInitCallback(void* pvArgs);
PUBLIC void vidGslSrvcCallback(void* pvArgs);
PUBLIC void vidGslProcCallback(void* pvArgs);
EXTERN tenuBsmNotify enuGslBsmNotifyCallback(tenuBsmType enuType);
EXTERN void vidGslDiagElapsedCallback(void* pvArgs);
...
```

- ***gsl_config.h***<br>
gsl_config.h is comprised of interfaces, i.e., defines, types, callbacks.<br>
UA shall redefine and implement callbacks for device specific features.<br>

```C
#define BSM_NAME_B0   (const char*)"B1_BLUE" /**< B1_BLUE PC13 on L053R8TX. */
#define BSM_PRD_B0    (U32)1  /**< B1_BLUE PC13 on L053R8TX. */
#define BSM_MCCP_B0   (U32)5    /* The match count of chattering prevention for buttons. */
#define BSM_THN_B0    (U32)1000 /* The threshold between short and long press on buttons. */
typedef tenuBsmEvent (*tpfBsmEventCallback)(tenuBsmType enuType);
EXTERN tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);
...
```

- ***gsl_def.h***<br>
gsl_def.h defines GSL types.<br>
As the GSL is aims to be platform independent static library, gsl_def.h may change according to the UA environment, e.g., tool chain.<br>

```C
#ifdef U32
#undef U32
#define U32 unsigned long
#endif 
...
```

- ***gsl_feature.h***<br>
gsl_feature.h is comprised of defines, i.e., macros, those specify GSL features.<br>

```C
#define FEATURE_DSM
#define FEATURE_BSM
#define FEATURE_LSM
#define FEATURE_DIAG
...
```

- ***gsl.c***<br>
gsl.c implements GSL APIs provided through gsl_api.h to UA.<br>

</details>

<div id="DIAG"></div>
<details open>
<summary><font size="5"><b>DIAG</b></font></summary>

- [Features](#Features)
- DIAG provides diagnostic method stride over GSL modules.
- Folder structure

| Path | File Name |
|:--|:--|
|Libs/DIAG/Inc|gsl_diag.h|
|Libs/DIAG/Src|gsl_diag.c|
|||

- ***gsl_diag.h***<br>
gsl_diag.h provides diagnostic method, e.g., vidDiagTusStart, vidDiagTrace, etc, stride over GSL.<br>

```C
PUBLIC void vidDiagInit(void* pvArgs);
PUBLIC void vidDiagTusAccumulate(void* pvArgs);
PUBLIC void vidDiagTusStart(void* pvArgs);
PUBLIC U32  u32DiagTusElapsed(void* pvArgs);
PUBLIC U64  u64DiagGetTusTotal(void *pvArgs);
PUBLIC void vidDiagTrace(CH* pcTrace);
PUBLIC void vidDiagKeepAlive(tstrDiagKeepAlive* pstrKeepAlive);
...
```

- ***gsl_diag.c***<br>
gsl_diag.c implements DIAG interfaces and UA callbacks as week functions.<br>

```C
PUBLIC __attribute__((weak)) U32 u32DiagTusCntCallback(void* pvArgs) { return (U32)0; }
PUBLIC __attribute__((weak)) U32 u32DiagTusCntPrdCallback(void* pvArgs) { return (U32)0; }
PUBLIC __attribute__((weak)) void vidDiagTraceCallback(char* pcTrace) {}
...
```

</details>

<div id="IPC"></div>
<details open>
<summary><font size="5"><b>IPC</b></font></summary>

- [Features](#Features)
- IPC (Inter Process Communication) provides communication between ISR (Interrupt Service Routine) and Thread, i.e., background process
- Time consuming requests, e.g., serial communication, E2P manipulation, from ISR shall be done in thread with IPC interfaces.
- Folder structure

| Path | File Name |
|:--|:--|
|Libs/IPC/Inc|gsl_queue.h|
|Libs/IPC/Src|gsl_queue.c|
|||

- ***gsl_queue.h***<br>
gsl_queue.h provides simple queue APIs, e.g, vidQueEnqueue, pvQueDequeue.<br>

```C
PUBLIC void  vidQueInit(void* pvArgs);
PUBLIC gBOOL bQueIsEmpty(tenuQueType enuType);
PUBLIC gBOOL bQueIsFull(tenuQueType enuType);
PUBLIC void  vidQueEnqueue(tenuQueType enuType, void* pvItem);
PUBLIC void* pvQueDequeue(tenuQueType enuType);
...
```

- ***gsl_queue.c***<br>
gsl_queue.c implements simple queue features and interfaces.<br>

</details>

<div id="NOOS"></div>
<details open>
<summary><font size="5"><b>NOOS</b></font></summary>

- [Features](#Features)
- NOOS (NO Operating System) provides OS-like features, e.g., periodic service, background process.
- Folder structure

| Path | File Name |
|:--|:--|
|Libs/NOOS/Inc|gsl_bpm.h|
||gsl_psm.h|
|Libs/NOOS/Src|gsl_bpm.c|
||gsl_psm.c|
|||

- ***gsl_bpm.h***<br>
gsl_bpm.h provides BPM (Background Process Manager) interfaces, e.g., vidBpmInit, vidBpmProc, etc.<br>

```C
PUBLIC void vidBpmInit(void* pvArgs);
PUBLIC void vidBpmProc(void* pvArgs);
...
```

- ***gsl_psm.h***<br>
gsl_psm.h provides PSM (Periodic Service Manager) interfaces, e.g., vidPsmInit, vidPsmSrvc, etc..<br>

```C
PUBLIC void vidPsmInit(void* pvArgs);
PUBLIC void vidPsmSrvc(void* pvArgs);
...
```

- ***gsl_bpm.c***<br>
gsl_bpm.c implements BPM features and interfaces, e.g., vidBpmInit, vidBpmProc, etc.<br>

- ***gsl_psm.c***<br>
gsl_psm.c implements PSM features and interfaces, e.g., vidPsmInit, vidPsmSrvc, etc.<br>

</details>

<div id="XPM"></div>
<details open>
<summary><font size="5"><b>XPM</b></font></summary>

- [Features](#Features)
- XPM (eXtended Process Manager) provide polling processes as part of BPM.
- Folder structure

| Path | File Name |
|:--|:--|
|Libs/XPM/Inc|gsl_dpm.h|
||gsl_ipm.h|
||gsl_xpm.h|
|Libs/XPM/Src|gsl_dpm.c|
||gsl_ipm.c|
||gsl_xsm.c|
|||

- ***gsl_dpm.h***<br>
gsl_dpm.h provides DPM (Diagnostic Process Manager) interfaces, e.g., vidDpmInit, vidDpmProc, etc.<br>

```C
PUBLIC void vidDpmInit(void* pvArgs);
PUBLIC void vidDpmProc(void* pvArgs);
...
```

- ***gsl_ipm.h***<br>
gsl_ipm.h provides IPM (Idle Process Manager) interfaces, e.g., vidIpmInit, vidIpmProc <br>

```C
PUBLIC void vidIpmInit(void* pvArgs);
PUBLIC void vidIpmProc(void* pvArgs);
...
```

- ***gsl_xpm.h***<br>
gsl_xpm.h provides XPM shared data types among XPM modules.<br>


- ***gsl_dpm.c***<br>
gsl_dpm.c implements DPM features and interfaces.<br>

- ***gsl_ipm.c***<br>
gsl_ipm.c implements IPM features and interfaces.<br>

</details>

<div id="XSM"></div>
<details open>
<summary><font size="5"><b>XSM</b></font></summary>

- [Features](#Features)
- XSM (exTended Service Manager) provides button, LED, Diag services as part of PSM.<br>
- Folder structure

| Path | File Name |
|:--|:--|
|XSM/Inc|gsl_bsm.h|
||gsl_dsm.h|
||gsl_lsm.h|
||gsl_xsm.h|
|XSM/Src|gsl_bsm.c|
||gsl_dsm.c|
||gsl_lsm.c|

- ***gsl_bsm.h***<br>
gsl_bsm.h provides BSM (Button Service Manager) interfaces.<br>
gsl_bsm.h also provide callback function for button notification, i.e., short-press, long-press.<br>
UA shall use that when extracting LED events.<br>

```C
PUBLIC void vidBsmInit(void* pvArgs);
PUBLIC void vidBsmSrvc(void* pvArgs);
PUBLIC tenuBsmNotify enuBsmNotifyCallback(tenuBsmType enuType);
...
```

- ***gsl_dsm.h***<br>
gsl_dsm.h provides DSM (Diagnostic Service Manager) features and interfaces. <br>

```C
PUBLIC void vidDsmInit(void* pvArgs);
PUBLIC void vidDsmSrvc(void* pvArgs);
...
```

- ***gsl_lsm.h***<br>
gsl_lsm.h provides LSM (LED Service Manager) interfaces.<br>

```C
PUBLIC void vidLsmInit(void* pvArgs);
PUBLIC void vidLsmSrvc(void* pvArgs);
...
```

- ***gsl_xsm.h***<br>
gsl_xsm.h provides XSM shared data types among XSM modules.<br>

- ***gsl_bsm.c***<br>
gsl_bsm.c implements BSM features and interfaces with its internal state machine to control button states.<br>

- ***gsl_dsm.c***<br>
gsl_dsm.c implements DSM features and interfaces.<br>

- ***gsl_lsm.c***<br>
gsl_lsm.c implements LSM features and interfaces with its internal state machine to control LED states.<br>

</details>

<div id="Reference"></div>
<details open>
<summary><font size="5"><b>Reference</b></font></summary>

- [TOC](#toc)

</details>
<br>

![latest tag](https://img.shields.io/github/v/tag/gtuja/CSC_MS.svg?color=brightgreen)
[![Language](https://img.shields.io/badge/Language-%E6%97%A5%E6%9C%AC%E8%AA%9E-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/README.md)