![latest tag](https://img.shields.io/github/v/tag/gtuja/GSL.svg?color=brightgreen)

# GSL requirements

<div id="toc"></div>
<details open>
<summary><font size="5"><b>Table of Contents</b></font></summary>

- [History](#history)
- [Concept](#Concept)
- [SwRS](#SwRS)
- [Reference](#Reference)

</details>

<div id="history"></div>
<details open>
<summary><font size="5"><b>History</b></font></summary> 

- [TOC](#toc)
- v0.1 2024/10/22 Seho.Seo Brand new document.

</details>

<div id="Concept"></div>
<details open>
<summary><font size="5"><b>Concept</b></font></summary>

- [TOC](#toc)
- In this document we're gonna specify GSL requirements.
- As the GSL is a platform independent static library, there is no application for now, SR and SyRS shall be specified as needed.
- GSL project shall provide more than one showcases to explain GSL feasibility and evaluate itself.
- Evaluation report may be used as criterias for the SyRS even the SR.
- Keep in mind [Golden ratio](https://en.m.wikipedia.org/wiki/Golden_ratio) and [Affordance](https://en.m.wikipedia.org/wiki/Affordance), divide and conquer with [Occam's razor](https://en.m.wikipedia.org/wiki/Occam%27s_razor). 

</details>

<div id="SwRS"></div>
<details open>
<summary><font size="5"><b>Software Requirement Specification</b></font></summary>

- [TOC](#toc)
- The **software block diagram** shall be below.<br>
![Software Block Diagram](https://github.com/gtuja/CSC_MS/blob/main/Resources/Part2/Part2_ALM_SoftwareBlockDiagram.drawio.png)<br>
- As described in the software block diagram, GSL is comprised of 7 software modules, i.e., CONFIG, GSL API, GSL, NOOS, XSM, XPM, QUEUE and DIAG.
- Those relationship among GSL modules and UA are described in HLDs below.<br>
![High Level Design](https://github.com/gtuja/CSC_MS/blob/main/Resources/Part2/Part2_ALM_SoftwareBlockDiagram.drawio.png)<br>
- CONFIG shall define UA interfaces, i.e., defines, data types, APIs, those are referenced by GSL.
- As the GSL is platform independent library, platform specific features are exported in this module.
- UA shall redefine defines and implement APIs on this module.
- GSL API is comprised of APIs triggered by UA.
- UA shall call those APIs in place as below.
  - vidGslInitCallback shall be called during UA initialization.
  - vidGslSrvcCallback shall be called every 1ms, e.g., TIM ISR.
  - vidGslProcCallback shall be called in the main loop.
  - enuGslBsmNotifyCallback shall be called while extracting LSM event.
- GSL plays counter role between UA and GSL.
- GSL shall implement GSL API and invoke APIs provided by GSL modules, e.g., NOOS, DIAG.
- NOOS shall provide OS-like features, e.g., periodic service, background process.
- NOOS is comprised of PSM and BPM.
- PSM shall be responsible for periodic services triggered by the GSL API, vidGslSrvcCallback.
- PSM is comprised of XSMs, i.e., BSM, LSM, DSM.
- PSM shall also provide diagnostic feature for GSL feasibilities, e.g., occupation times.
- XSM is eXtended Service Manager part of PSM.
- XSM is comprised of BSM, LSM, DSM.
- Each of modules in XSM has its own period.
- BSM shall manage button stataes with its internal state machine.
- BSM shall manage more than one buttons.
- The preset maximu of buttons is 5 that can change with UA requirements.
- Platform specific features below shall be configured by UA through the CONFIG module.
  - The name of button, e.g., #define BSM_NAME_B[0..5] "B1_BLUE"
  - The period of BSM, e.g., #define BSM_PRD_B[0..5] (U32)1
  - The match counts for chattering prevention, e.g., #define BSM_MCCP_B[0..5] (U32)5
  - The threshold of BSM event notificarion between short and long press, e.g., #define BSM_TBEN_B[0..5] (U32)1000
  - The BSM event callback, e.g., #define BSM_EVT_CB enuGslBsmEventCallback
- LSM shall manage LED stataes with its internal state machine.
- LSM shall manage more than one LEDs.
- The preset maximu of LEDs is 5 that can change with UA requirements.
- Platform specific features below shall be configured by UA through the CONFIG module.
  - The name of LED, e.g., #define LSM_NAME_L[0..5] "LD2_GREEN"
  - The period of LSM, e.g., #define LSM_PRD_L[0..5] (U32)1
  - The fade-in time out, e.g., #define LSM_FI_TMO_L[0..5] (U32)1000
  - The fade-out time out, e.g., #define LSM_FO_TMO_L[0..5] (U32)2000
  - The maximum PWM duty, e.g., #define LSM_PD_MAX_L[0..5] (U32)3199
  - The minum PWM duty, e.g., #define LSM_PD_MIN[0..5] (U32)0
  - The LSM event callback, e.g., #define LSM_EVT_CB enuGslLsmEventCallback
  - The LSM output callback, e.g., #define LSM_OUT_CB enuGslLsmOutputCallback
- DSM shall manage diagnostic services.
- DSM is comprised of trace, keep alive, etc.
- Trace shall enqueue data below.
  - BSM state transition
  - LSM state transition
- Keep alive shall enqueue data below.
  - Elapsed time after power on
  - Occupation time of PSM
- BPM shall be triggered by the GSL API, vidGslProcCallback.
- Time consuming peocesses, e.g., serial communication, E2P manipulation, shall be done in this module.
- BPM is comprised of XPMs, i.e., IPM, DPM, EPM.
- XPM is eXtended Process Manager part of BPM.
- XPM is comprised of IPM, DPM, EPM.
- IPM shall be responsible for idle processes, e.g., watch dog clear.
- DPM shall be responsible for diagnostic processes below.
  - Dequeue trace data and transmit them to connected device.
  - Dequeue keep alive data and transmit them to connected device.
- EPM shall be responsible for eeprom manipulations.
- DIAG is an independent module that provides diagnostic methods below.
  - Start us unit counter, e.g., vidDiagTusStart.
  - Get the us unit elapsed time, e.g., u64DiagTusElapses.
  - Make up diagnostic PDU, vidDiagMakeUpPdu(tenuDiagType enuType, void*pvArgs)
- DIAG shall provide data types below those are shared among GSL modules.

```C
typede enum {
  DIAG_TYPE_TRACE = 0,
  DIAG_TYPE_KEEP_ALIVE,
  DIAG_TYPE_MAX,
} tenuDiagType;

typedef struct {
  CH* pcTrace;
} tstrDiagTrace;

typedef struct {
  U64 u64TusTotal;
  U64 u64TusElapsed;
} tstrDiagKeepAlive;
...
```
- QUEUE is a simple IPC method among GSL modules.
- Enqueuing shall be available every where in GSL.
- Dequeuing shall be only available in XPM modules.
- Natural born simple library, there is no context switching on dequeue process, just polling in the background tasks if needed.

</details>

<div id="Reference"></div>
<details open>
<summary><font size="5"><b>Reference</b></font></summary>

- [TOC](#toc)

</details>
<br>

![latest tag](https://img.shields.io/github/v/tag/gtuja/CSC_MS.svg?color=brightgreen)
[![Language](https://img.shields.io/badge/Language-%E6%97%A5%E6%9C%AC%E8%AA%9E-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/Part2/2.RequirementAnalysis.md)
[![Home](https://img.shields.io/badge/Home-Readme-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/README_en.md)
[![Part.2.1](https://img.shields.io/badge/Prev-Part.2.1-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/Part2/1.WorFlowOnGithub_en.md)
[![Part.2.3](https://img.shields.io/badge/Next-Part.2.3-brightgreen)](https://github.com/gtuja/CSC_MS/blob/main/Part2/3.SoftwareDesign_en.md)