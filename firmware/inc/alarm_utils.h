/*
 * \file alarm_utils.h
 * \author Harper Weigle
 * \date Jan 21 2024
 * \brief Functions for handling alarms
 */

#ifndef _INC_ALARM_UTILS_H
#define _INC_ALARM_UTILS_H

#include <pico/stdlib.h>

void alarm_clear(uint32_t alarm);
void alarm_init(uint32_t alarm, uint32_t irq_num, irq_handler_t irq);
void alarm_set(uint32_t alarm, uint32_t delay);

#endif //_INC_ALARM_UTILS_H
