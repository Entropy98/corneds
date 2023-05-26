/*
 * \file keyamp.h
 * \author Harper Weigle
 * \date Apr 19 2023
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

#define ROW0  2
#define ROW1  3
#define ROW2  4
#define ROW3  5
#define COL0  6
#define COL1  7
#define COL2  8
#define COL3  9
#define COL4  10
#define COL5  11

typedef uint8_t keymap_t[3][6];

void init_keys(bool is_main);
void poll_keypresses();
void push_keypress(uint8_t row, uint8_t col);
uint8_t get_keypress();
bool lowered_mod_get();
void lowered_mod_set(bool pressed);
bool raised_mod_get();
void raised_mod_set(bool pressed);
bool shift_get();
void shift_set(bool pressed);

#endif //_INC_KEYMAP_H
