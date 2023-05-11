/*
 * \file usb_detect.c
 * \author Harper Weigle
 * \date May 10 2023
 * \brief functions to determine whether or not a USB is plugged in
 */

#ifndef _SRC_TIMESLICE_C
#define _SRC_TIMESLICE_C

#include "hardware/regs/usb.h"
#include "hardware/structs/usb.h"
#include "pico/stdlib.h"

/*
 * \fn usb_detected()
 * \brief determines if a usb is detected
 * \returns 0 if not detected
 */
uint8_t usb_detected(){
  return (usb_hw->sie_status) & USB_SIE_STATUS_VBUS_DETECTED_BITS;
}

#endif //_SRC_TIMESLICE_C
