/*
 * \file results.h
 * \author Harper Weigle
 * \date Jan 08 2024
 * \brief Result codes for the CorneDS 40% Keyboard
 */

#ifndef _INC_RESULTS_H
#define _INC_RESULTS_H

#include "pico/stdlib.h"

typedef uint8_t result_t;

#define RESULT_SUCCESS        0
#define RESULT_ALARM_FAILURE  1
#define RESULT_UNKNOWN        255

#endif //_INC_RESULTS_H
