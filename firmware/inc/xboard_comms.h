/*
 * \file xboard_comms.h
 * \author Harper Weigle
 * \date May 11 2023
 * \brief interfacing between sides of the keyboard
 */

#ifndef _INC_XBOARD_COMMS_H
#define _INC_XBOARD_COMMS_H

#include "pico/stdlib.h"

void xboard_comms_init();
void xboard_comms_send(uint8_t row, uint8_t col);

#endif //_INC_XBOARD_COMMS_H

