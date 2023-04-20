/*
 * \file keyamp.h
 * \author Harper Weigle
 * \date Apr 19 2023
 * \brief mapping of keys to functions
 */

#ifndef _SRC_KEYMAP_S
#define _SRC_KEYMAP_S

#include "tusb.h"
#include "pico/stdlib.h"

#include "keymap.h"

static bool keys_initialized = false;

/*
 * \fn void init_keys()
 * \brief initializes the appropraite GPIOs
 */
void init_keys() {
  gpio_init(ROW0);
  gpio_init(ROW1);
  gpio_init(ROW2);
  gpio_init(ROW3);
  gpio_init(COL0);
  gpio_init(COL1);
  gpio_init(COL2);
  gpio_init(COL3);
  gpio_init(COL4);
  gpio_init(COL5);

  gpio_set_dir(ROW0, GPIO_IN);
  gpio_set_dir(ROW1, GPIO_IN);
  gpio_set_dir(ROW2, GPIO_IN);
  gpio_set_dir(ROW3, GPIO_IN);
  gpio_set_dir(COL0, GPIO_OUT);
  gpio_set_dir(COL1, GPIO_OUT);
  gpio_set_dir(COL2, GPIO_OUT);
  gpio_set_dir(COL3, GPIO_OUT);
  gpio_set_dir(COL4, GPIO_OUT);
  gpio_set_dir(COL5, GPIO_OUT);
}

/*
 * \fn keycode_t* get_keypress()
 * \brief returns the keycode based on the key pressed
 * \returns keycode_t keycode of pressed key
 */
keycode_t* get_keypress() {

}

#endif //_SRC_KEYMAP_S
