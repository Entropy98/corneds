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

#define NUM_COLS 6
#define NUM_ROWS 4
#define NUM_KEYS 21

static keycode_t* key_buffer[NUM_KEYS];
static uint8_t key_buffer_head;
static uint8_t key_buffer_foot;
static bool keys_initialized = false;
static uint8_t rows[NUM_ROWS] = {ROW0, ROW1, ROW2, ROW3};
static uint8_t cols[NUM_COLS] = {COL0, COL1, COL2, COL3, COL4, COL5};

/*
 * \fn bool key_buffer_full()
 * \brief checks if the key buffer can be pushed to
 * \returns bool true if key buffer is full
 */
static bool key_buffer_full(){
  return (key_buffer_foot == key_buffer_head - 1)
      || ((key_buffer_foot == (NUM_KEYS - 1)) && (key_buffer_head == 0));
}

/*
 * \fn uint8_t key_buffer_push(keycode_t* key)
 * \brief local helper function for pushing to key buffer
 * \param keycode_t* key - key to push to key buffer
 * \returns 0 if successful
 */
static uint8_t key_buffer_push(keycode_t* key){
  uint8_t retval = 1;

  if(!key_buffer_full()) {
    key_buffer[key_buffer_head] = key;
    key_buffer_head++;
    retval = 0;
  }

  return retval;
}


/*
 * \fn void init_keys()
 * \brief initializes the appropraite GPIOs
 */
void init_keys() {
  key_buffer_head = 0;
  key_buffer_foot = 0;

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
 * \fn keycode_t** get_keypress()
 * \brief returns the keycode based on the key pressed
 * \returns keycode_t** list of pressed keys
 */
keycode_t** get_keypress() {
  for(uint8_t col=0; col < NUM_COLS; col++){
    gpio_put(cols[col], 1);
    for(uint8_t row=0; row < NUM_ROWS; row++){

    }
  }
}

#endif //_SRC_KEYMAP_S
