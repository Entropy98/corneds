/*
 * \file main.c
 * \author Harper Weigle
 * \date Dec 18 2023
 * \brief Firmware for corneDS split 40% keyboard
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "debug_uart.h"
#include "hid_task.h"
#include "keymap.h"
#include "led_utils.h"
#include "timeslice.h"
#include "usb_descriptors.h"
#include "usb_detect.h"
#include "xboard_comms.h"

int main(void) {
  board_init();

  led_init();
  tusb_init();
  debug_uart_init();

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
    update_time_slices();
    tud_task();

    if(get_ms_slice() != 0) {
      clear_ms_slice();
    }

    if(get_10ms_slice() != 0) {
      poll_keypresses();
      if(line_state == USB_CONNECTED) {
        send_hid_report();
      }
      clear_10ms_slice();
    }

    if(get_100ms_slice() != 0) {
      if(kbd_side_get() == KBDSIDE_RIGHT){
        led_toggle();
      }
      clear_100ms_slice();
    }

    if(get_s_slice() != 0) {
      if(kbd_side_get() == KBDSIDE_LEFT){
        led_toggle();
      }
      clear_s_slice();
    }

  }

  return 0;
}
