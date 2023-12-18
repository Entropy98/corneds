/*
 * \file hid_task.c
 * \author Harper Weigle
 * \date Dec 18 2023
 * \brief task to take care of HID USB device needs
 */

#ifndef _SRC_HID_TASK_C
#define _SRC_HID_TASK_C

#include "tusb.h"

#include "hid_task.h"
#include "keymap.h"
#include "usb_descriptors.h"

#define KEYCODE_LEN 6

/*
 * \fn void send_hid_report()
 * \brief sends report for keyboard HID profile. Should be called every 10ms
 */
void send_hid_report() {
  uint8_t keycode[KEYCODE_LEN] = {0};
  uint8_t modifier = 0;
  uint8_t raw_code = HID_KEY_NONE;
  uint8_t keycode_idx = 0;

  if(tud_hid_ready()) {
    if(shift_get() & MOD_MASK_LEFT){
        modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
    }
    if(shift_get() & MOD_MASK_RIGHT){
      modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT;
    }
    if(alt_get()){
      modifier |= KEYBOARD_MODIFIER_RIGHTALT;
    }
    if(gui_get()){
      modifier |= KEYBOARD_MODIFIER_LEFTGUI;
    }
    if(ctrl_get()){
      modifier |= KEYBOARD_MODIFIER_LEFTCTRL;
    }
    do {
      raw_code = get_keypress();
      switch (raw_code) {
        case MACRO_AMPERSAND:
          keycode[keycode_idx] = HID_KEY_7;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_CARET:
          keycode[keycode_idx] = HID_KEY_6;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_ASTRISK:
          keycode[keycode_idx] = HID_KEY_8;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_PARENTHESIS_LEFT:
          keycode[keycode_idx] = HID_KEY_9;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_PARENTHESIS_RIGHT:
          keycode[keycode_idx] = HID_KEY_0;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_COLON:
          keycode[keycode_idx] = HID_KEY_SEMICOLON;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_CURLY_LEFT:
          keycode[keycode_idx] = HID_KEY_BRACKET_LEFT;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_CURLY_RIGHT:
          keycode[keycode_idx] = HID_KEY_BRACKET_RIGHT;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_PIPE:
          keycode[keycode_idx] = HID_KEY_BACKSLASH;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_PLUS:
          keycode[keycode_idx] = HID_KEY_EQUAL;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_EXCLAMATION_POINT:
          keycode[keycode_idx] = HID_KEY_1;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_AT:
          keycode[keycode_idx] = HID_KEY_2;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_HASHTAG:
          keycode[keycode_idx] = HID_KEY_3;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_DOLLAR_SIGN:
          keycode[keycode_idx] = HID_KEY_4;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_PERCENT:
          keycode[keycode_idx] = HID_KEY_5;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_TILDE:
          keycode[keycode_idx] = HID_KEY_GRAVE;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_LESS_THAN:
          keycode[keycode_idx] = HID_KEY_COMMA;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        case MACRO_GREATER_THAN:
          keycode[keycode_idx] = HID_KEY_PERIOD;
          modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          break;
        default:
          keycode[keycode_idx] = raw_code;
      }
      keycode_idx++;
    } while((raw_code != HID_KEY_NONE) && (keycode_idx < KEYCODE_LEN));

    if(keycode[0] != HID_KEY_NONE){
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keycode);
    }
    else{
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
    }
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
#endif //_SRC_HID_TASK_C
