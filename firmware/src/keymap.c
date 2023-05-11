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
#define NUM_ROWS 3
#define KEY_BUFFER_SIZE 50

#define ROW0_MASK 0x04
#define ROW1_MASK 0x08
#define ROW2_MASK 0x10
#define ROW3_MASK 0x20

static uint8_t key_buffer[KEY_BUFFER_SIZE];
static uint8_t key_buffer_head = 0;
static uint8_t key_buffer_foot = 0;
static bool keys_initialized = false;
static uint8_t row_masks[NUM_ROWS] = {ROW0_MASK, ROW1_MASK, ROW2_MASK};
static uint8_t cols[NUM_COLS] = {COL0, COL1, COL2, COL3, COL4, COL5};

static keymap_t normal_map_r = {{ HID_KEY_Y,    HID_KEY_U,    HID_KEY_I,     HID_KEY_O,      HID_KEY_P,         HID_KEY_BACKSPACE},
                                { HID_KEY_H,    HID_KEY_J,    HID_KEY_K,     HID_KEY_L,      HID_KEY_SEMICOLON, HID_KEY_APOSTROPHE},
                                { HID_KEY_N,    HID_KEY_M,    HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH,     HID_KEY_SHIFT_RIGHT}};

static keymap_t raised_map_r = {{ MACRO_CARET,       MACRO_AMPERSAND,       MACRO_ASTRISK, MACRO_PARENTHESIS_LEFT, MACRO_PARENTHESIS_RIGHT, HID_KEY_DELETE},
                                { HID_KEY_6,         HID_KEY_7,             HID_KEY_8,     HID_KEY_9,              HID_KEY_0,               MACRO_COLON},
                                { MACRO_CURLY_RIGHT, HID_KEY_BRACKET_RIGHT, HID_KEY_EQUAL, HID_KEY_VOLUME_DOWN,    HID_KEY_VOLUME_UP,       HID_KEY_CONTROL_RIGHT}};

static keymap_t lowered_map_r = {{ HID_KEY_F6,         HID_KEY_F7,         HID_KEY_F8,       HID_KEY_F9,          HID_KEY_F10,  HID_KEY_INSERT},
                                 { HID_KEY_ARROW_LEFT, HID_KEY_ARROW_DOWN, HID_KEY_ARROW_UP, HID_KEY_ARROW_RIGHT, MACRO_PIPE,   HID_KEY_F11},
                                 { HID_KEY_MINUS,      MACRO_PLUS,         HID_KEY_NONE,     HID_KEY_NONE,        HID_KEY_NONE, HID_KEY_F12}};

static keymap_t normal_map_l = {{ HID_KEY_ESCAPE,       HID_KEY_Q,    HID_KEY_W,    HID_KEY_E,        HID_KEY_R, HID_KEY_T},
                                { HID_KEY_CONTROL_LEFT, HID_KEY_A,    HID_KEY_S,    HID_KEY_D,        HID_KEY_F, HID_KEY_G},
                                { HID_KEY_SHIFT_LEFT,   HID_KEY_Z,    HID_KEY_X,    HID_KEY_C,        HID_KEY_V, HID_KEY_B}};

static keymap_t raised_map_l = {{ HID_KEY_ESCAPE,     MACRO_EXCLAMATION_POINT, MACRO_AT,     MACRO_HASHTAG,    MACRO_DOLLAR_SIGN,    MACRO_PERCENT},
                                { HID_KEY_TAB,        HID_KEY_1,               HID_KEY_2,    HID_KEY_3,        HID_KEY_4,            HID_KEY_5},
                                { HID_KEY_SHIFT_LEFT, HID_KEY_BACKSLASH,       MACRO_TILDE,  HID_KEY_GRAVE,    HID_KEY_BRACKET_LEFT, MACRO_CURLY_LEFT}};

static keymap_t lowered_map_l = {{ HID_KEY_ESCAPE,       HID_KEY_F1,   HID_KEY_F2,   HID_KEY_F3,       HID_KEY_F4,         HID_KEY_F5},
                                 { HID_KEY_CONTROL_LEFT, HID_KEY_HOME, HID_KEY_END,  HID_KEY_NONE,     HID_KEY_NONE,       HID_KEY_PAGE_UP},
                                 { HID_KEY_SHIFT_LEFT,   HID_KEY_NONE, HID_KEY_NONE, MACRO_LESS_THAN,  MACRO_GREATER_THAN, HID_KEY_PAGE_DOWN}};


/*
 * \fn bool key_buffer_full()
 * \brief checks if the key buffer can be pushed to
 * \returns bool true if key buffer is full
 */
static bool key_buffer_full(){
  return (key_buffer_foot == (key_buffer_head - 1))
      || ((key_buffer_foot == (KEY_BUFFER_SIZE - 1)) && (key_buffer_head == 0));
}

/*
 * \fn bool key_buffer_empty()
 * \brief checks if the key buffer can be popped from
 * \returns bool true if key buffer is empty
 */
static bool key_buffer_empty(){
  return (key_buffer_foot == key_buffer_head);
}

/*
 * \fn uint8_t key_buffer_push(uint8_t key)
 * \brief local helper function for pushing to key buffer
 * \param uint8_t key - key to push to key buffer
 * \returns 0 if successful
 */
