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
- v0.2 2024/10/24 Seho.Seo Update overall.

</details>

<div id="Concept"></div>
<details open>
<summary><font size="5"><b>Concept</b></font></summary>

- [TOC](#toc)
- This document shall specify GSL requirements.
- GSL is aimed to light weight platform independent static library for embedded system.
- GSL provides an abstract way to control more than one buttons and LEDs.
- Platform specific features, e.g., TIM, GPIO, UART, etc, are exported in CONFIG modules and UA should redefine or implement them according to UA specification. 
- There is no application for now, SR and SyRS shall be specified as needed.
- GSL project shall provide more than one showcases to explain GSL feasibility and evaluate itself.
- Evaluation report may be used as criteria for the SyRS even the SR.
- Keep in mind [Golden ratio](https://en.m.wikipedia.org/wiki/Golden_ratio) and [Affordance](https://en.m.wikipedia.org/wiki/Affordance), divide and conquer with [Occam's razor](https://en.m.wikipedia.org/wiki/Occam%27s_razor). 

</details>

<div id="SwRS"></div>
<details open>
<summary><font size="5"><b>Software Requirement Specification</b></font></summary>

- [TOC](#toc)
- The **software block diagram** shall be below.<br>
![Software Block Diagram](https://github.com/gtuja/GSL/blob/main/Document/GSL_Diagram-SoftwareBlockDiagram.drawio.png)<br>
- As described in the software block diagram, GSL is comprised of 8 software modules, i.e., CONFIG, API, GSL, NOOS, XSM, XPM, QUEUE and DIAG.
- Those relationship, i.e., interfaces, among GSL modules and UA are described in the HLD below.<br>
![HLD](https://github.com/gtuja/GSL/blob/main/Document/GSL_Diagram-PackageDiagram.drawio.png)<br>
- ***CONFIG***<br> 
  The CONFIG shall define UA interfaces, i.e., defines, data types, APIs, those are referenced by GSL modules.<br>
  As the GSL is platform independent, platform specific features are exported in this module.<br>
  UA shall redefine defines and implement APIs on this module.<br>
- ***API***<br> 
  API is comprised of APIs triggered by UA<br>
  UA shall call those APIs in place as below<br>
  *vidGslInitCallback* shall be called during UA initialization.<br>
  *vidGslSrvcCallback* shall be called every 1ms, e.g., TIM ISR.<br>
  *vidGslProcCallback* shall be called in the main loop.<br>
  *enuGslBsmNotifyCallback* shall be called while extracting LSM event.<br>
- ***GSL***<br>
  GSL plays a counter role between UA and GSL.<br>
  GSL shall implement APIs and invoke APIs provided by GSL modules, e.g., NOOS, DIAG.<br>
- ***NOOS***<br>
  NOOS shall provide OS-like features, e.g., periodic service, background process.<br>
  NOOS is comprised of PSM and BPM.<br>
  PSM shall be responsible for periodic services triggered by the GSL API, i.e., *vidGslSrvcCallback*.<br>
  PSM is comprised of XSMs, i.e., BSM, LSM, DSM.<br>
  PSM shall also provide diagnostic feature for GSL feasibility, e.g., occupation times.<br>
  BPM shall be triggered by the GSL API, vidGslProcCallback.<br>
  Time consuming requests, e.g., serial communication, from ISR shall be done in this module with IPC interfaces.<br>
  BPM is comprised of XPMs, i.e., IPM, DPM.<br>
- ***XSM***<br>
  XSM is eXtended Service Manager comprised of BSM, LSM, DSM, those shall provide periodic services as part of PSM.<br>
  Each of modules in the XSM has its own period.<br>
  BSM shall manage button states with its internal state machine.<br>
  BSM shall manage more than one buttons.<br>
  The preset maximum of buttons is 5 that can change with UA requirements.<br>
  Platform specific features below shall be configured by UA through the CONFIG module.<br>
  - The name of button, e.g., *#define BSM_NAME_B[0..5] "B1_BLUE"*
  - The period of BSM, e.g., *#define BSM_PRD_B[0..5] (U32)1*
  - The match counts for chattering prevention, e.g., *#define BSM_MCCP_B[0..5] (U32)5*
  - The threshold of BSM event notification between short and long press, e.g., *#define BSM_THN_B[0..5] (U32)1000*
  - The BSM event callback, e.g., *enuGslBsmEventCallback*
  LSM shall manage LED states with its internal state machine.<br>
  LSM shall manage more than one LEDs.<br>
  The preset maximum of LEDs is 5 that can change with UA requirements.<br>
  Platform specific features below shall be configured by UA through the CONFIG module.<br>
  - The name of LED, e.g., *#define LSM_NAME_L[0..5] "LD2_GREEN"*
  - The period of LSM, e.g., *#define LSM_PRD_L[0..5] (U32)1*
  - The paired button type, e.g., *#define LSM_BSM_TYPE_L0 BSM_TYPE_B0*
  - The fade-in time out, e.g., *#define LSM_FI_TMO_L[0..5] (U32)1000*
  - The fade-out time out, e.g., *#define LSM_FO_TMO_L[0..5] (U32)2000*
  - The LSM event callback, e.g., *enuGslLsmEventCallback*
  - The LSM output callback, e.g., *vidGslLsmOutputCallback*
  - The minimum PWM duty callback, e.g., *enuGslLsmPwmMinCallback*
  - The maximum PWM duty callback, e.g., *enuGslLsmPwmMaxCallback*
  DSM shall manage diagnostic services.<br>
  DSM is comprised of trace, keep alive, etc.<br>
  Trace shall enqueue data below.<br>
  - BSM state transition
  - LSM state transition
  Keep alive shall enqueue data below.<br>
  - Elapsed time after power on
  - Occupation time of the PSM
- ***XPM***<br>
  XPM is eXtended Process Manager as part of BPM.<br>
  XPM is comprised of IPM, DPM.<br>
  IPM shall be responsible for idle processes.<br>
  DPM shall be responsible for diagnostic processes below.<br>
  - Dequeue trace data and transmit them to connected device with UA callback, e.g., *vidDiagTraceCallback*.
  - Dequeue keep alive data and transmit them to connected device with UA callback, e.g., *vidDiagTraceCallback*.
- ***QUEUE***<br>
  QUEUE is a simple IPC method among GSL modules.<br>
  Enqueuing shall be available every where in GSL.<br>
  Dequeuing shall only be available in XPM modules.<br>
  Natural born simple library, there is no context switching on dequeue process, just polling in the background tasks if needed.<br>
- ***DIAG***<br>
  DIAG is an independent module that provides diagnostic methods below.<br>
  - Start us unit counter, e.g., *vidDiagTusStart*.
  - Get the us unit elapsed time, e.g., *u32DiagTusElapsed*.
  - Trace wrapper to enqueue trace data, e.g., *vidDiagTrace*.
  - Keep alive wrapper to enqueue keep alive data, e.g., *vidDiagKeepAlive*.
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