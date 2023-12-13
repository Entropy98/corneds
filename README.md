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
 * Pins on left side didn't save

## Hardware
 * Modify top plate to not cover RPi. It won't fit

## Firmware
 * Tests
   * test debug_print test
     * unclear why uart isn't working
 * Everything is sending with ALT_L
