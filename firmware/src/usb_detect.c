/*
 * \file usb_detect.c
 * \author Harper Weigle
 * \date Jan 18 2024
 * \brief functions to determine whether or not a USB is plugged in
 */
#include <pico/stdlib.h>
#include <tusb.h>
#include <hardware/regs/usb.h>
#include <hardware/structs/usb.h>


#include "usb_detect.h"

static usb_state_e usb_state = USB_STATE_UNMOUNTED;
static bool usb_state_changed = false;

/*
 * \fn usb_state_get()
 * \brief getter for usb state
 * \returns enum of usb state
 */
usb_state_e usb_state_get(){
  return usb_state;
}

/*
 * \fn usb_state_changed()
 * \brief returns true if a usb state change is queued and will reset the flag
 * \returns true if there is a change queued
 */
bool usb_state_check() {
  bool change_queued = usb_state_changed;
  usb_state_changed = false;
  return change_queued;
}

void tud_mount_cb() {
  usb_state = USB_STATE_MOUNTED;
  usb_state_changed = true;
}

void tud_umount_cb() {
  usb_state = USB_STATE_UNMOUNTED;
  usb_state_changed = true;
}

void tud_suspend_cb(bool remote_wakeup_en) {
  usb_state = USB_STATE_SUSPENDED;
  usb_state_changed = true;
}

void tud_resume_cb() {
  usb_state = USB_STATE_MOUNTED;
  usb_state_changed = true;
}
