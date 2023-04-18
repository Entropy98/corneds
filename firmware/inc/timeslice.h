/*
 * \file time.h
 * \author Harper Weigle
 * \date Apr 15 2023
 * \brief timing functions to support backbone of time slice architecture
 */

#ifndef _SRC_TIMESLICE_H
#define _SRC_TIMESLICE_H

#include "pico/stdlib.h"

void update_time_slices();

void clear_ms_slice();
void clear_10ms_slice();
void clear_100ms_slice();
void clear_s_slice();

uint8_t get_ms_slice();
uint8_t get_10ms_slice();
uint8_t get_100ms_slice();
uint8_t get_s_slice();

#endif //_SRC_TIMESLICE_H
