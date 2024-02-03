/*
 * \file alarm_utils.c
 * \author Harper Weigle
 * \date Jan 21 2024
 * \brief Functions for handling alarms
 */
#include <hardware/timer.h>

#include "alarm_utils.h"

/*
 * \fn void alarm_clear()
 * \brief clears ah active alarm so it can be reset
 * \param uint32_t alarm - which alarm to arm
 */
void alarm_clear(uint32_t alarm) {
  hw_clear_bits(&timer_hw->intr, 1U << alarm);
}

/*
 * \fn void alarm_init()
 * \brief initializes an alarm
 * \param uint32_t alarm - alarm to initialize
 * \param uint32_t irq_num - interrupt to raise
 * \param irq_handler_t irq - handler to consume the interrupt
 */
void alarm_init(uint32_t alarm, uint32_t irq_num, irq_handler_t irq) {
  hw_set_bits(&timer_hw->inte, 1u << alarm);
  irq_set_exclusive_handler(irq_num, irq);
  irq_set_enabled(irq_num, true);
}

/*
 * \fn void alarm_set()
 * \brief sets the 5ms alarm that ticks up for the timing archs timers
 * \param uint32_t alarm - which alarm to arm
 * \param uint32_t delay - how long to arm the alarm for
 */
void alarm_set(uint32_t alarm, uint32_t delay) {
  hw_set_bits(&timer_hw->inte, 1U << alarm);
  timer_hw->alarm[alarm] = timer_hw->timerawl + delay;
}

