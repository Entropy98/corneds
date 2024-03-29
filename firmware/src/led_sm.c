/*
 * \file led_sm.c
 * \author Harper Weigle
 * \date Feb 3 2024
 * \brief Functions for handling the led behavior
 */

#include <hardware/timer.h>
#include <hardware/irq.h>

#include "alarm_utils.h"
#include "led_sm.h"
#include "led_utils.h"

#define LED_SM_ALARM_NUM 1
#define LED_SM_ALARM_IRQ TIMER_IRQ_1

#define LED_LONG_PAUSE_US  1000000U
#define LED_SHORT_PAUSE_US 300000U

#define STARTUP_NUM_BLINKS 4U
#define MOUNT_NUM_BLINKS 10U
#define UNMOUNT_NUM_BLINKS 12U

static bool initialized = false;

static uint32_t startup_behavior[STARTUP_NUM_BLINKS] = {LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        };

static uint32_t mount_behavior[MOUNT_NUM_BLINKS] = {LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    LED_LONG_PAUSE_US,
                                                    };

static uint32_t unmount_behavior[UNMOUNT_NUM_BLINKS] = {LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_LONG_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_LONG_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                        LED_SHORT_PAUSE_US,
                                                       };
static uint8_t blink_count = 0;

static led_state_e led_sm_curr_state;
static led_state_e led_sm_next_state;

/*
 * \fn complete_state_transition()
 * \brief moves the state to the next and resets state variables
 */
static void complete_state_transition() {
  blink_count = 0;
  led_sm_curr_state = led_sm_next_state;
  led_sm_next_state = LED_STATE_IDLE;
}

/*
 * \fn iterate_sm()
 * \brief callback the change the led based on the state
 *        and queue up the alarm for the next transition
 */
static void iterate_sm() {
  alarm_clear(LED_SM_ALARM_NUM);
  switch(led_sm_curr_state) {
    case LED_STATE_MOUNT:
      if(blink_count < MOUNT_NUM_BLINKS) {
        blink_count++;
        led_toggle();
        alarm_set(LED_SM_ALARM_NUM, mount_behavior[blink_count]);
      }
      else {
        complete_state_transition();
        iterate_sm();
      }
      break;
    case LED_STATE_UNMOUNT:
      if(blink_count < UNMOUNT_NUM_BLINKS) {
        blink_count++;
        led_toggle();
        alarm_set(LED_SM_ALARM_NUM, unmount_behavior[blink_count]);
      }
      else {
        complete_state_transition();
        iterate_sm();
      }
      break;
    case LED_STATE_STARTUP:
      if(blink_count < STARTUP_NUM_BLINKS) {
        blink_count++;
        led_toggle();
        alarm_set(LED_SM_ALARM_NUM, startup_behavior[blink_count]);
      }
      else {
        complete_state_transition();
        iterate_sm();
      }
      break;
    default:
      complete_state_transition();
      led_off();
  }
}

/*
 * \fn led_sm_init()
 * \brief initializes the state machine handling LED behavior
 */
void led_sm_init() {
  if(!initialized) {
    led_init();
    led_sm_curr_state = LED_STATE_IDLE;
    led_sm_next_state = LED_STATE_IDLE;
    alarm_init(LED_SM_ALARM_NUM, LED_SM_ALARM_IRQ, iterate_sm);
    initialized = true;
  }
}

/*
 * \fn led_sm_set_transition()
 * \brief sets the next state for the LED state machine to transition to
 * \param led_state_e - state to transition to
 */
void led_sm_set_transition(led_state_e next_state) {
  if(led_sm_curr_state == LED_STATE_IDLE) {
    led_sm_curr_state = next_state;
    led_sm_next_state = LED_STATE_IDLE;
    led_on();
    blink_count = 0;
    switch(next_state) {
      case LED_STATE_MOUNT:
        alarm_set(LED_SM_ALARM_NUM, mount_behavior[0]);
        break;
      case LED_STATE_UNMOUNT:
        alarm_set(LED_SM_ALARM_NUM, unmount_behavior[0]);
        break;
      case LED_STATE_STARTUP:
        alarm_set(LED_SM_ALARM_NUM, startup_behavior[0]);
        break;
      default:
        led_sm_curr_state = LED_STATE_IDLE;
    }
  }
  else{
    led_sm_next_state = next_state;
  }
}
