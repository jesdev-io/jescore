# Changelog

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
- `jescore dynamic linker` adds the FW version to ESP32 builds again

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