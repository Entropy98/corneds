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
#include "timeslice.h"
#include "usb_descriptors.h"

#define BOARD_LED 25

int main(void) {

  gpio_init(BOARD_LED);
  gpio_set_dir(BOARD_LED, GPIO_OUT);
  gpio_put(BOARD_LED, 0);
  init_time_slices();
  board_init();
  tusb_init();

  uint32_t key = 0;
  uint8_t led_toggle = 1;

  while(1) {
    update_time_slices();
    tud_task();

    if(get_ms_slice() != 0) {
      clear_ms_slice();
    }

    if(get_10ms_slice() != 0) {
      send_hid_report(0);
      clear_10ms_slice();
    }

    if(get_100ms_slice() != 0) {
      clear_100ms_slice();
    }

    if(get_s_slice() != 0) {
      gpio_put(25, led_toggle);
      led_toggle = led_toggle ? 0 : 1;
      key = key ? 0 : 1;
      clear_s_slice();
    }

  }

  return 0;
}
