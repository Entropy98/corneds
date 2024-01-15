/*
 * \file timing_arch.c
 * \author Harper Weigle
 * \date Jan 10 2024
 * \brief Functions for handling the timing of events
 */

#ifndef _INC_TIMING_ARCH_H
#define _INC_TIMING_ARCH_H

#include <bsp/board.h>
#include <pico/sem.h>
#include <hardware/timer.h>
#include <hardware/irq.h>

#include "results.h"
#include "timing_arch.h"
#include "led_utils.h"

#define ALARM0_OFFSET 0x10
#define ALARM1_OFFSET 0x14
#define ALARM2_OFFSET 0x18
#define ALARM3_OFFSET 0x1C

// Semaphores
static bool ms5_fired;
static bool ms10_fired;
static bool ms100_fired;
static bool s1_fired;

/*
 * \fn bool alarm_5ms_cback()
 * \brief Executes every 5ms and resets semaphore for that timing block
 */
static void alarm_5ms_cback() {
  ms5_fired = true;
}

/*
 * \fn bool alarm_10ms_cback()
 * \brief Executes every 10ms and resets semaphore for that timing block
 */
static void alarm_10ms_cback() {
  ms10_fired = true;
}

/*
 * \fn bool alarm_100ms_cback()
 * \brief Executes every 100ms and resets semaphore for that timing block
 */
static bool alarm_100ms_cback() {
  ms100_fired = true;
}

/*
 * \fn bool alarm_1s_cback()
 * \brief Executes every 1s and resets semaphore for that timing block
 */
static bool alarm_1s_cback() {
  s1_fired = true;
}

/*
 * \fn timing_arch_init()
 * \brief initializes the timing architecture
 */
void timing_arch_init() {
  alarm_pool_init_default();

  ms5_fired = false;
  ms10_fired = false;
  ms100_fired = false;
  s1_fired = false;

  hw_set_bits(&timer_hw->inte,
              ( (1U << ALARM0_OFFSET)
              | (1U << ALARM1_OFFSET)
              | (1U << ALARM2_OFFSET)
              | (1U << ALARM3_OFFSET)
              );
}

/*
 * \fn bool ms5_loop_check()
 * \brief checks if the 5ms loop can execute
 * \returns true if loop should execute
 */
bool ms5_loop_check() {
  return sem_try_acquire(&ms5_fired);
}

/*
 * \fn bool ms10_loop_check()
 * \brief checks if the 10ms loop can execute
 * \returns true if loop should execute
 */
bool ms10_loop_check() {
  return sem_try_acquire(&ms10_fired);
}

/*
 * \fn bool ms100_loop_check()
 * \brief checks if the 100ms loop can execute
 * \returns true if loop should execute
 */
bool ms100_loop_check() {
  return sem_try_acquire(&ms100_fired);
}

/*
 * \fn bool s1_loop_check()
 * \brief checks if the 1s loop can execute
 * \returns true if loop should execute
 */
bool s1_loop_check() {
  return sem_try_acquire(&s1_fired);
}

#endif //_INC_TIMING_ARCH_H
