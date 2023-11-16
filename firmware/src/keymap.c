/*
 * \file keyamp.c
 * \author Harper Weigle
 * \date Nov 14 2023
 * \brief mapping of keys to functions
 */

#ifndef _SRC_KEYMAP_C
#define _SRC_KEYMAP_C

#include "tusb.h"
#include "pico/stdlib.h"

#include "keymap.h"
#include "pinmap.h"
#include "xboard_comms.h"

static bool main_kbd = true;

static uint8_t key_buffer[KEY_BUFFER_SIZE];
static uint8_t key_buffer_head = 0;
static uint8_t key_buffer_foot = 0;

static uint8_t row_masks[NUM_ROWS] = {ROW0_MASK, ROW1_MASK, ROW2_MASK};
static uint8_t cols[NUM_COLS] = {KEYCOL0_PIN, KEYCOL1_PIN, KEYCOL2_PIN, KEYCOL3_PIN, KEYCOL4_PIN, KEYCOL5_PIN};

static bool keys_initialized = false;

static bool raised = false;
static bool lowered = false;
static bool shifted = false;

static keymap_t normal_map_r = {{ HID_KEY_Y,    HID_KEY_U,    HID_KEY_I,     HID_KEY_O,      HID_KEY_P,         HID_KEY_BACKSPACE},
                                { HID_KEY_H,    HID_KEY_J,    HID_KEY_K,     HID_KEY_L,      HID_KEY_SEMICOLON, HID_KEY_APOSTROPHE},
                                { HID_KEY_N,    HID_KEY_M,    HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH,     HID_KEY_SHIFT_RIGHT}};

static keymap_t raised_map_r = {{ MACRO_CARET,       MACRO_AMPERSAND,       MACRO_ASTRISK, MACRO_PARENTHESIS_LEFT, MACRO_PARENTHESIS_RIGHT, HID_KEY_DELETE},
                                { HID_KEY_6,         HID_KEY_7,             HID_KEY_8,     HID_KEY_9,              HID_KEY_0,               MACRO_COLON},
                                { MACRO_CURLY_RIGHT, HID_KEY_BRACKET_RIGHT, HID_KEY_EQUAL, HID_KEY_VOLUME_DOWN,    HID_KEY_VOLUME_UP,       HID_KEY_CONTROL_RIGHT}};

static keymap_t lowered_map_r = {{ HID_KEY_F6,         HID_KEY_F7,         HID_KEY_F8,       HID_KEY_F9,          HID_KEY_F10,  HID_KEY_INSERT},
                                 { HID_KEY_ARROW_LEFT, HID_KEY_ARROW_DOWN, HID_KEY_ARROW_UP, HID_KEY_ARROW_RIGHT, MACRO_PIPE,   HID_KEY_F11},
                                 { HID_KEY_MINUS,      MACRO_PLUS,         HID_KEY_NONE,     HID_KEY_NONE,        HID_KEY_NONE, HID_KEY_F12}};

static keymap_t normal_map_l = {{ HID_KEY_T, HID_KEY_R, HID_KEY_E, HID_KEY_W, HID_KEY_Q, HID_KEY_ESCAPE},
                                { HID_KEY_G, HID_KEY_F, HID_KEY_D, HID_KEY_S, HID_KEY_A, HID_KEY_CONTROL_LEFT},
                                { HID_KEY_B, HID_KEY_V, HID_KEY_C, HID_KEY_X, HID_KEY_Z, HID_KEY_SHIFT_LEFT}};


static keymap_t raised_map_l = {{ MACRO_PERCENT,    MACRO_DOLLAR_SIGN,    MACRO_HASHTAG, MACRO_AT,    MACRO_EXCLAMATION_POINT, HID_KEY_ESCAPE},
                                { HID_KEY_5,        HID_KEY_4,            HID_KEY_3,     HID_KEY_2,   HID_KEY_1,               HID_KEY_TAB},
                                { MACRO_CURLY_LEFT, HID_KEY_BRACKET_LEFT, HID_KEY_GRAVE, MACRO_TILDE, HID_KEY_BACKSLASH,       HID_KEY_SHIFT_LEFT}};

