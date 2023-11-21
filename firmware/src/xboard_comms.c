/*
 * \file xboard_comms.c
 * \author Harper Weigle
 * \date Nov 21 2023
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

#define BAUD_RATE 115200

/*
 * \fn void rx_irq()
 * \brief Pushes a key to the key buffer on UART interrupt. Inspired from pico sdk example
 */
static void rx_irq(){
  while(uart_is_readable(uart0)){
    uint8_t pkt = uart_getc(uart0);
    if(pkt & XBOARD_PKT_MOD_BIT){
      #ifdef KBDSIDE_RIGHT
      lowered_mod_set(true);
      #else
      raised_mod_set(true);
      #endif
    }
    else{
      #ifdef KBDSIDE_RIGHT
      lowered_mod_set(false);
      #else
      raised_mod_set(false);
      #endif
    }
    if(pkt & XBOARD_PKT_SHIFT_BIT){
      #ifdef KBDSIDE_RIGHT
        shift_set(true, false);
      #else
        shift_set(true, true);
      #endif
    }
    else{
      #ifdef KBDSIDE_RIGHT
        shift_set(false, false);
      #else
        shift_set(false, true);
      #endif
    }
    if((pkt & XBOARD_PKT_COL_MASK) != XBOARD_PKT_INVALID){
      #ifdef KBDSIDE_RIGHT
        push_keypress(pkt & XBOARD_PKT_COL_MASK,(pkt & XBOARD_PKT_ROW_MASK) >> 4, false, true);
      #else
        push_keypress(pkt & XBOARD_PKT_COL_MASK,(pkt & XBOARD_PKT_ROW_MASK) >> 4, true, true);
      #endif
    }
  }
}

/*
 * \fn void xboard_comms_init(uint8_t is_main)
 * \brief initializes the board for communicating with the other keyboard
 * \param bool is_main - initializes interrupt if it is right side
 */
void xboard_comms_init(bool is_main){
  // UART Initialization
  uart_init(uart0, BAUD_RATE);
  gpio_set_function(XBOARDTX_PIN, GPIO_FUNC_UART);
  gpio_set_function(XBOARDRX_PIN, GPIO_FUNC_UART);
  uart_set_hw_flow(uart0, false, false);
  uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
  uart_set_fifo_enabled(uart0, false);

  // Interrupt Initialization
  if(is_main){
    irq_set_exclusive_handler(UART0_IRQ, rx_irq);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(uart0, true, false);
  }
}

/*
 * \fn void xboard_comms_send(uint8_t col, uint8_t row);
 * \brief Sends a key press packet to the other keyboard
 * \param col - column of the key pressed or XBOARD_PKT_INVALID
 * \param row - row of the key pressed
 * \param is_modded - if the mod key is pressed
 * \param is_shifted - if the shift key is pressed
 */
void xboard_comms_send(uint8_t col, uint8_t row, bool is_modded, bool is_shifted){
  uint8_t pkt = 0;
  pkt |= is_modded ? XBOARD_PKT_MOD_BIT : 0;
  pkt |= is_shifted ? XBOARD_PKT_SHIFT_BIT : 0;
  pkt |= ((row << 4)) & XBOARD_PKT_ROW_MASK;
  pkt |= col & XBOARD_PKT_COL_MASK;
  uart_putc_raw(uart0, pkt);
}

#endif //_SRC_XBOARD_COMMS_C
