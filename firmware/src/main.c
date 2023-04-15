/*
 * \file main.c
 * \author Harper Weigle
 * \date Apr 15 2023
 * \brief Firmware for corneDS split 40% keyboard
 */

#include "pico/stdlib.h"

#include "timeslice.h"

#define BOARD_LED 25

uint8_t led_toggle = 1;

int main(void) {

  gpio_init(BOARD_LED);
  gpio_set_dir(BOARD_LED, GPIO_OUT);
  gpio_put(BOARD_LED, 0);

  while(1) {
    update_time_slices();

    if(get_us_slice()) {
      clear_us_slice();
    }

    if(get_ms_slice()) {
      clear_ms_slice();
    }

    if(get_10ms_slice()) {
      clear_10ms_slice();
    }

    if(get_100ms_slice()) {
      clear_100ms_slice();
    }

    if(get_s_slice()) {
      clear_s_slice();
    }

  }

  return 0;
}