static keymap_t lowered_map_l = {{ HID_KEY_F5,        HID_KEY_F4,         HID_KEY_F3,      HID_KEY_F2,   HID_KEY_F1,   HID_KEY_ESCAPE},
                                 { HID_KEY_PAGE_UP,   HID_KEY_NONE,       HID_KEY_NONE,    HID_KEY_END,  HID_KEY_HOME, HID_KEY_CONTROL_LEFT},
                                 { HID_KEY_PAGE_DOWN, MACRO_GREATER_THAN, MACRO_LESS_THAN, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_SHIFT_LEFT}};

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
 * \fn void populate_key_rows(uint8_t col, uint32_t keys_pressed)
 * \brief local helper function for pushing all pressed rows given a column or sending them to the main keyboard
 * \param uint8_t col - column asserted at time of function call
 * \param uint32_t keys_pressed - keys pressed at time of function call
 */
static void populate_key_rows(uint8_t col, uint32_t keys_pressed){
  for(uint8_t row = 0; row < NUM_ROWS; row++){
    if(key_buffer_full()) break;
    if(keys_pressed & row_masks[row]){
      if(main_kbd){
        if(raised){
          #ifdef KBDSIDE_RIGHT
          key_buffer_push(raised_map_r[row][NUM_COLS - 1 - col]);
          #else
          key_buffer_push(raised_map_l[row][NUM_COLS - 1 - col]);
          #endif //KBDSIDE
        }
        else if(lowered){
          #ifdef KBDSIDE_RIGHT
          key_buffer_push(lowered_map_r[row][NUM_COLS - 1 - col]);
          #else
          key_buffer_push(lowered_map_l[row][NUM_COLS - 1 - col]);
          #endif //KBDSIDE
        }
        else{
          #ifdef KBDSIDE_RIGHT
          key_buffer_push(normal_map_r[row][NUM_COLS - 1 - col]);
          #else
          key_buffer_push(normal_map_l[row][NUM_COLS - 1 - col]);
          #endif //KBDSIDE
        }
      }
      else{
        xboard_comms_send(row, col);
      }
    }
  }
}

/*
 * \fn void init_keys()
 * \brief initializes the appropraite GPIOs
 * \param bool is_main - true if this device is connected to USB
 */
void init_keys(bool is_main) {
  gpio_init(KEYROW0_PIN);
  gpio_init(KEYROW1_PIN);
  gpio_init(KEYROW2_PIN);
  gpio_init(KEYROW3_PIN);
  gpio_init(KEYCOL0_PIN);
  gpio_init(KEYCOL1_PIN);
  gpio_init(KEYCOL2_PIN);
  gpio_init(KEYCOL3_PIN);
  gpio_init(KEYCOL4_PIN);
  gpio_init(KEYCOL5_PIN);

  gpio_set_function(KEYROW0_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYROW1_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYROW2_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYROW3_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYCOL0_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYCOL1_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYCOL2_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYCOL3_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYCOL4_PIN, GPIO_FUNC_SIO);
  gpio_set_function(KEYCOL5_PIN, GPIO_FUNC_SIO);

  gpio_set_dir(KEYROW0_PIN, GPIO_IN);
  gpio_set_dir(KEYROW1_PIN, GPIO_IN);
  gpio_set_dir(KEYROW2_PIN, GPIO_IN);
  gpio_set_dir(KEYROW3_PIN, GPIO_IN);
  gpio_set_dir(KEYCOL0_PIN, GPIO_OUT);
  gpio_set_dir(KEYCOL1_PIN, GPIO_OUT);
  gpio_set_dir(KEYCOL2_PIN, GPIO_OUT);
  gpio_set_dir(KEYCOL3_PIN, GPIO_OUT);
  gpio_set_dir(KEYCOL4_PIN, GPIO_OUT);
  gpio_set_dir(KEYCOL5_PIN, GPIO_OUT);

  main_kbd = is_main;
}

/*
 * \fn void poll_keypresses(bool is_main)
 * \brief populates key_buffer with pressed keys
 */
