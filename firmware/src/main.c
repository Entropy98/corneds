/*
 * \file main.c
 * \author Harper Weigle
 * \date Nov 16 2023
 * \brief Firmware for corneDS split 40% keyboard
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "hid_task.h"
#include "keymap.h"
#include "led_utils.h"
#include "timeslice.h"
#include "usb_descriptors.h"
#include "usb_detect.h"
#include "xboard_comms.h"

int main(void) {

  led_init();
  init_time_slices();
  board_init();
  tusb_init();
  xboard_comms_init();

  uint8_t line_state = usb_detected();

  init_keys(line_state == USB_CONNECTED);
  while(1) {
    update_time_slices();
    tud_task();

    if(get_ms_slice() != 0) {
      poll_keypresses();
      if(line_state == USB_CONNECTED) {
        send_hid_report(get_keypress());
      }
      clear_ms_slice();
    }

    if(get_10ms_slice() != 0) {
      clear_10ms_slice();
    }

    if(get_100ms_slice() != 0) {
      if(line_state == USB_CONNECTED){
        led_toggle();
      }
      clear_100ms_slice();
    }

    if(get_s_slice() != 0) {
      clear_s_slice();
    }

  }

  return 0;
}
