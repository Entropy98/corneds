/*
 * \file led_blinky.c
 * \author Harper Weigle
 * \date 15 Jan 2024
 * \brief Blinks the main LED on the board to verify timing intervals
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "led_utils.h"
#include "timing_arch.h"


int main(void) {
  led_init();
  board_init();
  timing_arch_init();

  led_on();

  while(true){
    if(ms100_loop_check()) {
      led_toggle();
    }
  }
}
