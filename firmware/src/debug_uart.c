/*
 * \file debug_uart.c
 * \author Harper Weigle
 * \date Dec 06 2023
 * \brief uart print functions
 */
#ifndef _SRC_DEBUG_UART_C
#define _SRC_DEBUG_UART_C

#include "pico/stdlib.h"
#include "hardware/uart.h"

#include "pinmap.h"

#define DEBUG_UART_ID uart1
#define BAUD_RATE 115200

/*
 * \fn void xdebug_uart_init()
 * \brief initializes the board for communicating over the debug uart interface
 */
void debug_uart_init(){
  uart_init(DEBUG_UART_ID, BAUD_RATE);
  gpio_set_function(DEBUGUARTTX_PIN, GPIO_FUNC_UART);
  gpio_set_function(DEBUGUARTRX_PIN, GPIO_FUNC_UART);
  uart_set_hw_flow(DEBUG_UART_ID, false, false);
  uart_set_format(DEBUG_UART_ID, 8, 1, UART_PARITY_NONE);
  uart_set_fifo_enabled(DEBUG_UART_ID, false);

}

/*
 * \fn void print_debug(const char* s)
 * \param s string to print
*/
void debug_print(const char* s){
  uart_puts(DEBUG_UART_ID, s);
}

#endif // _SRC_DEBUG_UART_C
