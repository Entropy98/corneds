/*
 * \file timing_arch.c
 * \author Harper Weigle
 * \date Jan 08 2024
 * \brief Functions for handling the timing of events
 */

#ifndef _INC_TIMING_ARCH_H
#define _INC_TIMING_ARCH_H

#include <bsp/board.h>
#include <pico/stdlib.h>
#include <pico/sem.h>

#include "results.h"

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
 * \fn alarm_callback_t alarm_5ms_cback()
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
 * \fn alarm_callback_t alarm_10ms_cback()
 * \brief Executes every 10ms and resets semaphore for that timing block
 * \param alarm_id_t id - id of the alarm
 * \param void* user_data - UNUSED
 * \returns true if repeating should continue
 */
static bool alarm_10ms_cback(alarm_id_t id, void* user_data) {
  sem_release(&ms10_fired);
}

/*
 * \fn alarm_callback_t alarm_100ms_cback()
 * \brief Executes every 100ms and resets semaphore for that timing block
 * \param alarm_id_t id - id of the alarm
 * \param void* user_data - UNUSED
 * \returns true if repeating should continue
 */
static bool alarm_100ms_cback(alarm_id_t id, void* user_data) {
  sem_release(&ms100_fired);
}

/*
 * \fn alarm_callback_t alarm_1s_cback()
 * \brief Executes every 1s and resets semaphore for that timing block
 * \param alarm_id_t id - id of the alarm
 * \param void* user_data - UNUSED
 * \returns RESULT_SUCCESS on successful execution
 * \returns true if repeating should continue
 */
static bool alarm_1s_cback(alarm_id_t id, void* user_data) {
  sem_release(&s1_fired);
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
  ms5_alarm_id = add_repeating_timer_ms(5U, alarm_5ms_cback, NULL, &timer);
  ms10_alarm_id = add_repeating_timer_ms(10U, ms10_fired, NULL, true);
  ms100_alarm_id = add_repeating_timer_ms(100U, ms100_fired, NULL, true);
  s1_alarm_id = add_repeating_timer_ms(1000U, s1_fired, NULL, true);
}

/*
 * \fn timing_arch_task_add()
 * \brief Adds a task to the timing architecture
 * \param void task - task to add
 * \param uint64_t exec_freq_ms - how long to wait before executing the task again
 * \returns RESULT_SUCCESS on successful execution
 */
result_t timing_arch_task_add(void task(), uint64_t exec_freq_ms) {
  result_t result = RESULT_UNKNOWN;
}

#endif //_INC_TIMING_ARCH_H
