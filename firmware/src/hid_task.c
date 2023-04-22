/*
 * \file hid_task.c
 * \author Harper Weigle
 * \date Apr 18 2023
 * \brief task to take care of HID USB device needs
 */

#ifndef _SRC_HID_TASK_C
#define _SRC_HID_TASK_C

#include "tusb.h"

#include "usb_descriptors.h"
#include "hid_task.h"

/*
 * \fn void send_hid_report()
 * \brief sends report for keyboard HID profile. Should be called every 10ms
 * \param keycode_t* keycode to send
 */
void send_hid_report(uint8_t raw_code) {
  uint8_t keycode[6] = {0};
  keycode[0] = raw_code;
  if(tud_hid_ready()) {
    if(raw_code != HID_KEY_NONE){
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
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
