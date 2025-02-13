/*
 * \file test_keypress.c
 * \author Harper Weigle
 * \date Dec 06 2023
 * \brief Testing if keypresses are read properly
 *        Sends the keys "[cX, rY]" where X is the column of the key pressed
 *        and Y is the row of the key pressed
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "keymap.h"
#include "pinmap.h"
#include "led_utils.h"
#include "timing_arch.h"
#include "usb_descriptors.h"

#define NUM_KEYS 8U
#define COL_IDX 2U
#define ROW_IDX 6U

// Can treat all rows as the same because we're just looking at coordinates. Not mapping to keys
static uint8_t row_masks[NUM_ROWS] = {ROW0_MASK, ROW1_MASK, ROW2_MASK, ROW3_MASK};
static uint8_t cols[NUM_COLS] = {KEYCOL0_PIN, KEYCOL1_PIN, KEYCOL2_PIN, KEYCOL3_PIN, KEYCOL4_PIN, KEYCOL5_PIN};

static uint8_t num_keys[NUM_COLS] = {HID_KEY_0, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5};
static uint8_t key_array[NUM_KEYS] = {HID_KEY_BRACKET_LEFT, HID_KEY_C, 0, HID_KEY_COMMA, HID_KEY_SPACE, HID_KEY_R, 0, HID_KEY_BRACKET_RIGHT};

int main(void) {
  led_init();
  board_init();
  tusb_init();
  init_keys(true);
  timing_arch_init();

  uint8_t key = 0;
  uint8_t keycode[6] = {0};
  uint32_t keys_pressed = 0;
  bool key_is_pressed = false;
  uint8_t col = 0;

  while(true){
    tud_task();
    if(ms_loop_check()) {

      if(key_is_pressed){
        if(tud_hid_ready()){
          keycode[0] = key_array[key];
          tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
          key++;
          if(key == (NUM_KEYS)){
            key_is_pressed = false;
            key = 0;
          }
        }
      }
      else{
        gpio_put(cols[col], true);
        keys_pressed = gpio_get_all();
        for(uint8_t row=0; row<(NUM_ROWS); row++){
          if(keys_pressed & row_masks[row]){
            key_array[COL_IDX] = num_keys[col];
            key_array[ROW_IDX] = num_keys[row];
            key_is_pressed = true;
          }
        }
        gpio_put(cols[col], false);
        col++;
        col %= NUM_COLS;
        if(tud_hid_ready()){
          tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        }
      }
    }
    if(s1_loop_check()) {
      led_toggle();
    }
  }
}
