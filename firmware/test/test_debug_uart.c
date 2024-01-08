/*
 * \file test_debug_uart.c
 * \author Harper Weigle
 * \date Dec 06 2023
 * \brief Testing the debug uart interface
 *        Prints "Hello World\n" over the debug UART interface
 */

#include "bsp/board.h"
#include "pico/stdlib.h"

#include "debug_uart.h"
#include "led_utils.h"

int main(void) {
  board_init();

  led_init();
  debug_uart_init();

  while(true){
    tud_task();
    debug_print("Hello world\n");
  }
}
