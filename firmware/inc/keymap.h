/*
 * \file keymap.h
 * \author Harper Weigle
 * \date Nov 25 2023
 * \brief mapping of keys to functions
 */

#ifndef _INC_KEYMAP_H
#define _INC_KEYMAP_H

#include "tusb.h"

#define MACRO_AMPERSAND         0xFF
#define MACRO_CARET             0xFE
#define MACRO_ASTRISK           0xFD
#define MACRO_PARENTHESIS_LEFT  0xFC
#define MACRO_PARENTHESIS_RIGHT 0xFB
#define MACRO_COLON             0xFA
#define MACRO_CURLY_LEFT        0xF9
#define MACRO_CURLY_RIGHT       0xF8
#define MACRO_PIPE              0xF7
#define MACRO_PLUS              0xF6
#define MACRO_EXCLAMATION_POINT 0xF5
#define MACRO_AT                0xF4
#define MACRO_HASHTAG           0xF3
#define MACRO_DOLLAR_SIGN       0xF2
#define MACRO_PERCENT           0xF1
#define MACRO_TILDE             0xF0
#define MACRO_LESS_THAN         0xEF
#define MACRO_GREATER_THAN      0xEE

#define ROW0_MASK 0x04
#define ROW1_MASK 0x08
#define ROW2_MASK 0x10
#define ROW3_MASK 0x20

#define NUM_COLS 6
#define NUM_ROWS 4
#define KEY_BUFFER_SIZE 50

#define ALTGUI_ROW 3
#define ALTGUI_COL 3
#define SHIFT_ROW  2
#define SHIFT_COL  0
#define MOD_ROW    3
#define MOD_COL    4

#define KEY_COOLDOWN_MS 30U

#define KBDSIDE_RIGHT 1U
#define KBDSIDE_LEFT 2U

typedef uint8_t keymap_t[3][6];

bool alt_get();
void alt_set(bool pressed);
void decrement_key_cooldowns();
void init_keys(bool is_main);
uint8_t kbd_side_get();
void poll_keypresses();
void push_keypress(uint8_t col, uint8_t row, bool is_right_side, bool ignore_cooldown);
uint8_t get_keypress();
bool gui_get();
void gui_set(bool pressed);
bool lowered_mod_get();
void lowered_mod_set(bool pressed);
bool raised_mod_get();
void raised_mod_set(bool pressed);
bool shift_get();
void shift_set(bool pressed, bool right_side);

#endif //_INC_KEYMAP_H
