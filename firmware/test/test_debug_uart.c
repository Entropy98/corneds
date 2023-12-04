/*
 * \file test_debug_uart.c
 * \author Harper Weigle
 * \date Nov 13 2023
 * \brief Testing the debug uart interface
 *        Prints "Hello World\n" over the debug UART interface
 */

#include "bsp/board.h"
#include "pico/stdlib.h"

#include "debug_uart.h"
#include "timeslice.h"
#include "led_utils.h"

int main(void) {
  led_init();
  board_init();
  init_time_slices();
  debug_uart_init();

  while(true){
    update_time_slices();
    tud_task();

    if(get_ms_slice() != 0) {
      clear_ms_slice();
    }

    if(get_10ms_slice() != 0) {
      clear_10ms_slice();
    }

    if(get_100ms_slice() != 0) {
      clear_100ms_slice();
    }

    if(get_s_slice() != 0) {
      debug_print("Hello world\n");
      led_toggle();
      clear_s_slice();
    }
  }
}
