#!/bin/bash

# Static Analysis Script for jescore
# Run this to check for potential issues in the codebase

CPPCHECK_PATH="/home/j/.platformio/packages/tool-cppcheck/cppcheck"

if [ ! -x "$CPPCHECK_PATH" ]; then
    echo "Error: cppcheck not found at $CPPCHECK_PATH"
    exit 1
fi

echo "Running static analysis with cppcheck..."
echo "=========================================="

# Run cppcheck on all source files
"$CPPCHECK_PATH" \
    --enable=all \
    --inconclusive \
    --std=c11 \
    -I include \
    -I lib \
    lib/ \
    test/

exit_code=$?

echo "=========================================="
echo "Static analysis complete (exit code: $exit_code)"

exit $exit_code
