/*
 * \file xboard_comms.h
 * \author Harper Weigle
 * \date Jan 18 2024
 * \brief interfacing between sides of the keyboard
 */

#ifndef _INC_XBOARD_COMMS_H
#define _INC_XBOARD_COMMS_H

#include "pico/stdlib.h"

/**
 * Cross Board Packet Structure
 *   | Bits | Content |
 *   |------|---------|
 *   | 7-8  | Header  |
 *   | 0-6  |  Data   |
 */

#define XBOARD_PKT_KEY_HEADER 0x00
#define XBOARD_PKT_MOD_HEADER 0x40

#define XBOARD_PKT_ALTGUI_BIT 0x01
#define XBOARD_PKT_MOD_BIT    0x02
#define XBOARD_PKT_SHIFT_BIT  0x04
#define XBOARD_PKT_CTRL_BIT   0x08

#define XBOARD_PKT_DATA_MASK   0x3F
#define XBOARD_PKT_HEADER_MASK 0xC0

#define XBOARD_PKT_INVALID    0x3F

void xboard_comms_init();
void xboard_comms_send(uint8_t col, uint8_t row);

#endif //_INC_XBOARD_COMMS_H

