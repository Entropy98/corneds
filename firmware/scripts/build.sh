#!/bin/bash
mkdir -p build
cd build

cmake -DPICO_BOARD=pico -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
cmake --build .
