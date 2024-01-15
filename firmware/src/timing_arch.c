/*
 * \file timing_arch.c
 * \author Harper Weigle
 * \date Jan 10 2024
 * \brief Functions for handling the timing of events
 */

#ifndef _INC_TIMING_ARCH_H
#define _INC_TIMING_ARCH_H

#include <bsp/board.h>
#include <hardware/timer.h>
#include <hardware/irq.h>

#include "results.h"
#include "timing_arch.h"
#include "led_utils.h"

#define ALARM_NUM 0
#define ALARM_IRQ TIMER_IRQ_0

static volatile bool ms5_fired;
static volatile bool ms10_fired;
static volatile bool ms100_fired;
static volatile bool s1_fired;

static volatile uint8_t ms5_counter;
static volatile uint8_t ms10_counter;
static volatile uint8_t ms100_counter;

/*
 * \fn void set_alarm
 * \brief sets the 5ms alarm that ticks up for the timing archs timers
 */
static void set_alarm() {
  hw_set_bits(&timer_hw->inte, 1U << ALARM_NUM);
  timer_hw->alarm[ALARM_NUM] = timer_hw->timerawl + 5000U;
}

/*
 * \fn bool alarm_1s_cback()
 * \brief Executes every 1s and increments timers accordingly
 */
static bool alarm_1s_cback() {
  s1_fired = true;
}

/*
 * \fn bool alarm_100ms_cback()
 * \brief Executes every 100ms and increments timers accordingly
 */
static bool alarm_100ms_cback() {
  ms100_fired = true;
  if(ms100_counter == 9U) {
    alarm_1s_cback();
    ms100_counter = 0U;
  }
  else {
    ms100_counter++;
  }
}


/*
 * \fn bool alarm_10ms_cback()
 * \brief Executes every 10ms and increments timers accordingly
 */
static void alarm_10ms_cback() {
  ms10_fired = true;
  if(ms10_counter == 9U) {
    alarm_100ms_cback();
    ms10_counter = 0U;
  }
  else {
    ms10_counter++;
  }
}

/*
 * \fn bool alarm_5ms_cback()
 * \brief Executes every 5ms and increments timers accordingly
 */
static void alarm_5ms_cback() {
  hw_clear_bits(&timer_hw->intr, 1U << ALARM_NUM);
  ms5_fired = true;
  if(ms5_fired == 1U) {
    alarm_10ms_cback();
    ms5_counter = 0U;
  }
  else {
    ms5_counter = 1U;
  }
  set_alarm();
}

/*
 * \fn timing_arch_init()
 * \brief initializes the timing architecture
 */
void timing_arch_init() {
  ms5_fired = false;
  ms10_fired = false;
  ms100_fired = false;
  s1_fired = false;

  ms5_counter = 0U;
  ms10_counter = 0U;
  ms100_counter = 0U;

  hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM);
  irq_set_exclusive_handler(ALARM_IRQ, alarm_5ms_cback);
  irq_set_enabled(ALARM_IRQ, true);
  set_alarm();
}

/*
 * \fn bool ms5_loop_check()
 * \brief checks if the 5ms loop can execute
 * \returns true if loop should execute
 */
bool ms5_loop_check() {
  bool ready = ms5_fired;
  ms5_fired = false;
  return ready;
}

/*
 * \fn bool ms10_loop_check()
 * \brief checks if the 10ms loop can execute
 * \returns true if loop should execute
 */
bool ms10_loop_check() {
  bool ready = ms10_fired;
  ms10_fired = false;
  return ready;
}

/*
 * \fn bool ms100_loop_check()
 * \brief checks if the 100ms loop can execute
 * \returns true if loop should execute
 */
bool ms100_loop_check() {
  bool ready = ms100_fired;
  ms100_fired = false;
  return ready;
}

/*
 * \fn bool s1_loop_check()
 * \brief checks if the 1s loop can execute
 * \returns true if loop should execute
 */
bool s1_loop_check() {
  bool ready = s1_fired;
  s1_fired = false;
  return ready;
}

#endif //_INC_TIMING_ARCH_H