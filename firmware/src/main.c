/*
 * \file main.c
 * \author Harper Weigle
 * \date Apr 18 2023
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

int main(void) {

  led_init();
  init_time_slices();
  board_init();
  tusb_init();
  init_keys();

  uint8_t line_state = usb_detected();

  if(line_state == 0){
    led_on();
  }
  while(1) {
    update_time_slices();
    tud_task();

    if(get_ms_slice() != 0) {
      clear_ms_slice();
    }

    if(get_10ms_slice() != 0) {
      poll_keypresses();
      send_hid_report(get_keypress());
      clear_10ms_slice();
    }

    if(get_100ms_slice() != 0) {
      if(line_state == 0x4){
        led_toggle();
      }
      clear_100ms_slice();
    }

    if(get_s_slice() != 0) {
      if(line_state == 0xc){
        led_toggle();
      }
      clear_s_slice();
    }

  }

  return 0;
}
