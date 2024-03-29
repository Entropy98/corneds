/*
 * \file test_xboard_comms.c
 * \author Harper Weigle
 * \date Dec 13 2023
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
#include "led_utils.h"
#include "timing_arch.h"
#include "usb_descriptors.h"
#include "xboard_comms.h"

#define NUM_KEYS 4U

#define PKT_QUEUE_SIZE 255U

static uint8_t key_array[NUM_KEYS] = {HID_KEY_P, 0, 0, HID_KEY_ENTER};

static uint8_t pkt_queue[PKT_QUEUE_SIZE] = {0};
static uint8_t pkt_queue_head = 0;
static uint8_t pkt_queue_tail = 0;

static bool pkt_queue_empty(){
  return pkt_queue_head == pkt_queue_tail;
}

static bool pkt_queue_full(){
  bool full = false;
  if((pkt_queue_head == 0) && (pkt_queue_tail == (PKT_QUEUE_SIZE - 1))) {
    full = true;
  }
  if(pkt_queue_tail == (pkt_queue_head - 1)){
    full = true;
  }
  return full;
}

static bool pkt_queue_push(uint8_t pkt) {
  bool success = false;

  if(!pkt_queue_full()){
    pkt_queue[pkt_queue_tail] = pkt;
    pkt_queue_tail = (pkt_queue_tail + 1) % PKT_QUEUE_SIZE;
    success = true;
  }

  return success;
}

static uint8_t pkt_queue_pop(){
  uint8_t pkt = 255;

  if(!pkt_queue_empty()){
    pkt = pkt_queue[pkt_queue_head];
    pkt_queue_head = (pkt_queue_head + 1) % PKT_QUEUE_SIZE;
  }

  return pkt;
}

void test_rx_irq(){
  while(uart_is_readable(uart0)){
    pkt_queue_push(uart_getc(uart0));
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
  init_keys(true);

  // Initialize UART but don't initialize irq because unit test one will be used instead
  xboard_comms_init(false);

  timing_arch_init();

  irq_set_exclusive_handler(UART0_IRQ, test_rx_irq);
  irq_set_enabled(UART0_IRQ, true);
  uart_set_irq_enables(uart0, true, false);

  uint8_t key = 0;
  uint8_t keycode[6] = {0};
  uint8_t pkt = 0;
  bool msg_rdy = false;

  while(true){
    tud_task();

    if(ms_loop_check()) {
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
        if(!pkt_queue_empty()){
          pkt = pkt_queue_pop();
          key_array[1] = nibble2HIDKEY(pkt >> 4);
          key_array[2] = nibble2HIDKEY(pkt);
          msg_rdy = true;
        }
        if(tud_hid_ready()){
          tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        }
      }
    }
  }

  if(s1_loop_check()) {
    led_toggle();
  }

}

