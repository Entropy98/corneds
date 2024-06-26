/*
 * \file test_print.c
 * \author Harper Weigle
 * \date Nov 13 2023
 * \brief Testing the scan codes sent over the tud_hid_report
 *        Sends the keys "Hello World\n" every second and toggles the main LED
 *        !!Discalimer!!
 *        This is easiest done with two computers so you can still send commands
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "led_utils.h"
#include "usb_descriptors.h"

#define NUM_KEYS 12U

uint8_t key_array[NUM_KEYS] = {HID_KEY_H, HID_KEY_E, HID_KEY_L, HID_KEY_L, HID_KEY_O, HID_KEY_SPACE,
                    HID_KEY_W, HID_KEY_O, HID_KEY_R, HID_KEY_L, HID_KEY_D, HID_KEY_ENTER};
uint8_t modifier_array[NUM_KEYS] = {KEYBOARD_MODIFIER_LEFTSHIFT, 0, 0, 0, 0, 0,
                              KEYBOARD_MODIFIER_LEFTSHIFT, 0, 0, 0, 0, 0};

int main(void) {
  led_init();
  board_init();
  tusb_init();

  uint8_t key = 0;
  uint8_t keycode[6] = {0};

  while(true){
    tud_task();


      if(tud_hid_ready()) {
        keycode[0] = key_array[key];
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier_array[key], keycode);
        key++;
        key = key % 12;
        led_toggle();
      }
  }
}
