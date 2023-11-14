/*
 * \file debug_uart.h
 * \author Harper Weigle
 * \date Nov 13 2023
 * \brief uart print functions
 */

#ifndef _INC_DEBUG_UART_H
#define _INC_DEBUG_UART_H

#include "pico/stdlib.h"

void debug_uart_init();
void debug_print(const char* s);

#endif //_INC_DEBUG_UART_H

