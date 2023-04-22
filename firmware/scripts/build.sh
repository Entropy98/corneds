#!/bin/bash
mkdir -p build/right
cd build/right
cmake -DPICO_BOARD=pico -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DKBDSIDE=right ../..
cmake --build .

cd ..
mkdir -p left
cd left

cmake -DPICO_BOARD=pico -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DKBDSIDE=left ../..
cmake --build .
