/*
 * \file led_sm.h
 * \author Harper Weigle
 * \date Jan 19 2024
 * \brief Functions for handling the led behavior
 */

#ifndef _INC_LED_SM_H
#define _INC_LED_SM_H

#include <pico/stdlib.h>

typedef enum {
  LED_STATE_NONE = 0,
} led_state_e;

void led_sm_init();

#endif //_INC_LED_SM_H
