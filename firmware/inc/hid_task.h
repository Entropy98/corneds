/*
 * \file hid_task.h
 * \author Harper Weigle
 * \date Apr 18 2023
 * \brief task to take care of HID USB device needs
 */

#ifndef _INC_HID_TASK_H
#define _INC_HID_TASK_H

#include "pico/stdlib.h"
#include "tusb.h"

void send_hid_report();
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);

#endif //_INC_HID_TASK_H
