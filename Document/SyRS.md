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
- GSL project shall provide more than one showcases to show GSL feasibilities and evaluate itself.
- Evaluation report might be criteria for the SyRS even the SR.
- Keep in mind [Golden ratio](https://en.m.wikipedia.org/wiki/Golden_ratio) and [Affordance](https://en.m.wikipedia.org/wiki/Affordance), divide and conquer with [Occam's razor](https://en.m.wikipedia.org/wiki/Occam%27s_razor). 

</details>

<div id="SwRS"></div>
<details open>
<summary><font size="5"><b>System requirement Specification</b></font></summary>

- [TOC](#toc)
- The **software block diagram** shall be below.<br>
![Software Block Diagram](https://github.com/gtuja/CSC_MS/blob/main/Resources/Part2/Part2_ALM_SoftwareBlockDiagram.drawio.png)<br>
- There are 6 software modules, i.e., CONFIG, API, GSL, NOOS, XSM, DIAG among UA, GSL, GSL itself as an abstract point of view.
- Those relationship among modules are described as HLD below.
![High Level Design](https://github.com/gtuja/CSC_MS/blob/main/Resources/Part2/Part2_ALM_SoftwareBlockDiagram.drawio.png)<br>
- CONFIG shall provide interface, i.e., defines, data types,  use middlewares, i.e., HAL (Hardware Abstraction Layer), FreeRTOS. [SyRS_04]
- <br>
\- [SwRS_04] The application services shall be implemented in the ISR Service (Interrupt Service Routine) of a dedicated timer attached to 1msec SysTick. [SyRS_05]<br>
\- [SwRS_05] LED Service shall be processed after Button Service to ensure the button event consistency. [SyRS_07]<br>
\- [SwRS_06] Button Service shall provide features below.<br>
&nbsp;&nbsp;\+ Retrieve button state [SyRS_06]<br>
&nbsp;&nbsp;\+ Chattering prevention [SyRS_06]<br>
&nbsp;&nbsp;\+ Extract button events [SyRS_06]<br>
&nbsp;&nbsp;\+ Notify button state transition [SyRS_09]<br>
\- [SwRS_07] Button Service shall retrieve button state of connected GPIO.[SyRS_06]<br>
\- [SwRS_08] Button Service shall implement chattering prevention with state transition below. [SyRS_06]<br>
![Button State Diagram](https://github.com/gtuja/CSC_MS/blob/main/Resources/Part2/Part2_ALM_StateDiagram_Button.drawio.png)<br>
\- [SwRS_09] Button Service shall extract button events defined in [SyRS_06]<br>
&nbsp;&nbsp;\+ short-press [SyRS_06]<br>
&nbsp;&nbsp;\+ long-press [SyRS_06]<br>
\- [SwRS_10] Button Service shall notify state transition to Button Task. [SyRS_09]<br>
\- [SwRS_11] LED Service shall provide features below.<br>
&nbsp;&nbsp;\+ Output LED [SyRS_08]<br>
&nbsp;&nbsp;\+ Fading [SyRS_08]<br>
&nbsp;&nbsp;\+ Notify LED state transition [SyRS_09]<br>
\- [SwRS_12] LED Service shall implement on, off, fading with state transition below. [SyRS_08]<br>
![LED State Diagram](https://github.com/gtuja/CSC_MS/blob/main/Resources/Part2/Part2_ALM_StateDiagram_LED.drawio.png)<br>
\- [SwRS_13] LED Service shall notify state transition to Button Task. [SyRS_09]<br>
\- [SwRS_14] There are 3 application tasks, Button Task, LED Task, SWD (Serial Wire Debug) Task.<br>
\- [SwRS_15] Button Task shall receive state transition notification from Button Service with IPC, e.g., Queue. [SyRS_09]<br>
\- [SwRS_16] Button Task shall notify state transition to SWD Task with IPC, e.g., Queue. [SyRS_09]<br>
\- [SwRS_17] LED Task shall receive state transition notification from LED Service with IPC, e.g., Queue. [SyRS_09]<br>
\- [SwRS_18] LED Task shall notify state transition to SWD Task with IPC, e.g., Queue. [SyRS_09]<br>
\- [SwRS_19] SWD Task shall receive notifications from Services and Tasks with IPC, e.g., Queue. [SyRS_09]<br>
\- [SwRS_20] SWD Task shall send serial data to connected ST-Link with UART and ST-Link send them to outside world, e.g, PC, with Virtual-Com communication. [SyRS_09]<br>

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