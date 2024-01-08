/*
 * \file main.c
 * \author Harper Weigle
 * \date Jan 08 2024
 * \brief Firmware for corneDS split 40% keyboard
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "debug_uart.h"
#include "hid_task.h"
#include "keymap.h"
#include "led_utils.h"
#include "timing_arch.h"
#include "usb_descriptors.h"
#include "usb_detect.h"
#include "xboard_comms.h"

int main(void) {
  board_init();

  led_init();
  tusb_init();
  debug_uart_init();
  timing_arch_init();

  uint8_t line_state = usb_detected();

  xboard_comms_init(line_state == USB_CONNECTED);
  init_keys(line_state == USB_CONNECTED);

  debug_print("CorneDS 40% Keyboard\n");
  if(line_state == USB_CONNECTED){
    debug_print("Initializing with USB Connected\n");
  }
  else {
    debug_print("Initializing with USB Disconnected\n");
  }

  if(kbd_side_get() == KBDSIDE_RIGHT) {
    debug_print("Initializing as Right Side\n");
  }
  else {
    debug_print("Initializing as Left Side\n");
  }

  while(1) {
    tud_task();
  }

  return 0;
}
