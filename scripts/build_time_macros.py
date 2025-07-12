# -----------------------------------
# Name: build_time_macros.py
# Author: jake-is-ESD-protected
# Date: 2025-07-12
# Description: PlatformIO SConsScript that fetches firmware version,
# branch and commit hash and converts it to firmware macros.            
# -----------------------------------


Import("env")

import json
import subprocess
from os.path import exists

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