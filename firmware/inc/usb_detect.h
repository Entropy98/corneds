/*
 * \file usb_detect.h
 * \author Harper Weigle
 * \date Jan 18 2024
 * \brief functions to determine whether or not a USB is plugged in
 */

#ifndef _INC_USB_DETECT_H
#define _INC_USB_DETECT_H

#include "pico/stdlib.h"

#define USB_DISCONNECTED 0
#define USB_CONNECTED 0x4

typedef enum {
  USB_STATE_UNMOUNTED = 0,
  USB_STATE_MOUNTED,
  USB_STATE_SUSPENDED,
} usb_state_e;

usb_state_e usb_state_get();
bool usb_state_check();

#endif //_INC_USB_DETECT_H
