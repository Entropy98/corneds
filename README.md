# Dependencies
 * PyOCD
 * Pico-SDK
 * gdb-multiarch

# Flashing
1. run `./scripts/flash.sh ./build/corneds_<side>.elf` in the firmware directory
2. Enter `target remote localhost:3333` to connect to started gdb server
**cannot connect with gdb-multiarch --pid argument due to ptrace vulnerability in system**


# To Do
## PCB
 * Mounting holes are way too big. The units are probably wrong in KiCAD. They should be shrunk

## Hardware
 * ~~Design Top Plate~~
 * ~~Design Back Plate~~
 * Modify top plate to not cover RPi. It won't fit

## Firmware
 * Tests
   * print passes
   * test debug_print test
   * write keypress test
   * write test to determine which board is which
