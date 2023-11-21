/*
 * \file xboard_comms.h
 * \author Harper Weigle
 * \date Nov 21 2023
 * \brief interfacing between sides of the keyboard
 */

#ifndef _INC_XBOARD_COMMS_H
#define _INC_XBOARD_COMMS_H

#include "pico/stdlib.h"

/**
 * Cross Board Packet Structure
 * 0x0 Column Bit 0
 * 0x1 Column Bit 1
 * 0x2 Column But 2
 * 0x3 Column Bit 3
 * 0x4 Row Bit 0
 * 0x5 Row Bit 2
 * 0x6 Shift Bit
 * 0x7 Mod Bit
 */

#define XBOARD_PKT_MOD_BIT   0x80
#define XBOARD_PKT_SHIFT_BIT 0x40
#define XBOARD_PKT_ROW_MASK  0x30
#define XBOARD_PKT_COL_MASK  0x0F

#define XBOARD_PKT_INVALID 0xF

void xboard_comms_init(bool is_main);
void xboard_comms_send(uint8_t col, uint8_t row, bool is_modded, bool is_shifted);

#endif //_INC_XBOARD_COMMS_H

