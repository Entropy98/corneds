/*
 * \file led_sm.c
 * \author Harper Weigle
 * \date Jan 19 2024
 * \brief Functions for handling the led behavior
 */

#include <bsp/board.h>
#include <hardware/timer.h>
#include <hardware/irq.h>

#define LED_SM_ALARM_NUM 1
#define LED_SM_ALARM_IRQ TIMER_IRQ_1

/*
 * \fn led_sm_init()
 * \brief initializes the state machine handling LED behavior
 */
void led_sm_init() {

}