static uint8_t key_buffer_push(uint8_t key){
  uint8_t retval = 1;

  if(!key_buffer_full()) {
    key_buffer[key_buffer_foot] = key;
    key_buffer_foot++;
    if(key_buffer_foot == KEY_BUFFER_SIZE) key_buffer_foot = 0;
    retval = 0;
  }

  return retval;
}

/*
 * \fn uint8_t key_buffer_pop()
 * \brief local helper function for pushing to key buffer
 * \returns uint8_t keycode off the top of the buffer and HID_KEY_NONE otherwise
 */
static uint8_t key_buffer_pop(){
  uint8_t keycode = HID_KEY_NONE;

  if(!key_buffer_empty()) {
    keycode = key_buffer[key_buffer_head];
    key_buffer_head++;
    if(key_buffer_head == KEY_BUFFER_SIZE) key_buffer_head = 0;
  }

  return keycode;
}

/*
 * \fn void populate_key_rows(uint8_t col, uint32_t keys_pressed, uint8_t mod)
 * \brief local helper function for pushing all pressed rows given a column
 * \param uint8_t col - column asserted at time of function call
 * \param uint32_t keys_pressed - keys pressed at time of function call
 * \param uint8_t - 1 if mod if pressed
 */
static void populate_key_rows(uint8_t col, uint32_t keys_pressed, uint8_t mod){
  for(uint8_t row = 0; row < NUM_ROWS; row++){
    if(key_buffer_full()) break;
    if(mod){
      if(keys_pressed & row_masks[row]){
        #ifdef KBDSIDE_RIGHT
        key_buffer_push(lowered_map_r[row][NUM_COLS - 1 - col]);
        #else
        key_buffer_push(raised_map_l[row][NUM_COLS - 1 - col]);
        #endif //KBDSIDE
      }
    }
    else{
      if(keys_pressed & row_masks[row]){
        #ifdef KBDSIDE_RIGHT
        key_buffer_push(normal_map_r[row][NUM_COLS - 1 - col]);
        #else
        key_buffer_push(normal_map_l[row][NUM_COLS - 1 - col]);
        #endif //KBDSIDE
      }
    }
  }
}

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

  gpio_set_function(ROW0, GPIO_FUNC_SIO);
  gpio_set_function(ROW1, GPIO_FUNC_SIO);
  gpio_set_function(ROW2, GPIO_FUNC_SIO);
  gpio_set_function(ROW3, GPIO_FUNC_SIO);
  gpio_set_function(COL0, GPIO_FUNC_SIO);
  gpio_set_function(COL1, GPIO_FUNC_SIO);
  gpio_set_function(COL2, GPIO_FUNC_SIO);
  gpio_set_function(COL3, GPIO_FUNC_SIO);
  gpio_set_function(COL4, GPIO_FUNC_SIO);
  gpio_set_function(COL5, GPIO_FUNC_SIO);

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
 * \fn void poll_keypresses()
 * \brief populates key_buffer with pressed keys
 */
void poll_keypresses() {
  uint8_t mod = 0;
  uint8_t buf_full = 0;
  uint32_t keys_pressed = 0;

  buf_full = key_buffer_full();
  if(buf_full) return;

  gpio_put(COL4, 1);
  keys_pressed = gpio_get_all();
  mod = keys_pressed & ROW3_MASK ? 1 : 0;
  populate_key_rows(4, keys_pressed, mod);
  gpio_put(COL4, 0);

  buf_full = key_buffer_full();
  if(buf_full) return;

  gpio_put(COL5, 1);
  keys_pressed = gpio_get_all();
  #ifdef KBDSIDE_RIGHT
  if(keys_pressed & ROW3_MASK) key_buffer_push(HID_KEY_ENTER);
  #else
  if(keys_pressed & ROW3_MASK) key_buffer_push(HID_KEY_SPACE);
  #endif //KBDSIDE
  populate_key_rows(5, keys_pressed, mod);
  gpio_put(COL5, 0);

  buf_full = key_buffer_full();
  if(buf_full) return;

  gpio_put(COL3, 1);
  keys_pressed = gpio_get_all();
  #ifdef KBDSIDE_RIGHT
  if(keys_pressed & ROW3_MASK) key_buffer_push(HID_KEY_ALT_RIGHT);
  #else
  if(keys_pressed & ROW3_MASK) key_buffer_push(HID_KEY_GUI_RIGHT);
  #endif //KBDSIDE
  populate_key_rows(3, keys_pressed, mod);
  gpio_put(COL3, 0);

  buf_full = key_buffer_full();
  if(buf_full) return;

  for(uint8_t col=0; col < (NUM_COLS - 3); col++){
    gpio_put(cols[col], 1);
    keys_pressed = gpio_get_all();
    populate_key_rows(col, keys_pressed, mod);
    gpio_put(cols[col], 0);
  }
}

/*
 * \fn uint8_t get_keypress()
 * \brief returns the next keycode. Must call poll_keypresses() first
 * \returns uint8_t keycode of the next keypress
 */
uint8_t get_keypress() {
  return key_buffer_pop();
}

#endif //_SRC_KEYMAP_S
