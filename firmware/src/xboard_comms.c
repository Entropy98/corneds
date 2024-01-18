/*
 * \file xboard_comms.c
 * \author Harper Weigle
 * \date Jan 18 2024
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

#define XBOARD_UART_ID uart0
#define XBOARD_UART_IRQ UART0_IRQ
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
 *        This only happens if the keyboard is the main keybaord
 */
static void rx_irq(){
  uint8_t col = 0;
  uint8_t header = 0;
  uint8_t key = 0;
  uint8_t pkt = 0;
  uint8_t row = 0;

  while(uart_is_readable(XBOARD_UART_ID)){
    pkt = uart_getc(XBOARD_UART_ID);
    header = pkt & XBOARD_PKT_HEADER_MASK;

    if(keymap_main_kbd_get()) {
      switch (header) {
        case XBOARD_PKT_KEY_HEADER:
          key = (pkt & XBOARD_PKT_DATA_MASK);
          if(key != XBOARD_PKT_INVALID){
            row = keynum_to_row(key);
            col = keynum_to_col(key, row);
            push_keypress(col, row, kbd_side_get() == KBDSIDE_LEFT);
          }
          break;

        case XBOARD_PKT_MOD_HEADER:
          if(pkt & XBOARD_PKT_ALTGUI_BIT){
            if(kbd_side_get() == KBDSIDE_RIGHT){
              alt_set(true);
            }
            else {
              gui_set(true);
            }
          }
          else{
            if(kbd_side_get() == KBDSIDE_RIGHT){
              alt_set(false);
            }
            else {
              gui_set(false);
            }
          }

          if(pkt & XBOARD_PKT_MOD_BIT){
            if(kbd_side_get() == KBDSIDE_RIGHT){
              raised_mod_set(true);
            }
            else {
              lowered_mod_set(true);
            }
          }
          else{
            if(kbd_side_get() == KBDSIDE_RIGHT){
              raised_mod_set(false);
            }
            else {
              lowered_mod_set(false);
            }
          }

          if(pkt & XBOARD_PKT_SHIFT_BIT){
            shift_set(true, kbd_side_get() == KBDSIDE_LEFT);
          }
          else{
            shift_set(false, kbd_side_get() == KBDSIDE_LEFT);
          }

          if(pkt & XBOARD_PKT_CTRL_BIT){
            ctrl_set(true);
          }
          else{
            ctrl_set(false);
          }

          break;
      }
    }
  }
}

/*
 * \fn void xboard_comms_init(uint8_t is_main)
 * \brief initializes the board for communicating with the other keyboard
 */
void xboard_comms_init(){
  // UART Initialization
  uart_init(XBOARD_UART_ID, BAUD_RATE);
  gpio_set_function(XBOARDTX_PIN, GPIO_FUNC_UART);
  gpio_set_function(XBOARDRX_PIN, GPIO_FUNC_UART);
  uart_set_hw_flow(XBOARD_UART_ID, false, false);
  uart_set_format(XBOARD_UART_ID, 8, 1, UART_PARITY_NONE);
  uart_set_fifo_enabled(XBOARD_UART_ID, false);

  irq_set_exclusive_handler(XBOARD_UART_IRQ, rx_irq);
  irq_set_enabled(XBOARD_UART_IRQ, true);
  uart_set_irq_enables(uart0, true, false);
}

/*
 * \fn void xboard_comms_send(uint8_t col, uint8_t row);
 * \brief Sends a key press packet to the other keyboard
 * \param col - column of the key pressed or XBOARD_PKT_INVALID
 * \param row - row of the key pressed or XBOARD_PKT_INVALID
 */
void xboard_comms_send(uint8_t col, uint8_t row){
  uint8_t key_pkt = 0;
  uint8_t mod_pkt = 0;

  if(kbd_side_get() == KBDSIDE_RIGHT){
    mod_pkt |= lowered_mod_get() ? XBOARD_PKT_MOD_BIT : 0;
    mod_pkt |= alt_get() ? XBOARD_PKT_ALTGUI_BIT : 0;
  }
  else {
    mod_pkt |= raised_mod_get() ? XBOARD_PKT_MOD_BIT : 0;
    mod_pkt |= gui_get() ? XBOARD_PKT_ALTGUI_BIT : 0;
    mod_pkt |= ctrl_get() ? XBOARD_PKT_CTRL_BIT : 0;
  }
  mod_pkt |= shift_get() ? XBOARD_PKT_SHIFT_BIT : 0;
  mod_pkt &= XBOARD_PKT_DATA_MASK;
  mod_pkt |= XBOARD_PKT_MOD_HEADER;

  if((col == XBOARD_PKT_INVALID) || (row == XBOARD_PKT_INVALID)){
    key_pkt |= XBOARD_PKT_INVALID;
  }
  else {
    key_pkt = (row * NUM_COLS);
    if(row == 3){
      key_pkt += col - 3;
    }
    else{
      key_pkt += col;
    }
  }
  key_pkt &= XBOARD_PKT_DATA_MASK;
  key_pkt |= XBOARD_PKT_KEY_HEADER;

  uart_putc_raw(uart0, mod_pkt);
  uart_putc_raw(uart0, key_pkt);
  unset_change_queued();
}

#endif //_SRC_XBOARD_COMMS_C
