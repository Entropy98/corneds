/*
 * \file xboard_comms.c
 * \author Harper Weigle
 * \date Nov 25 2023
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
 * \fn uint8_t keynum_to_col()
 * \brief gets the column from the key number
 * \param uint8_t keynum - key number
 * \param uint8_t row - row of the key
 * \returns column of the key
 */
static uint8_t keynum_to_col(uint8_t keynum, uint8_t row) {
  uint8_t col = 0;
  if(row >= 3){
    col = (keynum % NUM_COLS) + 3;
  }
  else{
    col = keynum % NUM_COLS;
  }
  return col;
}

/*
 * \fn uint8_t keynum_to_row()
 * \brief gets the row from the key number
 * \param uint8_t keynum - key number
 * \returns row of the key
 */
static uint8_t keynum_to_row(uint8_t keynum) {
  return keynum / NUM_COLS;
}

/*
 * \fn void rx_irq()
 * \brief Pushes a key to the key buffer on UART interrupt. Inspired from pico sdk example
 */
static void rx_irq(){
  uint8_t col = 0;
  uint8_t key = 0;
  uint8_t pkt = 0;
  uint8_t row = 0;

  while(uart_is_readable(uart0)){
    pkt = uart_getc(uart0);

    if(pkt & XBOARD_PKT_ALTGUI_BIT){
      if(kbd_side_get() == KBDSIDE_RIGHT){
        gui_set(true);
      }
      else {
        alt_set(true);
      }
    }
    else{
      if(kbd_side_get() == KBDSIDE_RIGHT){
        gui_set(false);
      }
      else {
        alt_set(false);
      }
    }
    if(pkt & XBOARD_PKT_MOD_BIT){
      if(kbd_side_get() == KBDSIDE_RIGHT){
        lowered_mod_set(true);
      }
      else {
        raised_mod_set(true);
      }
    }
    else{
      if(kbd_side_get() == KBDSIDE_RIGHT){
        lowered_mod_set(false);
      }
      else {
        raised_mod_set(false);
      }
    }
    if(pkt & XBOARD_PKT_SHIFT_BIT){
      shift_set(true, kbd_side_get() == KBDSIDE_LEFT);
    }
    else{
      shift_set(false, kbd_side_get() == KBDSIDE_LEFT);
    }
    key = pkt & XBOARD_PKT_KEY_MASK;
    if(key != XBOARD_PKT_INVALID){
      row = keynum_to_row(key);
      col = keynum_to_col(key, row);
      push_keypress(col, row, kbd_side_get() == KBDSIDE_LEFT, true);
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
 * \param row - row of the key pressed or XBOARD_PKT_INVALID
 */
void xboard_comms_send(uint8_t col, uint8_t row){
  uint8_t key = 0;
  uint8_t pkt = 0;

  if(kbd_side_get() == KBDSIDE_RIGHT){
    pkt |= raised_mod_get() ? XBOARD_PKT_MOD_BIT : 0;
    pkt |= alt_get() ? XBOARD_PKT_ALTGUI_BIT : 0;
  }
  else {
    pkt |= lowered_mod_get() ? XBOARD_PKT_MOD_BIT : 0;
    pkt |= gui_get() ? XBOARD_PKT_ALTGUI_BIT : 0;
  }

  pkt |= shift_get() ? XBOARD_PKT_SHIFT_BIT : 0;

  if((col == XBOARD_PKT_INVALID) || (row == XBOARD_PKT_INVALID)){
    key |= XBOARD_PKT_INVALID;
  }
  else {
    key = (row * NUM_COLS);
    if(row == 3){
      key += col - 3;
    }
    else{
      key += col;
    }
    key &= XBOARD_PKT_KEY_MASK;
  }
  pkt |= key;

  uart_putc_raw(uart0, pkt);
}

#endif //_SRC_XBOARD_COMMS_C
