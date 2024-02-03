/*
 * \file watchdog.c
 * \author Harper Weigle
 * \date Feb 3 2024
 * \brief Runs the startup LED behavior every 5 seconds when it fails
 *        to pet the watchdog
 */

#include <pico/stdlib.h>
#include <hardware/watchdog.h>

#include "led_sm.h"
#include "timing_arch.h"

#define WATCHDOG_TIMEOUT_MS 2000U
#define NUM_WATCHDOG_PETS 5U

int main(void) {
  led_sm_init();
  timing_arch_init();

  watchdog_start_tick(8U);
  watchdog_enable(WATCHDOG_TIMEOUT_MS, true);

  led_sm_set_transition(LED_STATE_STARTUP);

  uint8_t pet_count = 0;

  while(true){
    if(ms_loop_check()) {
    }
    if(ms5_loop_check()) {
    }
    if(ms10_loop_check()) {
    }
    if(ms100_loop_check()) {
    }
    if(s1_loop_check()) {
      if(pet_count < NUM_WATCHDOG_PETS) {
        watchdog_update();
        pet_count++;
      }
    }
  }
}

