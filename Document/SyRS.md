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
- Evaluation report may be used as criteria for the SyRS even the SR.
- Keep in mind [Golden ratio](https://en.m.wikipedia.org/wiki/Golden_ratio) and [Affordance](https://en.m.wikipedia.org/wiki/Affordance), divide and conquer with [Occam's razor](https://en.m.wikipedia.org/wiki/Occam%27s_razor). 

</details>

<div id="SwRS"></div>
<details open>
<summary><font size="5"><b>Software Requirement Specification</b></font></summary>

- [TOC](#toc)
- The **software block diagram** shall be below.<br>
![Software Block Diagram](https://github.com/gtuja/CSC_MS/blob/main/Resources/Part2/Part2_ALM_SoftwareBlockDiagram.drawio.png)<br>
- GSL is comprised of 6 software modules, i.e., CONFIG, GSL API, GSL, NOOS, XSM, DIAG.
- Those relationship among modules and UA are described as HLD below.
![High Level Design](https://github.com/gtuja/CSC_MS/blob/main/Resources/Part2/Part2_ALM_SoftwareBlockDiagram.drawio.png)<br>
- CONFIG shall provide interfaces, i.e., defines, data types, APIs, between GSL and UA.
- As GSL is platform independent library, platform specific features are defined at this module.
- UA shall redefine defines and implement APIs on the HLD.
- GSL API is comprised of APIs provided by GSL.
- UA shall call those APIs in place on the HLD.
- GSL plays counter role between UA and GSL.
- GSL shall implement GSL API and invoke GSL modules.
- NOOS shall provide os-like features, e.g., PSM, BTM, Queue.
- PSM shall be responsible for periodic services, e.g., XSM, triggered by UA.
- PSM shall also provide diagnostic feature for system feasibility, e.g., occupation time.
- BPM shall be responsible for background processes triggered by UA.
- Time consuming peocesses, e.g., serial communication, E2P, shall be done wirh IPC method, e.g., Queue of NOOS.
- XSM is eXtended Service Manager for 
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