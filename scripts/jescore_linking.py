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
import inspect

print(Color.C + "----------------------------------" + Color.X)
print(Color.M + "*.*.* jescore dynamic linker *.*.*" + Color.X)
print(Color.C + "----------------------------------" + Color.X)

# Get MCU and board info from PlatformIO
board = env.BoardConfig()
mcu = board.get("build.mcu", "").lower()  # e.g., "stm32l476rg"
core = board.get("build.cpu", "").lower()  # e.g., "cortex-m4"
vendor = board.get("build.vendor", "").lower()  # e.g., "st"

print(f"Starting dynamic linking script for jescore on {Color.G + mcu + Color.X} ...")
# ESP32 specific linking is done out-of-the-box
if "esp" in mcu:
    print(" > Found ESP32 platform, defaulting to base config ...")

else:
    # Determine STM32 family (e.g., "stm32l4", "stm32f4")
    stm_family = mcu[:7].lower() if mcu.startswith("stm32") else "unknown"

    freertos_base = f"$PROJECT_PACKAGES_DIR/framework-stm32cube{stm_family[5:7]}"  # e.g., "l4", "f4"

    if "cortex-m4" in core:
        freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    elif "cortex-m33" in core:
        freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure"
    else:
        print(Color.R + f"Warning: Unsupported core {core} for FreeRTOS" + Color.X)
        freertos_port_path = None

    jescore_as_lib = os.path.join(env["PROJECT_LIBDEPS_DIR"], env["PIOENV"], "jescore")
    if os.path.exists(jescore_as_lib):
        config_path = os.path.join(  
            jescore_as_lib,
            "include", 
            "FreeRTOSConfig",  
            "FreeRTOSConfig.h"  
        )  
        env.Append(CPPPATH=[os.path.dirname(config_path)])  
        print(f"Added STM32 FreeRTOSConfig.h from: {config_path}")  
    else:
        env.Append(CPPPATH=["$PROJECT_DIR/include/FreeRTOSConfig"]) 
        print("Local build of library...")

    mcu_flags = [
        f"-mcpu={core}",
        "-mthumb",
        "-DUSE_HAL_DRIVER",
    ]

    env.Append(
        CPPPATH=[
            f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/include",
            f"{freertos_port_path}" if freertos_port_path else "",
        ],
        CCFLAGS=mcu_flags,
        CXXFLAGS=mcu_flags,
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

    fpu_args = [
        "-mfloat-abi=hard",
        "-mfpu=fpv4-sp-d16",
    ]

    if env.get("BUILD_FLAGS", None):
        if fpu_args[0] in env["BUILD_FLAGS"] and fpu_args[1] in env["BUILD_FLAGS"]:
            print("Found FPU args")
            env.Append(LINKFLAGS=fpu_args)

    env.AppendUnique(
        BUILD_FLAGS=mcu_flags
    )

    print(f"MCU: {Color.G + mcu + Color.X}, Core: {Color.G + core + Color.X}, STM Family: {Color.G + stm_family + Color.X}")
    print(f"FreeRTOS Path: {Color.G + freertos_port_path + Color.X}")

    script_path = os.path.dirname(inspect.getfile(inspect.currentframe()))
    lib_root = os.path.abspath(os.path.join(script_path, ".."))
    mcu_dir = join(lib_root, "dynamic_src", mcu)

    if os.path.isdir(mcu_dir):
        dyn_sources = env.Glob(join(mcu_dir, "*.c")) + env.Glob(join(mcu_dir, "*.cpp"))
        if dyn_sources:
            obj_lib = env.BuildObjects(
                target=env.subst(f"$BUILD_DIR/${{PIOENV}}/.dyn_{env.subst('PIOPACKAGE_NAME')}"),
                source=dyn_sources
            )
            env.Append(
                LIBSOURCE_GROUPS={"dynamic": obj_lib},
                LIBS=["dynamic"],
                LINKFLAGS=env.get("LINKFLAGS", [])
            )





def get_git_info():
    try:
        branch = subprocess.check_output(
            ["git", "rev-parse", "--abbrev-ref", "HEAD"],
            stderr=subprocess.STDOUT
        ).decode("utf-8").strip()
    except:
        branch = "unknown"

    try:
        githash = subprocess.check_output(
            ["git", "rev-parse", "--short", "HEAD"],
            stderr=subprocess.STDOUT
        ).decode("utf-8").strip()
    except:
        githash = "unknown"

    return branch, githash

def get_fw_version():
    version = "unknown"
    version_files = [
        "library.json",  # PlatformIO library version
        "package.json",  # Alternative common version file
        "version.txt"    # Simple version file
    ]

    for file in version_files:
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
                pass

    return version

def add_build_flags():
    fw_version = get_fw_version()
    git_branch, git_hash = get_git_info()

    env.Append(
        CPPDEFINES=[
            ("JES_FW_VER", f'\\"{fw_version}\\"'),
            ("JES_FW_BRANCH", f'\\"{git_branch}\\"'),
            ("JES_FW_GIT_HASH", f'\\"{git_hash}\\"')
        ]
    )

    print("\n==== Firmware Build Information ====")
    print(f"Version:  {fw_version}")
    print(f"Branch:   {git_branch}")
    print(f"Commit:   {git_hash}")
    print("=================================\n")

add_build_flags()