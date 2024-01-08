/*
 * \file timing_arch.c
 * \author Harper Weigle
 * \date Jan 08 2024
 * \brief Functions for handling the timing of events
 */

#ifndef _INC_TIMING_ARCH_H
#define _INC_TIMING_ARCH_H

#include "bsp/board.h"
#include "pico/stdlib.h"

#include "results.h"

/*
 * \fn timing_arch_init()
 * \brief initializes the timing architecture
 */
void timing_arch_init() {
  alarm_pool_init_default();
}

/*
 * \fn timing_arch_task_add()
 * \brief Adds a task to the timing architecture
 * \param uint32_t* task - task to add
 * \param uint64_t exec_freq - how long to wait before executing the task again
 * \returns RESULT_SUCCESS on successful execution
 */
result_t timing_arch_task_add(uint32_t* task, uint64_t exec_freq) {
  result_t result = RESULT_UNKNOWN;
}

#endif //_INC_TIMING_ARCH_H
