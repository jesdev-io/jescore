# -----------------------------------
# Name: freertos_linking.py
# Author: jake-is-ESD-protected
# Date: 2025-07-12
# Description: PlatformIO SConsScript that links the FreeRTOS source
# shipped with PlatformIO.            
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

   # FreeRTOS paths (adjust based on MCU)
   freertos_base = f"$PROJECT_PACKAGES_DIR/framework-stm32cube{stm_family[5:7]}"  # e.g., "l4", "f4"

   if "cortex-m4" in core:
      freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
   elif "cortex-m33" in core:
      freertos_port_path = f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure"
   else:
      print(Color.R + f"Warning: Unsupported core {core} for FreeRTOS" + Color.X)
      freertos_port_path = None

   if os.path.exists(os.path.join(env["PROJECT_LIBDEPS_DIR"], env["PIOENV"], "jescore")):
      config_path = os.path.join(  
         env.PioPlatform().get_package_dir("jescore"),
         "include", 
         "FreeRTOSConfig",  
         "FreeRTOSConfig.h"  
      )  
      env.Append(CPPPATH=[os.path.dirname(config_path)])  
      print(f"Added STM32 FreeRTOSConfig.h from: {config_path}")  
   else:
      env.Append(CPPPATH=["$PROJECT_DIR/include/FreeRTOSConfig"]) 
      print("Local build of library...")

   env.Append(
      CPPPATH=[
         f"{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/include",
         f"{freertos_port_path}" if freertos_port_path else "",
      ],

      CCFLAGS=[
         "-mfloat-abi=hard",
         "-mfpu=fpv4-sp-d16",
         f"-mcpu={core}",
         "-mthumb",
         "-DUSE_HAL_DRIVER",
      ],

      LINKFLAGS=[
         "-mfloat-abi=hard",
         "-mfpu=fpv4-sp-d16"
      ],

      SRC_FILTER=[
         "+<**/*.c>",
         f"+<{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/*.c>",
         f"+<{freertos_port_path}/*.c>" if freertos_port_path else "",
         f"+<{freertos_base}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c>",
      ],
   )

   print(f"MCU: {Color.G + mcu + Color.X}, Core: {Color.G + core + Color.X}, STM Family: {Color.G + stm_family + Color.X}")
   print(f"FreeRTOS Path: {Color.G + freertos_port_path + Color.X}")