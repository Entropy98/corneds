/*
 * \file timing_arch.h
 * \author Harper Weigle
 * \date Jan 10 2024
 * \brief Functions for handling the timing of events
 */

#ifndef _INC_TIMING_ARCH_H
#define _INC_TIMING_ARCH_H

#include <pico/stdlib.h>

bool ms_loop_check();
bool ms5_loop_check();
bool ms10_loop_check();
bool ms100_loop_check();
bool s1_loop_check();
void timing_arch_init();

#endif //_INC_TIMING_ARCH_H
