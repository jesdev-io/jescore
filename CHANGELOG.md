# Changelog

## 2.3.2-post.1 - 2026-07-29

### Fixed
- 🚢 Release automation now creates tags only after deployment succeeds
- 🔌 Remote test runner USB mappings corrected from live `rpi-cm5-1` device inventory
- 🧪 STM32 unregister test no longer allocates an extra loop job on constrained boards
- ⏱️ STM32 tests wait longer for slower boards to drain burst-launched jobs and enter runtime environments
- 🧠 STM32G431 uses smaller per-job heap to fit burst tests on the constrained board

## 2.3.2 - 2026-07-29

### Added
- 🛡️ API init protection
- 🖥️ Physical remote deployer
- 🖨️ Better printing

### Fixed
- 📜 GCC version for STM32 builds pinned
- 🛑 API access denial for core level jobs
- ❓️ Core jobs no longer lost thanks to queue

## 2.3.1 - 2026-05-20 **The safety update**

### Added
- 📜 Static analysis script
- ⏲️ UART timeout for STM32
- 🔧 Job unregistration API
- 🔒 Self-closing mutex context
- ⏱️ Core timeout mechanism
- 🛡️ Experimental Daisy Seed H7 support (the hardware, not the library)

### Fixed
- 🪨 `const` parameter declarations where appropriate
- ✒️ ~~`sprintf`~~ -> `snprintf`
- 📦 Increased stats descriptor size
- 🔄 Switched to PortMalloc
- 🔒 Core and job task locking
- 🚫 Null dereference prevention
- 🏃 Faster return on parameter errors

### Changed
- 🔆 Core job has priority 0
- 📝 Updated README with board list and makefile references
- 📋 Noted Cortex M0 series exclusion

## 2.3.0 - 2026-04-21

### Added
- 🏹 API CHANGE: "singleton" argument in job registration
- 🖨️ New `jes_print()` macro for formatted strings
- 🔍 `jes_print()` identifies calling job in print statement

### Fixed
- 💬 ESP32 UART driver doesn't behave faulty when initialized twice

### Removed
- 👷 ESP32 `esp-builtin` specifier removed for C3

## 2.2.3 - 2025-11-23

### Added
- 🖥️ Inline flag for CLI responses to reuse same line
- 🔆 New logo banner and function diagram 
- 📜 Short installation paragraph in main README
- 🚫 `e_err_NUM_ERR` enum for better "inheritance" of errors
- 🔢 Baudrate of firmware now settable with `CLI_BAUDRATE` macro

### Fixed
- 🚫 Notifying a non-existent job will now no longer crash

### Removed
- 🐍 `jescorecli` python wrapper now moved to own [repo](https://github.com/jesdev-io/jescorecli)

## 2.2.2 - 2025-10-07

### Added
- 🔆 Support for ESP32-NodeMCU
- 🛡️ Core and job task lock
- 📜 Compilation now outputs `compile_commands.json` for clangd

### Fixed
- 💡 LED pin definitions for boards without `LED_BUILTIN`

## 2.2.1 - 2025-09-29

### Added
- 💬 UART queue commands for invalid data and CLI session closing
- 🌊 Early termination on CLI input overflow 
- 🔆 Core now checks for errors in job **before** launch as well
- 🛡️ Macros for generic resource protection
- ‼️ UART double buffer to stop DMA override of burst data

### Changed
- 🚫 Shorter error messages
- 🧑‍🌾 Reduced heap size for STM32 projects

### Fixed
- 🔆 Core initializer now calls all registrations in one place 
- 🔢 `jescore dynamic linker` adds the FW version to ESP32 builds again

### Removed
- 🛠️ Public reference to CLI helper functions
- ❌ `errorhandler` is now inline and no longer a job
- 😺 `jescore dynamic linker` will no longer log git information

## 2.2.0 - 2025-09-23

### Added
- ⏲️ Timing hooks in runtime environment
- ⏲️ `bench` base job for showing job execution time
- 📖 Docs on `bench` job
- 📜 Logging type macros `JES_LOG_REGISTER`, `JES_LOG_LAUNCH`, `JES_LOG_FINISH`, `JES_LOG_FAULT`
- 🐍 Python unit tests for `bench` job CLI interaction
- ✒️ Missing CI builds for examples 

### Changed
- 🚫 Errors are now displayed with their `enum` in brackets

### Fixed
- ❌ The `jescore dynamic linker` will now exit correctly on error with `Exit()` instead of `env.Fatal()`
- 🪝 Logging hooks have been moved to let `logp` accurately see the amount of instances
- 🪲 `uart_unif_writef()` works again

## 2.1.0 - 2025-09-08

### Added
- ➡️ This changelog
- ⌨️ CP2102 UART-USB now known by CLI with `jescore -d`
- ⌨️ USB SIngle Serial UART-USB now known by CLI with `jescore -d`
- 🔆 Support for ESP32-WROVER
- 🖥️ Open UART stream support from CLI with `jescore -l`
- 🕐️ `systime`-getter in backend
- 📜 Core logging with log printer job `logp`
- 📖 Docs on `-l` command
- 📖 Docs on `logp` job
- ⏲️ Sync/Async example

### Changed
- 🏃 Default FreeRTOS tick rate from 100 to 1000 (10 times faster)
- 💬 CLI server job renamed to `clisrv`
- 🐍 Python backend method `discoverPorts` now returns all ports as list 


### Fixed
- 🐞 Any CLI call will now display nothing in case that no `jescore`-compatible device is connected instead of a confusing python error

### Removed
- 🚮 `jescore stats -a` will no longer print the git branch of `jescore`