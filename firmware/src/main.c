/*
 * \file main.c
 * \author Harper Weigle
 * \date Jan 18 2024
 * \brief Firmware for corneDS split 40% keyboard
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "debug_uart.h"
#include "hid_task.h"
#include "keymap.h"
#include "led_sm.h"
#include "led_utils.h"
#include "timing_arch.h"
#include "usb_descriptors.h"
#include "usb_detect.h"
#include "xboard_comms.h"

int main(void) {
  board_init();

  led_init();
  tusb_init();
  debug_uart_init();
  timing_arch_init();
  led_sm_init();

  xboard_comms_init();
  init_keys();

  led_sm_set_transition(LED_STATE_STARTUP);

  while(1) {
    tud_task();

    if(usb_state_check()) {
      switch (usb_state_get()) {
        case USB_STATE_MOUNTED:
          keymap_main_kbd_set(true);
          break;
        case USB_STATE_UNMOUNTED:
        case USB_STATE_SUSPENDED:
        default:
          keymap_main_kbd_set(false);
      }
    }

    if(ms_loop_check()) {
      poll_keypresses();
      if(usb_state_get() == USB_STATE_MOUNTED) {
        send_hid_report();
      }
    }
    if(ms5_loop_check()) {
    }

    if(ms10_loop_check()) {
    }

    if(ms100_loop_check()) {
    }

    if(s1_loop_check()) {
      if(usb_state_get() != USB_STATE_MOUNTED) {
        led_toggle();
      }
    }
  }

  return 0;
}
