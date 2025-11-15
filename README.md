<img width="1920" height="300" alt="jescore_logo_banner" src="https://github.com/user-attachments/assets/2fc4f696-0a6c-444b-a99b-053f9bee6d59" />

# `jescore`

**J's Embedded Systems Core**: a convenient, dynamic and portable wrapper for FreeRTOS with native CLI support running on embedded systems with a central core unit that makes adding, debugging and abstracting new features easy.

[![PlatformIO CI](https://github.com/jake-is-ESD-protected/jescore/actions/workflows/workflow.yml/badge.svg?branch=main)](https://github.com/jake-is-ESD-protected/jescore/actions/workflows/workflow.yml)

![jescore_blink](https://github.com/user-attachments/assets/d180d1a6-2823-451f-9092-45aa2639c013)

## Key-concepts
`jescore` is a dynamic abstraction library for commonly used FreeRTOS tasks written in C. Its goal is to make development of multitasking procedures easier with more focus on the actual application than the **boring** backend-stuff. `jescore` does that for you. A second goal is native CLI support: As huge fan of CLIs, I also want this functionality on my embedded systems, to better interact with the device and to test software without hardware. This way, graphical UI elements become naturally abstracted from the actual process. On a desktop OS, calling `cd folder` is exactly the same as double clicking on that folder with the cursor, but the second method provides a good looking UI, then calls `cd folder` by itself. `jescore` follows a similar philosophy.

<img width="902" height="592" alt="jescore_program drawio 5" src="https://github.com/user-attachments/assets/a53cc14c-568d-4f03-9e90-21fb231bbefb" />

`jescore` works by launching a given **"job"** in a runtime environment that is then executed in a multitasking context. The core handles calls to these jobs and checks if they have been registered by the user. Such a call can happen in the form of the [API](https://github.com/jesdev-io/jescore/wiki/API-Documentation) from within the program or the [CLI](https://github.com/jesdev-io/jescore/wiki/CLI-Documentation) if the MCU is connected to a host machine. This means that a program can be written that has **automatic and native** CLI support for **automation tasks**, **in-situ debugging** or **backend-access** that the UI of a project doesn't allow. It additionally implements so called [**"base jobs"**](https://github.com/jesdev-io/jescore/wiki/CLI-Documentation#built-in-jobs), which offer additional functionality like **job status**, **logging** and **benchmarking**.

## Why?
Many times I find myself needing the same type of overarching architecture for embedded systems projects. This includes things like multitasking, interrupts, sampling tasks and user input tasks. As of now, every project consists of me setting up this architecture manually for every project, always slightly different with new knowledge added. At one point I stopped and thought, why not outsource this specific framework into its own project, software, module... Something that is always at the center of my projects: **jescore**.

The great thing about this name is that it can stand for "**J**'s **e**mbedded **s**ystems **core**" but it can also be an homage to the legacy YouTube name "**J's e-shack**", which inspired this core system in the first place.

## Currently supported platforms
|ESP32|STM32|
|-|-|
|ESP32|STM32L476|
|ESP32-C3|STM32L432|
|ESP32-S3|STM32G431|
|ESP32-WROVER|STM32H753|

## Get started, examples and use-cases

- [Examples](https://github.com/jake-is-ESD-protected/jescore/wiki/Examples)
- [API documentation](https://github.com/jake-is-ESD-protected/jescore/wiki/API-Documentation)
- [Gallery](https://github.com/jake-is-ESD-protected/jescore/wiki#check-the-stats-of-your-program)

All that stuff is explained in detail in the [wiki](https://github.com/jake-is-ESD-protected/jescore/wiki).

## Find this stuff useful? Consider a donation!
[![Buy me a coffee](https://img.shields.io/badge/Ko--fi-Support%20Me-FF5E5B?style=for-the-badge&logo=ko-fi&logoColor=white)](https://ko-fi.com/jseshack) 

## Disclaimer
The software shown here is how **I like to structure my projects**. You might disagree with the way the code is structured. If you would like to try this code out as well and you are not happy with certain features, you are very welcome to contribute. I keep this core updated to always reflect my current coding style for embedded systems. For any legal conditions regarding this software, see the [license](LICENSE).
