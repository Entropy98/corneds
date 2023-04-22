#!/bin/bash
cd build
cmake -DPICO_BOARD=pico -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DKBDSIDE=$1 ..
cmake --build .
