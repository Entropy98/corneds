/*
 * \file led_blinky.c
 * \author Harper Weigle
 * \date 15 Jan 2024
 * \brief Blinks the main LED on the board to verify timing intervals
 *        Blinks in each loop for 5 seconds each
 */

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "led_utils.h"
#include "timing_arch.h"

#define STATE_DURATION_S 5
#define NUM_LED_STATES 4

#define MS5_STATE 0
#define MS10_STATE 1
#define MS100_STATE 2
#define S1_STATE 3

static uint8_t led_interval_state = 0;
static uint8_t state_time_s = 0;

void iterate_led_state() {
  if(state_time_s >= STATE_DURATION_S) {
    state_time_s = 0U;
    led_interval_state = (led_interval_state + 1) % NUM_LED_STATES;
  }
  else {
    state_time_s++;
  }
}

int main(void) {
  board_init();

  led_init();
  timing_arch_init();

  led_on();

  while(true){
    if(ms5_loop_check()) {
      if(led_interval_state == MS5_STATE){
        led_toggle();
      }
    }
    if(ms10_loop_check()) {
      if(led_interval_state == MS10_STATE){
        led_toggle();
      }
    }
    if(ms100_loop_check()) {
      if(led_interval_state == MS100_STATE) {
        led_toggle();
      }
    }
    if(s1_loop_check()) {
      if(led_interval_state == S1_STATE) {
        led_toggle();
      }
      iterate_led_state();
    }
  }
}
