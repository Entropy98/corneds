/*
 * \file keyamp.h
 * \author Harper Weigle
 * \date Apr 19 2023
 * \brief mapping of keys to functions
 */

#ifndef _INC_KEYMAP_H
#define _INC_KEYMAP_H

#include "tusb.h"

#define MOD_KEY                 0xFF
#define MACRO_AMPERSAND         0xFE
#define MACRO_CARET             0xFD
#define MACRO_ASTRISK           0xFC
#define MACRO_PARENTHESIS_LEFT  0xFB
#define MACRO_PARENTHESIS_RIGHT 0xFA
#define MACRO_COLON             0xF9
#define MACRO_CURLY_LEFT        0xF8
#define MACRO_CURLY_RIGHT       0xF7
#define MACRO_PIPE              0xF6
#define MACRO_PLUS              0xF5
#define MACRO_EXCLAMATION_POINT 0xF4
#define MACRO_AT                0xF3
#define MACRO_HASHTAG           0xF2
#define MACRO_DOLLAR_SIGN       0xF1
#define MACRO_PERCENT           0xF0
#define MACRO_TILDE             0xEF
#define MACRO_LESS_THAN         0xEE
#define MACRO_GREATER_THAN      0xED

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

void init_keys();
void poll_keypresses();
uint8_t get_keypress();

#endif //_INC_KEYMAP_H
