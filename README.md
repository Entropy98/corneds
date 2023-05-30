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
 * ~~Add mounting holes~~
 * ~~TRRS jack inverted. The plug fits backwards and upside down~~ -> Made own footprint. Mounting holes are sketchy though
 * ~~Change non-ground test points to circles~~

## Hardware
 * ~~Design Top Plate~~
 * ~~Design Back Plate~~
 * Modify top plate to not cover RPi. It won't fit

## Firmware
 * ~~Make keymap~~
 * ~~Implement GPIO -> HID Report~~
 * ~~Need to implement cross board communication~~
   * Can't implement this well because I can't differentiate the boards with cross board power coming two and from VBUS.
   * I'll try to implement with the inefficiency of both boards attempting to send to the USB Host as well as the other board.
 * Change firmware to use rev2 PCB ports when rev2 is ordered
 * Bug where ; key sends ;l
 * Bug where pressing multiple keys causes both to be sent way too many times
 * Bug where board stops responding after use
 * ~~Alt and Enter aren't working~~
   * ~~Need to test but this was because I wasn't handling those rows in xboard comms~~ -> Nothing works right now
 * ~~Implement shift~~
   * ~~Need to test~~ -> Nothing works right now