void poll_keypresses() {
  uint32_t keys_pressed = 0;

  if(!key_buffer_full()){
    gpio_put(KEYCOL4_PIN, true);
    keys_pressed = gpio_get_all();

    #ifdef KBDSIDE_RIGHT
    if(keys_pressed & ROW3_MASK){
      lowered_mod_set(true);
    }
    else{
      lowered_mod_set(false);
    }
    #else
    if(keys_pressed & ROW3_MASK){
      raised_mod_set(true);
    }
    else{
      raised_mod_set(false);
    }
    #endif //KBDSIDE

    populate_key_rows(4, keys_pressed);
    gpio_put(KEYCOL4_PIN, false);
  }

  if(!key_buffer_full()){
    gpio_put(KEYCOL5_PIN, true);
    keys_pressed = gpio_get_all();
    #ifdef KBDSIDE_RIGHT
    if(keys_pressed & ROW3_MASK) key_buffer_push(HID_KEY_ENTER);
    #else
    if(keys_pressed & ROW3_MASK) key_buffer_push(HID_KEY_SPACE);
    #endif //KBDSIDE
    populate_key_rows(5, keys_pressed);
    gpio_put(KEYCOL5_PIN, false);
  }

  if(!key_buffer_full()){
    gpio_put(KEYCOL3_PIN, true);
    keys_pressed = gpio_get_all();
    #ifdef KBDSIDE_RIGHT
    if(keys_pressed & ROW3_MASK) key_buffer_push(HID_KEY_ALT_RIGHT);
    #else
    if(keys_pressed & ROW3_MASK) key_buffer_push(HID_KEY_GUI_RIGHT);
    #endif //KBDSIDE
    populate_key_rows(3, keys_pressed);
    gpio_put(KEYCOL3_PIN, false);
  }

  if(!key_buffer_full()){
    gpio_put(KEYCOL0_PIN, true);
    keys_pressed = gpio_get_all();
    if(keys_pressed & ROW2_MASK) {
      shift_set(true);
    }
    else {
      shift_set(false);
    }
    populate_key_rows(0, keys_pressed);
    gpio_put(KEYCOL0_PIN, false);
  }

  for(uint8_t col=1; col < (NUM_COLS - 3); col++){
    if(key_buffer_full()) break;

    gpio_put(cols[col], true);
    keys_pressed = gpio_get_all();
    populate_key_rows(col, keys_pressed);
    gpio_put(cols[col], false);
  }
}

/*
 * \fn void push_keypress(uint8_t col, uint8_t row)
 * \brief Push a specific key to the key buffer
 * \param uint8_t col - column of the key pressed
 * \param uint8_t row - row of the key pressed
 */
void push_keypress(uint8_t col, uint8_t row){
  if(!key_buffer_full()){
    if(row < 3){
      if(raised_mod_get()){
        #ifdef KBDSIDE_RIGHT
        key_buffer_push(raised_map_l[row][NUM_COLS - 1 - col]);
        #else
        key_buffer_push(raised_map_r[row][NUM_COLS - 1 - col]);
        #endif //KBDSIDE
      }
      else if(lowered_mod_get()){
        #ifdef KBDSIDE_RIGHT
        key_buffer_push(lowered_map_l[row][NUM_COLS - 1 - col]);
        #else
        key_buffer_push(lowered_map_r[row][NUM_COLS - 1 - col]);
        #endif //KBDSIDE
      }
      else{
        #ifdef KBDSIDE_RIGHT
        key_buffer_push(normal_map_l[row][NUM_COLS - 1 - col]);
        #else
        key_buffer_push(normal_map_r[row][NUM_COLS - 1 - col]);
        #endif //KBDSIDE
      }
    }
    else{ // mod row. The mod key is handled in the packet information
      if(col == 5){
        #ifdef KBDSIDE_RIGHT
        key_buffer_push(HID_KEY_ENTER);
        #else
        key_buffer_push(HID_KEY_SPACE);
        #endif //KBDSIDE
      }
      else if(col == 3){
        #ifdef KBDSIDE_RIGHT
        key_buffer_push(HID_KEY_ALT_RIGHT);
        #else
        key_buffer_push(HID_KEY_GUI_RIGHT);
        #endif //KBDSIDE
      }
    }
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

/*
 * \fn bool rasied_mod_get()
 * \brief getter for raised mod
 * \returns state of the raised modifier
 */
bool raised_mod_get(){
  return raised;
}

/*
 * \fn void rasied_mod_set(bool pressed)
 * \brief setter for raised mod
 */
void raised_mod_set(bool pressed){
  raised = pressed;
}

/*
 * \fn bool lowered_mod_get()
 * \brief getter for raised mod
 */
bool lowered_mod_get(){
  return lowered;
}

/*
 * \fn void lowered_mod_set(bool pressed)
 * \brief setter for raised mod
 */
void lowered_mod_set(bool pressed){
  lowered = pressed;
}

/*
 * \fn bool shift_get()
 * \brief getter for shift
 */
bool shift_get(){
  return shifted;
}

/*
 * \fn void shift_set(bool pressed)
 * \brief setter for shift
 */
void shift_set(bool pressed){
  shifted = pressed;
}

#endif //_SRC_KEYMAP_C
