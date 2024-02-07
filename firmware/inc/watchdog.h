/*
 * \file watchdog.h
 * \author Harper Weigle
 * \date Feb 7 2024
 * \brief Functions for handling the watchdog
 */

#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#include <pico/stdlib.h>

#define WATCHDOG_ALARM_NUM 2
#define WATCHDOG_ALARM_IRQ TIMER_IRQ_2

void watchdog_init(bool debug_disable);

#endif // _WATCHDOG_H
