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
- v0.3 2024/11/07 Seho.Seo Update overall.

</details>

<div id="Abbreviation"></div>
<details open>
<summary><font size="5"><b>Abbreviation</b></font></summary>

- [TOC](#toc)<br>

| Abbrev. | Meaning |
|:--|:--|
|GSL|G? Static Library|
|UA|User Application|
|PSM|Periodic Service Manager|
|TPM|Task Process Manager|
|DIAG|Diagnostics|
|IPC|Inter Process Communication|
|BSM|Button Service Manager|
|LSM|LED Service Manager|

<br>
</details>

<div id="Concept"></div>
<details open>
<summary><font size="5"><b>Concept</b></font></summary>

- [TOC](#toc)
- GSL is a collection of C libraries for embedded devices.
- GSL is comprised of modules those implement general purpose features.
- GSL shall be deployed as a platform independent static library. 
- Keep in mind [Golden ratio](https://en.m.wikipedia.org/wiki/Golden_ratio) and [Affordance](https://en.m.wikipedia.org/wiki/Affordance), divide and conquer with [Occam's razor](https://en.m.wikipedia.org/wiki/Occam%27s_razor). 

</details>

<div id="Environment"></div>
<details open>
<summary><font size="5"><b>Environment</b></font></summary>

- [TOC](#toc)
- Nucleo Evaluation Board<br>
  - [NUCLEO-L053R8](https://www.st.com/en/evaluation-tools/nucleo-l053r8.html)
  - [NUCLEO-F429ZI](https://www.st.com/en/evaluation-tools/nucleo-f429zi.html)
  - [NUCLEO-G0B1RE](https://www.st.com/en/evaluation-tools/nucleo-g0b1re.html)
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
- [PSM](#PSM)
- [TPM](#TPM)

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
Each of files in this folder provides interfaces between GSL and UA (User Application).
- ***gsl_api.h***<br>
gsl_api.h provides GSL callback APIs below shall be called by UA.<br>

```C
EXTERN void vidGslInitCallback(void* pvArgs);
EXTERN void vidGslSrvcCallback(void* pvArgs);
EXTERN void vidGslProcCallback(void* pvArgs);
EXTERN tenuBsmNotify enuGslBsmNotifyCallback(tenuBsmType enuType);
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
#define FEATURE_L053R8
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
|DIAG/Inc|gsl_diag.h|
|DIAG/Src|gsl_diag.c|
|||

- ***gsl_diag.h***<br>
gsl_diag.h provides diagnostic method, e.g., vidDiagTusStart, vidDiagTrace, etc, stride over GSL.<br>

```C
PUBLIC void vidDiagInit(void* pvArgs);
PUBLIC void vidDiagTusStart(void* pvArgs);
PUBLIC U32  u32DiagTusElapsed(void* pvArgs);
PUBLIC void vidDiagTrace(CH* pcTrace);
PUBLIC void vidDiagTracePsmState(tenuPsmType enuType, const CH* pcName, U32 u32SttPrevious, U32 u32SttCurrent, U32 u32Event);
PUBLIC void vidDiagKeepAlive(U32 u32TmsCnt, U32 u32TusOrtMax);
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
- IPC (Inter Process Communication) provides communication among GSL modules.
- Time consuming requests, e.g., serial communication, shall be done in task with IPC interfaces.
- Folder structure

| Path | File Name |
|:--|:--|
|IPC/Inc|gsl_queue.h|
|IPC/Src|gsl_queue.c|
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

<div id="PSM"></div>
<details open>
<summary><font size="5"><b>PSM</b></font></summary>

- [Features](#Features)
- PSM (Periodic Service Manager) provides periodic services, i.e., BSM (Button Service Manager), LSM(LED Service Manager).
- Folder structure

| Path | File Name |
|:--|:--|
|PSM/Inc|gsl_psm.h|
||gsl_bsm.h|
||gsl_lsm.h|
|PSM/Src|gsl_psm.c|
||gsl_bsm.c|
||gsl_lsm.c|
|||

- ***gsl_psm.h***<br>
gsl_psm.h provides PSM (Periodic Service Manager) interfaces, e.g., vidPsmInit, vidPsmSrvc, etc.<br>

```C
PUBLIC void vidPsmInit(void* pvArgs);
PUBLIC void vidPsmSrvc(void* pvArgs);
...
```

- ***gsl_bsm.h***<br>
gsl_bsm.h provides BSM interfaces.<br>

```C
PUBLIC void vidBsmInit(void* pvArgs);
PUBLIC void vidBsmSrvc(void* pvArgs);
PUBLIC tenuBsmNotify enuBsmNotifyCallback(tenuBsmType enuType);
...
```

- ***gsl_lsm.h***<br>
gsl_lsm.h provides LSM interfaces.<br>

```C
PUBLIC void vidLsmInit(void* pvArgs);
PUBLIC void vidLsmSrvc(void* pvArgs);
...
```

- ***gsl_psm.c***<br>
gsl_psm.c implements PSM features and interfaces, e.g., vidPsmInit, vidPsmSrvc, etc.
- ***gsl_bsm.c***<br>
gsl_bsm.c implements BSM features and interfaces with its internal state machine to control button states.
- ***gsl_lsm.c***<br>
gsl_lsm.c implements LSM features and interfaces with its internal state machine to control LED states.

</details>

<div id="TPM"></div>
<details open>
<summary><font size="5"><b>TPM</b></font></summary>

- [Features](#Features)
- XPM (eXtended Process Manager) provide polling processes as part of BPM.
- Folder structure

| Path | File Name |
|:--|:--|
|TPM/Inc|gsl_tpm.h|
|TPM/Src|gsl_tpm.c|
|||

- ***gsl_tpm.h***<br>
gsl_tpm.h provides TPM interfaces.<br>

```C
PUBLIC void vidTpmInit(void* pvArgs);
PUBLIC void vidTpmProc(void* pvArgs);
...
```

- ***gsl_tpm.c***<br>
gsl_tpm.c implements TPM features and interfaces.<br>

</details>

<div id="Reference"></div>
<details open>
<summary><font size="5"><b>Reference</b></font></summary>

- [TOC](#toc)

</details>
<br>

![latest tag](https://img.shields.io/github/v/tag/gtuja/CSC_MS.svg?color=brightgreen)
[![Language](https://img.shields.io/badge/Language-%E6%97%A5%E6%9C%AC%E8%AA%9E-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/README.md)