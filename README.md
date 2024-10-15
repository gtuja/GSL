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
|BTM|Background Task Manager|
|XSM|eXclusive State Manager|
|BSM|Button State Manager|
|LSM|LED State Manager|
|DIAG|Diagnosis |

<br>
</details>

<div id="Concept"></div>
<details open>
<summary><font size="5"><b>Concept</b></font></summary>

- [TOC](#toc)
- GSL is a collection of C libraries for enbeded devices.
- GSL is comprised of NOS and modules that implement general purpose features.
- GSL shall be deployed as a platform independant static library. 
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
- [XSM](#XSM)
- [DIAG](#DIAG)

</details>

<div id="GSL"></div>
<details open>
<summary><font size="5"><b>GSL</b></font></summary>

- [Features](#Features)
- GSL plays a counter role between UA and GSL.
- GSL is comprised of APIs, configuration, and callbacks.
- Folder structure

| Path | File Name |
|:--|:--|
|source|gsl.c|
|include|gsl_def.h|
||gsl_api.h|
||gsl_feature.h|

- APIs
```C
PUBLIC void vidGslInitialize(void* pvArgs);
PUBLIC void vidGslService(void* pvArgs);
PUBLIC void vidGslProcess(void* pvArgs);
```
- <div id="Configuration"></div>

```C
#define BSM_PRD (U32)1
#define BSM_MATCH_CNT (U32)5
#define BSM_PRESS_TH (U32)1000
#define BSM_EVT_CB enuGslBsmEventCallback

#define LSM_PRD (U32)1
#define LSM_FI_TMO (U32)1000
#define LSM_FI_TMO (U32)2000
#define LSM_EVT_CB enuGslLsmEventCallback
#define LSM_OUTPUT_CB vidGslLsmOutputCallback
```

- Callbacks
```C
EXTERN U32  u32GslTickCountCallback(void* pvArgs);
EXTERN U32  u32GslTickPeriodCallback(void* pvArgs);
EXTERN void vidGslTraceCallback(char* pcTrace);
EXTERN tenuBsmEvent enuGslBsmEventCallback(tenuBsmType enuType);
EXTERN tenuLsmEvent enuGslLsmEventCallback(tenuBsmType enuBsmType, tenuLsmType enuLsmType);
EXTERN void vidGslLsmOutputCallback(tenuLsmType enuType, U32 u32PwmDuty);
```

</details>

<div id="NOOS"></div>
<details open>
<summary><font size="5"><b>NOOS</b></font></summary>

- [Features](#Features)
- NOOS provides OS-like features below.<br>
\- PSM (Periodic Service Manager)<br>
\- BTM (Background Task Manager)<br>
\- Simpe queue provides IPC among GCL features.<br>

- Folder structure

| Path | File Name |
|:--|:--|
|NOOS/include|gsl_psm.h|
||gsl_btm.h|
||gsl_queue.h|
|NOOS/source|gsl_psm.c|
||gsl_btm.c|
||gsl_queue.c|

- PSM is responsibe for periodic services and shall be invoked by GSL API, vidGslService.
- Preset configurations are below and those might change depending on the UA specification. 

```C
PUBLIC const tstrPsmCfg gcpstrPsmCfgTbl[PSM_TYPE_MAX] = {
  /* u32Period  pfPsmService        */
  {  (U32)1,    vidPsmServiceClock  },  /* PSM_TYPE_CLK */
  {  (U32)1,    vidPsmServiceBsm    },  /* PSM_TYPE_BSM */
  {  (U32)1,    vidPsmServiceLsm    },  /* PSM_TYPE_LSM */
};
```
- BTM is responsibe for task processes and shall be invoked by GSL API, vidGslProcess.
- Preset [Configuration](#Configuration) might change depending on the UA specification. 

```C
PUBLIC const tstrBtmCfg gcpstrBtmCfgTbl[BTM_TYPE_MAX] = {
  /* pfBtmProcess        */
  {  vidBtmProcessIdle  },  /* BTM_TYPE_IDLE */
  {  vidBtmProcessTrace },  /* BTM_TYPE_TRACE */
};
```
- Queue provides generic features, i.e., enqueue, dequeue.
- Simpe itself, it dosen't provide any context switching or receiving.
- Enqueue is available for each feature of GSL.
- Dequeue shall be done at the BTM, polling style.
</details>

<div id="Reference"></div>
<details open>
<summary><font size="5"><b>Reference</b></font></summary>

- [TOC](#toc)

</details>
<br>

![latest tag](https://img.shields.io/github/v/tag/gtuja/CSC_MS.svg?color=brightgreen)
[![Language](https://img.shields.io/badge/Language-%E6%97%A5%E6%9C%AC%E8%AA%9E-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/README.md)