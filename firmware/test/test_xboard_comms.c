/*
 * \file test_xboard_comms.c
 * \author Harper Weigle
 * \date Nov 21 2023
 * \brief Testing if keypresses are transmitted properly to the other board
 *        Sends the keys "[cX, rY, sA, mB]" where X is the column of the key pressed
 *        Y is the row of the key pressed, A = T if shift is pressed but F otherwise.
 *        B = T if mod is pressed but F otherwise.
 *
 *        This test should be flashed on the main board. The other board should have
 *        main firmware
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "keymap.h"
#include "pinmap.h"
#include "timeslice.h"
#include "led_utils.h"
#include "usb_descriptors.h"
#include "xboard_comms.h"

#define NUM_KEYS  4U
#define COL_IDX   2U
#define ROW_IDX   6U
#define SHIFT_IDX 10U
#define MOD_IDX   14U

static uint8_t key_array[NUM_KEYS] = {HID_KEY_P, 0, 0, HID_KEY_ENTER};

static volatile bool pkt_recevied = false;
static volatile uint8_t pkt = 0;

void test_rx_irq(){
  while(uart_is_readable(uart0)){
    pkt = uart_getc(uart0);
    pkt_recevied = true;
  }
}

uint8_t nibble2HIDKEY(uint8_t byte){
  uint8_t nibble = byte & 0xF;
  uint8_t key_val = HID_KEY_NONE;
  switch (nibble){
    case 0x0: key_val = HID_KEY_0; break;
    case 0x1: key_val = HID_KEY_1; break;
    case 0x2: key_val = HID_KEY_2; break;
    case 0x3: key_val = HID_KEY_3; break;
    case 0x4: key_val = HID_KEY_4; break;
    case 0x5: key_val = HID_KEY_5; break;
    case 0x6: key_val = HID_KEY_6; break;
    case 0x7: key_val = HID_KEY_7; break;
    case 0x8: key_val = HID_KEY_8; break;
    case 0x9: key_val = HID_KEY_9; break;
    case 0xA: key_val = HID_KEY_A; break;
    case 0xB: key_val = HID_KEY_B; break;
    case 0xC: key_val = HID_KEY_C; break;
    case 0xD: key_val = HID_KEY_D; break;
    case 0xE: key_val = HID_KEY_E; break;
    case 0xF: key_val = HID_KEY_F; break;
    default: key_val = HID_KEY_X;
  }
  return key_val;
}

int main(void) {
  led_init();
  board_init();
  tusb_init();
  init_time_slices();
  init_keys(true);

  // Initialize UART but don't initialize irq because unit test one will be used instead
  xboard_comms_init(false);

  irq_set_exclusive_handler(UART0_IRQ, test_rx_irq);
  irq_set_enabled(UART0_IRQ, true);
  uart_set_irq_enables(uart0, true, false);

  uint8_t key = 0;
  uint8_t keycode[6] = {0};
  bool msg_rdy = false;

  while(true){
    update_time_slices();
    tud_task();

    if(get_ms_slice() != 0) {
      if(msg_rdy){
        if(tud_hid_ready()){
          keycode[0] = key_array[key];
          tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
          key++;
          if(key == (NUM_KEYS)){
            msg_rdy = false;
            key = 0;
          }
        }
      }
      else{
        if(pkt_recevied){
          if((pkt & XBOARD_PKT_COL_MASK) != XBOARD_PKT_INVALID){
            key_array[1] = nibble2HIDKEY(pkt >> 4);
            key_array[2] = nibble2HIDKEY(pkt);
            msg_rdy = true;
          }
          pkt_recevied = false;
        }
        if(tud_hid_ready()){
          tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        }
      }
      clear_ms_slice();
    }

    if(get_10ms_slice() != 0) {
      clear_10ms_slice();
    }

    if(get_100ms_slice() != 0) {
      clear_100ms_slice();
    }

    if(get_s_slice() != 0) {
      led_toggle();
      clear_s_slice();
    }
  }
}

