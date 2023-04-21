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
#define MACRO_QUOTE             0xFE
#define MACRO_AMPERSAND         0xFD
#define MACRO_CARET             0xFC
#define MACRO_ASTRISK           0xFB
#define MACRO_PARENTHESIS_LEFT  0xFA
#define MACRO_PARENTHESIS_RIGHT 0xF9
#define MACRO_COLON             0xF8
#define MACRO_CURLY_LEFT        0xF7
#define MACRO_CURLY_RIGHT       0xF6
#define MACRO_PIPE              0xF5
#define MACRO_PLUS              0xF4
#define MACRO_EXCLAMATION_POINT 0xF3
#define MACRO_AT                0xF2
#define MACRO_HASHTAG           0xF1
#define MACRO_DOLLAR_SIGN       0xF0
#define MACRO_PERCENT           0xEF
#define MACRO_TILDE             0xEE
#define MACRO_LESS_THAN         0xED
#define MACRO_GREATER_THAN      0xEC

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

typedef uint8_t keycode_t[6];
typedef keycode_t keymap_t[6][4];

keymap_t normal_map_r = {{ HID_KEY_Y,    HID_KEY_U,    HID_KEY_I,     HID_KEY_O,      HID_KEY_P,         HID_KEY_BACKSPACE},
                         { HID_KEY_H,    HID_KEY_J,    HID_KEY_K,     HID_KEY_L,      HID_KEY_SEMICOLON, MACRO_QUOTE},
                         { HID_KEY_N,    HID_KEY_M,    HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH,     HID_KEY_SHIFT_RIGHT},
                         { HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE,  HID_KEY_ENTER,  MOD_KEY,           HID_KEY_ALT_RIGHT}};

keymap_t raised_map_r = {{ MACRO_CARET,       MACRO_AMPERSAND,       MACRO_ASTRISK, MACRO_PARENTHESIS_LEFT, MACRO_PARENTHESIS_RIGHT, HID_KEY_DELETE},
                         { HID_KEY_6,         HID_KEY_7,             HID_KEY_8,     HID_KEY_9,              HID_KEY_0,               MACRO_COLON},
                         { MACRO_CURLY_RIGHT, HID_KEY_BRACKET_RIGHT, HID_KEY_EQUAL, HID_KEY_VOLUME_DOWN,    HID_KEY_VOLUME_UP,       HID_KEY_CONTROL_RIGHT},
                         { HID_KEY_NONE,      HID_KEY_NONE,          HID_KEY_NONE,  HID_KEY_ENTER,          MOD_KEY,                 HID_KEY_ALT_RIGHT}};

keymap_t lowered_map_r = {{ HID_KEY_F6,         HID_KEY_F7,         HID_KEY_F8,       HID_KEY_F9,          HID_KEY_F10,  HID_KEY_INSERT},
                          { HID_KEY_ARROW_LEFT, HID_KEY_ARROW_DOWN, HID_KEY_ARROW_UP, HID_KEY_ARROW_RIGHT, MACRO_PIPE,   HID_KEY_F11},
                          { HID_KEY_MINUS,      MACRO_PLUS,         HID_KEY_NONE,     HID_KEY_NONE,        HID_KEY_NONE, HID_KEY_F12},
                          { HID_KEY_NONE,       HID_KEY_NONE,       HID_KEY_NONE,     HID_KEY_ENTER,       MOD_KEY,      HID_KEY_ALT_RIGHT}};

keymap_t normal_map_l = {{ HID_KEY_ESCAPE,       HID_KEY_Q,    HID_KEY_W,    HID_KEY_E,        HID_KEY_R, HID_KEY_T},
                         { HID_KEY_CONTROL_LEFT, HID_KEY_A,    HID_KEY_S,    HID_KEY_D,        HID_KEY_F, HID_KEY_G},
                         { HID_KEY_SHIFT_LEFT,   HID_KEY_Z,    HID_KEY_X,    HID_KEY_C,        HID_KEY_V, HID_KEY_B},
                         { HID_KEY_NONE,         HID_KEY_NONE, HID_KEY_NONE, HID_KEY_GUI_LEFT, MOD_KEY,   HID_KEY_SPACE}};

keymap_t raised_map_l = {{ HID_KEY_ESCAPE,     MACRO_EXCLAMATION_POINT, MACRO_AT,     MACRO_HASHTAG,    MACRO_DOLLAR_SIGN,    MACRO_PERCENT},
                         { HID_KEY_TAB,        HID_KEY_1,               HID_KEY_2,    HID_KEY_3,        HID_KEY_4,            HID_KEY_5},
                         { HID_KEY_SHIFT_LEFT, HID_KEY_BACKSLASH,       MACRO_TILDE,  HID_KEY_GRAVE,    HID_KEY_BRACKET_LEFT, MACRO_CURLY_LEFT},
                         { HID_KEY_NONE,       HID_KEY_NONE,            HID_KEY_NONE, HID_KEY_GUI_LEFT, MOD_KEY,           HID_KEY_SPACE}};

keymap_t lowered_map_l = {{ HID_KEY_ESCAPE,       HID_KEY_F1,   HID_KEY_F2,   HID_KEY_F3,       HID_KEY_F4,         HID_KEY_F5},
                          { HID_KEY_CONTROL_LEFT, HID_KEY_HOME, HID_KEY_END,  HID_KEY_NONE,     HID_KEY_NONE,       HID_KEY_PAGE_UP},
                          { HID_KEY_SHIFT_LEFT,   HID_KEY_NONE, HID_KEY_NONE, MACRO_LESS_THAN,  MACRO_GREATER_THAN, HID_KEY_PAGE_DOWN},
                          { HID_KEY_NONE,         HID_KEY_NONE, HID_KEY_NONE, HID_KEY_GUI_LEFT, MOD_KEY,            HID_KEY_SPACE}};

void init_keys();
keycode_t** get_keypress();

#endif //_INC_KEYMAP_H
