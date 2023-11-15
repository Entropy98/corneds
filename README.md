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
 * Debug UART has CTS and VCC switched
 * UART on GP6 and GP7 but need to be on GP4 and GP5 to be on a UART bus
 * Add resistor to determine left or right
 * Add reset button

## Hardware
 * Modify top plate to not cover RPi. It won't fit

## Firmware
 * Tests
   * test debug_print test
    * debug uart on wrong ports
   * write test to determine which board is which
    * Need a pull-up/down resistor to determine which side is left or right
 * Everything works but multiple keys is hella buggy
