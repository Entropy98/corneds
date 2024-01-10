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

#include "results.h"
#include "timing_arch.h"

// Semaphores
static semaphore_t ms5_fired;
static semaphore_t ms10_fired;
static semaphore_t ms100_fired;
static semaphore_t s1_fired;

volatile static alarm_id_t ms5_alarm_id;
volatile static alarm_id_t ms10_alarm_id;
volatile static alarm_id_t ms100_alarm_id;
volatile static alarm_id_t s1_alarm_id;

/*
 * \fn bool alarm_5ms_cback()
 * \brief Executes every 5ms and resets semaphore for that timing block
 * \param alarm_id_t id - id of the alarm
 * \param void* user_data - UNUSED
 * \returns true if repeating should continue
 */
static bool alarm_5ms_cback(struct repeating_timer *t) {
  sem_release(&ms5_fired);
  return true;
}

/*
 * \fn bool alarm_10ms_cback()
 * \brief Executes every 10ms and resets semaphore for that timing block
 * \param alarm_id_t id - id of the alarm
 * \param void* user_data - UNUSED
 * \returns true if repeating should continue
 */
static bool alarm_10ms_cback(struct repeating_timer *t) {
  sem_release(&ms10_fired);
  return true;
}

/*
 * \fn bool alarm_100ms_cback()
 * \brief Executes every 100ms and resets semaphore for that timing block
 * \param alarm_id_t id - id of the alarm
 * \param void* user_data - UNUSED
 * \returns true if repeating should continue
 */
static bool alarm_100ms_cback(struct repeating_timer *t) {
  sem_release(&ms100_fired);
  return true;
}

/*
 * \fn bool alarm_1s_cback()
 * \brief Executes every 1s and resets semaphore for that timing block
 * \param alarm_id_t id - id of the alarm
 * \param void* user_data - UNUSED
 * \returns RESULT_SUCCESS on successful execution
 * \returns true if repeating should continue
 */
static bool alarm_1s_cback(struct repeating_timer *t) {
  sem_release(&s1_fired);
  return true;
}

/*
 * \fn timing_arch_init()
 * \brief initializes the timing architecture
 */
void timing_arch_init() {
  alarm_pool_init_default();

  sem_init(&ms5_alarm_id, 0, 1);
  sem_init(&ms10_alarm_id, 0, 1);
  sem_init(&ms100_alarm_id, 0, 1);
  sem_init(&s1_alarm_id, 0, 1);

  struct repeating_timer timer;
  s1_alarm_id = add_repeating_timer_ms(1000U, alarm_1s_cback, NULL, &timer);
  sleep_ms(1U);
  ms100_alarm_id = add_repeating_timer_ms(100U, alarm_100ms_cback, NULL, &timer);
  sleep_ms(1U);
  ms10_alarm_id = add_repeating_timer_ms(10U, alarm_10ms_cback, NULL, &timer);
  sleep_ms(1U);
  ms5_alarm_id = add_repeating_timer_ms(5U, alarm_5ms_cback, NULL, &timer);
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
