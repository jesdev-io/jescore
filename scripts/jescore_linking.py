# -----------------------------------
# Name: jescore_linking.py
# Author: jake-is-ESD-protected
# Date: 2025-07-12
# Description: PlatformIO SConsScript that links the FreeRTOS source
# shipped with PlatformIO.
#
# Additional note:
# A STM32 build requires a custom FreeRTOSConfig.h file, which is present
# in the jescore repo. This file can not be included in general, because
# it would overwrite the ESP32 config file. This means that the file has
# to be included depending on platform build type, which this script does.
# However, this process is different for a local build of the library and
# a build as dependency in some other project, because the locations relative
# to the actual project are different. This script obtains the correct path
# to that file by checking if it is present as dependency. If it is, the
# config file resides within .pio/lib_deps/env_name/jescore/include. If it 
# isn't, the build is local and the library itself and the file is under
# include/.
# -----------------------------------


class Color:
   R = '\033[91m'
   G = '\033[92m'
   B = '\033[94m'
   Y = '\033[93m'
   M = '\033[95m'
   C = '\033[96m'
   W = '\033[97m'
   Gr = '\033[90m'
   X = '\033[0m'

Import("env")
import os
import json
import subprocess
from os.path import exists, join
from SCons.Script import Exit
from time import sleep

print(Color.G + "----------------------------------" + Color.X)
print(Color.M + "*.*.* jescore dynamic linker *.*.*" + Color.X)
print(Color.G + "----------------------------------" + Color.X)

# Get MCU and board info from PlatformIO
board = env.BoardConfig()
mcu = board.get("build.mcu", "").lower()  # e.g., "stm32l476rg"
core = board.get("build.cpu", "").lower()  # e.g., "cortex-m4"
vendor = board.get("build.vendor", "").lower()  # e.g., "st"

jescore_as_lib = os.path.join(env["PROJECT_LIBDEPS_DIR"], env["PIOENV"], "jescore")

print(f"Starting dynamic linking script for jescore on {Color.G + mcu + Color.X} ...")
# ESP32 specific linking is done out-of-the-box
if "esp" in mcu:
    if not os.path.exists(jescore_as_lib):
        jescore_as_lib = "."
    print(" > Found ESP32 platform, defaulting to base config ...")

else:
    # Determine STM32 family (e.g., "stm32l4", "stm32f4")
    stm_family = mcu[:7].lower() if mcu.startswith("stm32") else "unknown"

    # FreeRTOS path based on MCU family
    freertos_base = f"$PROJECT_PACKAGES_DIR/framework-stm32cube{stm_family[5:7]}"  # e.g., "l4", "f4"

    # Determine the correct architecture port path for FreeRTOS.
    if "cortex-m0" in core:
        freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0"
    elif "cortex-m3" in core:
        if "cortex-m33" in core:
            freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ"
        else:
            freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3"
    elif "cortex-m4" in core:
        if "cortex-m4+mpu" in core:
            freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4_MPU"
        else:
            freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    elif "cortex-m7" in core:
        if "cortex-m7+mpu" in core:
            freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7_MPU/r0p1"
        else:
            freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1"
    elif "cortex-m23" in core:
        freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM23_NTZ"
    else:
        print(Color.R + f"Warning: Unsupported core {core} for FreeRTOS" + Color.X)
        freertos_port_path = None

    # Check if jescore is used as library and append the FreeRTOS config.
    is_lib = False
    if os.path.exists(jescore_as_lib):
        config_path = os.path.join(  
            jescore_as_lib,
            "include", 
            "FreeRTOSConfig",  
            "FreeRTOSConfig.h"  
        )  
        env.Append(CPPPATH=[os.path.dirname(config_path)])  
        print(f"Added STM32 FreeRTOSConfig.h from: {config_path}")  
        is_lib = True
    else:
        env.Append(CPPPATH=["$PROJECT_DIR/include/FreeRTOSConfig"]) 
        print("Local build of library...")
        jescore_as_lib = "."

    mcu_flags = [
        f"-mcpu={core}",
        "-mthumb",
        "-DUSE_HAL_DRIVER",
    ]

    fpu_args = [
        "-mfloat-abi=hard",
        "-mfpu=fpv4-sp-d16",
    ]

    # Fail in case that FPU specifiers are not set from outside (can't be set from in here)
    help_msg = Color.R + "Please add `build_flags = -mfloat-abi=hard -mfpu=fpv4-sp-d16` to your `platformio.ini` when building jescore for STM32!" + Color.X
    if env.get("BUILD_FLAGS", None):
        if not (fpu_args[0] in env["BUILD_FLAGS"] and fpu_args[1] in env["BUILD_FLAGS"]):
            sleep(0.001)
            Exit(help_msg)
    else:
        sleep(0.001)
        Exit(help_msg)

    env.Append(
        CPPPATH=[
            f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/include",
            f"{freertos_port_path}" if freertos_port_path else "",
        ],
        CCFLAGS=mcu_flags,
        CXXFLAGS=mcu_flags,
        LINKFLAGS=fpu_args
    )

    env.BuildSources(
        "$BUILD_DIR/FreeRTOS_src",
        f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source",
        src_filter=["+<*.c>"]
    )

    env.BuildSources(
        "$BUILD_DIR/FreeRTOS_port",
        f"{freertos_port_path}" if freertos_port_path else "",
        src_filter=["+<*.c>"]
    )

    env.BuildSources(
        "$BUILD_DIR/FreeRTOS_mem",
        f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang",
        src_filter=["-<*.c>", "+<heap_4.c>"]
    )

    print(f"MCU: {Color.G + mcu + Color.X}, Core: {Color.G + core + Color.X}, STM Family: {Color.G + stm_family + Color.X}")
    print(f"FreeRTOS Path: {Color.G + freertos_port_path + Color.X}")


def get_fw_version():
    version = "unknown"
    version_files = [
        "library.json",  # PlatformIO library version
        "package.json",  # Alternative common version file
        "version.txt"    # Simple version file
    ]

    for file in version_files:
        file = join(jescore_as_lib, file)
        print("DEBUGG: ", file)
        if exists(file):
            
            try:
                with open(file) as f:
                    if file.endswith('.json'):
                        data = json.load(f)
                        version = data.get("version", version)
                    else:
                        version = f.read().strip()
                break
            except:
                print("sus")
                # pass

    return version

def add_build_flags():
    fw_version = get_fw_version()

    env.Append(
        CPPDEFINES=[
            ("JES_FW_VER", f'\\"{fw_version}\\"')
        ]
    )

    print("\n==== jescore Build Information ====")
    print(f"Version:  {Color.G + fw_version + Color.X}")
    print("=====================================\n")

add_build_flags()