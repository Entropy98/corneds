/*
 * \file xboard_comms.c
 * \author Harper Weigle
 * \date May 11 2023
 * \brief interfacing between sides of the keyboard
 */

#ifndef _SRC_XBOARD_COMMS_C
#define _SRC_XBOARD_COMMS_C

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "keymap.h"
#include "pinmap.h"
#include "xboard_comms.h"

#define MOD_BIT   0x80
#define SHIFT_BIT 0x40
#define ROW_MASK  0x30
#define COL_MASK  0x0F

#define BAUD_RATE 115200

/*
 * \fn void rx_irq()
 * \brief Pushes a key to the key buffer on UART interrupt. Inspired from pico sdk example
 */
void rx_irq(){
  while(uart_is_readable(uart0)){
    uint8_t pkt = uart_getc(uart0);
    if(pkt & MOD_BIT){
      #ifdef KBDSIDE_RIGHT
      raised_mod_set(true);
      #else
      lowered_mod_set(true);
      #endif
    }
    else{
      #ifdef KBDSIDE_RIGHT
      raised_mod_set(false);
      #else
      lowered_mod_set(false);
      #endif
    }
    if(pkt & SHIFT_BIT){
      shift_set(true);
    }
    else{
      shift_set(false);
    }
    push_keypress((pkt & ROW_MASK) >> 4, pkt & COL_MASK);
  }
}

/*
 * \fn void xboard_comms_init(uint8_t is_main)
 * \brief initializes the board for communicating with the other keyboard
 */
void xboard_comms_init(){
  // UART Initialization
  uart_init(uart0, BAUD_RATE);
  gpio_set_function(XBOARDTX_PIN, GPIO_FUNC_UART);
  gpio_set_function(XBOARDRX_PIN, GPIO_FUNC_UART);
  uart_set_hw_flow(uart0, false, false);
  uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
  uart_set_fifo_enabled(uart0, false);

  // Interrupt Initialization
  irq_set_exclusive_handler(UART0_IRQ, rx_irq);
  irq_set_enabled(UART0_IRQ, true);
  uart_set_irq_enables(uart0, true, false);
}

/*
 * \fn void xboard_comms_send();
 * \brief Sends a key press packet to the other keyboard
 * \param row - row of the key pressed
 * \param row - column of the key pressed
 */
void xboard_comms_send(uint8_t row, uint8_t col){
  uint8_t pkt = 0;
  #ifdef KBDSIDE_RIGHT
  pkt |= lowered_mod_get() ? MOD_BIT : 0;
  #else
  pkt |= raised_mod_get() ? MOD_BIT : 0;
  #endif
  pkt |= shift_get() ? SHIFT_BIT : 0;
  pkt |= ((row << 4)) & ROW_MASK;
  pkt |= col & COL_MASK;
  uart_putc_raw(uart0, pkt);
}

#endif //_SRC_XBOARD_COMMS_C
