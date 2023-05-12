/*
 * \file led_utils.c
 * \author Harper Weigle
 * \date May 11 2023
 * \brief LED utilities
 */

#ifndef _SRC_LED_UTILS_C
#define _SRC_LED_UTILS_C

#include "pico/stdlib.h"

#include "led_utils.h"

#define BOARD_LED 25

static uint8_t led_state;

/*
 * \fn void led_init()
 * \brief initializes the board LED
 */
void led_init(){
  gpio_init(BOARD_LED);
  gpio_set_dir(BOARD_LED, GPIO_OUT);
  gpio_put(BOARD_LED, 0);
  led_state = 0;
}

/*
 * \fn void led_toggle()
 * \brief toggles the board LED
 */
void led_toggle(){
  led_state = led_state ? 0 : 1;
  gpio_put(BOARD_LED, led_state);
}

/*
 * \fn void led_on()
 * \brief turns the board LED on
 */
void led_on(){
  led_state = 1;
  gpio_put(BOARD_LED, led_state);
}

/*
 * \fn void led_off()
 * \brief turns the board LED off
 */
void led_off(){
  led_state = 0;
  gpio_put(BOARD_LED, led_state);
}

#endif //_SRC_KEYMAP_C
