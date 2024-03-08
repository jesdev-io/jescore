# jes-core
J's Embedded Systems Core: a convenient, dynamic and portable wrapper for FreeRTOS running on embedded systems with a central core unit that makes adding, debugging and abstracting new features easy.

[![PlatformIO CI](https://github.com/jake-is-ESD-protected/jes-core/actions/workflows/workflow.yml/badge.svg?branch=main)](https://github.com/jake-is-ESD-protected/jes-core/actions/workflows/workflow.yml)

## Key-concepts
`jes-core` is a dynamic abstraction library for commonly used FreeRTOS tasks written in C. Its goal is to make development of multitasking procedures easier with more focus on the actual application than the **boring** backend-stuff. `jes-core` does that for you. A second goal is native CLI support: As huge fan of CLIs, I also want this functionality on my embedded systems, to better interact with the device and to test software without hardware. This way, graphical UI elements become naturally abstracted from the actual process. On a desktop OS, calling `cd folder` is exactly the same as double clicking on that folder with the cursor, but the second method provides a good looking UI, then calls `cd folder` by itself. `jes-core` follows a similar philosophy.

## Why?
Many times I find myself needing the same type of overarching architecture for embedded systems projects. This includes things like multitasking, interrupts, sampling tasks and user input tasks. As of now, every project consists of me setting up this architecture manually for every project, always slightly different with new knowledge added. At one point I stopped and thought, why not outsource this specific framework into its own project, software, module... Something that is always at the center of my projects: **jes-core**.

The great thing about this name is that it can stand for "**J**'s **e**mbedded **s**ystems **core**" but it can also be an homage to the legacy YouTube name "**J's e-shack**", which inspired this core system in the first place.

## Get started, examples and use-cases

- [Examples](https://github.com/jake-is-ESD-protected/jes-core/wiki/Examples)
- [API documentation](https://github.com/jake-is-ESD-protected/jes-core/wiki/API-Documentation)
- [Gallery](https://github.com/jake-is-ESD-protected/jes-core/wiki#check-the-stats-of-your-program)

All that stuff is explained in detail in the [wiki](https://github.com/jake-is-ESD-protected/jes-core/wiki).

## Disclaimer
The software shown here is how **I like to structure my projects**. You might disagree with the way the code is structured. If you would like to try this code out as well and you are not happy with certain features, you are very welcome to contribute. I keep this core updated to always reflect my current coding style for embedded systems.
