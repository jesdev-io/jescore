# Changelog

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