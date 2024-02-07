/*
 * \file watchdog.c
 * \author Harper Weigle
 * \date Feb 7 2024
 * \brief Functions for handling the watchdog
 */

#include <hardware/watchdog.h>

#include "alarm_utils.h"
#include "watchdog.h"

#define WATCHDOG_PET_INTERVAL_US 500000U
#define WATCHDOG_TIMEOUT_MS      1000U

static bool initialized = false;

static void watchdog_pet() {
  alarm_clear(WATCHDOG_ALARM_NUM);
  watchdog_update();
  alarm_set(WATCHDOG_ALARM_NUM, WATCHDOG_PET_INTERVAL_US);
}

/*
 * \fn watchdog_init()
 * \brief initializes the watchdog functionality
 * \param debug_disable - true is the watchdog should be disabled when the
 *                        debugger is connected
 */
void watchdog_init(bool debug_disable) {
  if(!initialized) {
    watchdog_start_tick(8U);
    alarm_init(WATCHDOG_ALARM_NUM, WATCHDOG_ALARM_IRQ, watchdog_pet);
    alarm_set(WATCHDOG_ALARM_NUM, WATCHDOG_PET_INTERVAL_US);
    watchdog_enable(WATCHDOG_TIMEOUT_MS, debug_disable);
    initialized = true;
  }
}



