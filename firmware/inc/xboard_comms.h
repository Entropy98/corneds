/*
 * \file xboard_comms.h
 * \author Harper Weigle
 * \date Nov 25 2023
 * \brief interfacing between sides of the keyboard
 */

#ifndef _INC_XBOARD_COMMS_H
#define _INC_XBOARD_COMMS_H

#include "pico/stdlib.h"

/**
 * Cross Board Packet Structure
 * 0x0 Key Bit 0
 * 0x1 Key Bit 1
 * 0x2 Key But 2
 * 0x3 Key Bit 3
 * 0x4 Key Bit 0
 * 0x5 Key Bit 2
 * 0x6 Shift Bit
 * 0x7 Mod Bit
 */

#define XBOARD_PKT_ALTGUI_BIT 0x2
#define XBOARD_PKT_MOD_BIT    0x80
#define XBOARD_PKT_SHIFT_BIT  0x40
#define XBOARD_PKT_KEY_MASK   0x1F

#define XBOARD_PKT_INVALID 0x1F

void xboard_comms_init(bool is_main);
void xboard_comms_send(uint8_t col, uint8_t row);

#endif //_INC_XBOARD_COMMS_H

