# jes-core
J's Embedded Systems Core: a convenient, dynamic and portable wrapper for FreeRTOS running on embedded systems with a central core unit that makes adding, debugging and abstracting new features easy.

##### **[About](#about)** | **[Key-concepts](#key-concepts)** | **[Use-cases](#use-cases)** | **[Known issues](#known-issues)** | **[Disclaimer](#disclaimer)**

## About
Many times I find myself needing the same type of overarching architecture for embedded systems projects. This includes things like multitasking, interrupts, sampling tasks and user input tasks. As of now, every project consists of me setting up this architecture manually for every project, always slightly different with new knowledge added. At one point I stopped and thought, why not outsource this specific framework into its own project, software, module... Something that is always at the center of my projects: **jes-core**.

The great thing about this name is that it can stand for "**J**'s **e**mbedded **s**ystems **core**" but it can also be an homage to the legacy YouTube name "**J's e-shack**", which inspired this core system in the first place.

## Key-concepts
`jes-core` is a dynamic abstraction library for commonly used FreeRTOS tasks written in C. Its goal is to make development of multitasking procedures easier with more focus on the actual application than the **boring** backend-stuff. `jes-core` does that for you. A second goal is native CLI support: As huge fan of CLIs, I also want this functionality on my embedded systems, to better interact with the device and to test software without hardware. This way, graphical UI elements become naturally abstracted from the actual process. On a desktop OS, calling `cd folder` is exactly the same as double clicking on that folder with the cursor, but the second method provides a good looking UI, then calls `cd folder` by itself. `jes-core` follows a similar philosophy.
### Jobs
Many names are given to digital procedures that occur on a system in the context of multitasking. FreeRTOS calls them "tasks", POSIX calls them "threads" and so on. To not confuse a FreeRTOS task with the processes that happen in `jes-core`, they are called "**jobs**". These jobs are connected to one or more FreeRTOS tasks, but they are more than just wrappers. A job exists as a `struct`, being nothing more than a static variable holding descriptions of what to do in case of invocation. Below is such a job `struct`:
```c
typedef struct job_struct_t{
    char name[__MAX_JOB_NAME_LEN_BYTE] = {0};
    TaskHandle_t handle = NULL;
    uint32_t mem_size = 0;
    uint8_t priority = 0;
    void (*function) (void* p) = NULL;
    char args[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    bool is_loop = false;
    uint8_t instances = 0;
    e_role_t role = e_role_core;
    origin_t caller = e_origin_undefined;
    void* optional = NULL;
    jes_err_t error = e_err_no_err;
    struct job_struct_t* pn = NULL;
}job_struct_t;
```

#### Parameters
#####  `name`
Notice the emphasis on on a dedicated string buffer. Not only does FreeRTOS require each task to have a name for debugging and tracing, but `jes-core` gives it a second purpose. If desired, the job can be launched via the mentioned CLI by this exact name, making CLI integration **native**.
##### `handle`
This is the same handle you would expect from FreeRTOS. It has exactly that same usage.
##### `mem_size`
Allocated dynamic memory for the job in bytes. Is used by FreeRTOS.
##### `priority`
Priority of the task. Be careful to give tasks that are always active the same priority, otherwise they won't be executed in parallel! This is also used by FreeRTOS.
##### `function`
##### `args`
##### `is_l0op`
##### `instances`
##### `role`
##### `caller`
##### `optional`
##### `error`
##### `pn`
Now for the coolest thing: Job-structs themselves are arranged in a **linked list**. As soon as you register them, they become a member of the list which gives `jes-core` the ability to create a **journal** of the specified jobs. You can search through this journal and get every job, status, resource... Whatever you wish to get. If this reminds you somewhat of system-daemons on Linux, then I have reached my goal!
### CLI
>Expected command shapes:
>`<name> <arg> [args]`
### User function integration
## Use-cases
`jes-core` can be used for a variety of applications for and on embedded systems. Since multitasking and CLI are native, it opens up some possibilities for cool cross-platform applications.
### Finite state machine
Does your project involve user-triggered tasks with sleep time in between? Perfect for `jes-core`! Use interrupts to launch jobs that process your requests before going to sleep again. Since `jes-core` deallocates memory for jobs which are done, you can expect that your sleep-overhead is close to **zero**.
### Sampler with user input
Many devices in the realm of media streaming or measurement require a constant stream of data but also have to react to user input during runtime. Think of a digital oscilloscope which has to adjust its trigger value according to the users input while data is being sampled. `jes-core` can take care of that. Define a sampler-job with a `while(1)` and UI-handler job which gets triggered by an interrupt, adjust some values within your trigger-job and exit.
### Unit testing
Native CLI support enables easy cross-platform unit-testing. Using `pyserial` for python you can send commands to `jes-core` via serial. This way, you can easily test your embedded systems internal calculation processes without being physically present.

## Known issues
### Interrupts
Since interrupts are managed by the specific hardware and not FreeRTOS, `jes-core` can't cover them in a concise manner. You can notify jobs from within an interrupt by setting the common parameter `from_ISR = true`, but other than that you will need your own ISRs and register them according to your system. If the CLI is enabled, the controller makes use of the systems UART interrupts or any similar mechanism.
### Job runtime environment
The world is your oyster. This also means that `jes-core` won't check what sort of devilish things you do in the jobs you give to it and will happily run against a wall if you do something nasty like this `uint8_t lolxd = lol_array[MAX_LOL_ARRAY_LEN]`. This will provoke your controllers natural error mechanisms and `jes-core` will die at this point.
### Dynamic memory & strings
Since `jes-core` creates and ends jobs during runtime, it uses dynamic memory and takes a bit longer to start a job. Additionally, the native CLI support uses a lot of internal memory for strings which could just be `enums` without the CLI. If your goal is to build a fast, static and memory efficient software, the standard `jes-core` might not be the best option for you. However, you can check out `jes-core-light` or `jes-core-mini`if you want to reduce or remove CLI support or dynamic capabilities.
## Disclaimer
The software shown here is how **I like to structure my projects**. You might disagree with the way the code is structured. If you would like to try this code out as well and you are not happy with certain features, you are very welcome to contribute. I keep this core updated to always reflect my current coding style for embedded systems.