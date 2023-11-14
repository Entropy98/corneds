/*
 * \file debug_uart.c
 * \author Harper Weigle
 * \date Nov 12 2023
 * \brief uart print functions
 */

#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart1
#define BAUD_RATE 115200

#define UART_TX_PIN 4
#define UART_RX_PIN 5


/*
 * \fn void xdebug_uart_init()
 * \brief initializes the board for communicating over the debug uart interface
 */
void debug_uart_init(){
  uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
}

/*
 * \fn void print_debug(const char* s)
 * \param s string to print
*/
void debug_print(const char* s){
  #ifdef DEBUG
  uart_puts(UART_ID, s);
  #endif
}
