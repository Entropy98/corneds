/*
 * \file time.c
 * \author Harper Weigle
 * \date Dec 18 2023
 * \brief timing functions to support backbone of time slice architecture
 */

#ifndef _SRC_TIMESLICE_C
#define _SRC_TIMESLICE_C

#include "pico/stdlib.h"

#include "timeslice.h"

#define MS_SLICE      0x01
#define MS_10_SLICE   0x02
#define MS_100_SLICE  0x04
#define S_SLICE       0x08

#define MS_SLICE_MASK      0xFE
#define MS_10_SLICE_MASK   0xFD
#define MS_100_SLICE_MASK  0xFB
#define S_SLICE_MASK       0xF7

static uint8_t time_slices = 0;

/*
 * \fn void update_time_slices()
 * \brief updates the time slices. Meant to be called every tick
 */
void update_time_slices() {
  uint32_t time = time_us_32();

  if(time % 1000U == 0U){
    time_slices |= MS_SLICE;
  }

  if(time % 10000U == 0U){
    time_slices |= MS_10_SLICE;
  }

  if(time % 100000U == 0U){
    time_slices |= MS_100_SLICE;
  }

  if(time % 1000000U == 0U){
    time_slices |= S_SLICE;
  }
}

/*
 * \fn void clear_ms_slice()
 * \brief resets ms slice
 */
void clear_ms_slice() {
  time_slices &= MS_SLICE_MASK;
}

/*
 * \fn void clear_10ms_slice()
 * \brief resets 10ms slice
 */
void clear_10ms_slice() {
  time_slices &= MS_10_SLICE_MASK;
}

/*
 * \fn void clear_100ms_slice()
 * \brief resets 100ms slice
 */
void clear_100ms_slice() {
  time_slices &= MS_100_SLICE_MASK;
}

/*
 * \fn void clear_s_slice()
 * \brief resets s slice
 */
void clear_s_slice() {
  time_slices &= S_SLICE_MASK;
}

/*
 * \fn uint8_t get_ms_slice()
 * \brief checks if the ms slice is high
 *
 * \returns 0 if low
 */
uint8_t get_ms_slice() {
  return time_slices & MS_SLICE;
}

/*
 * \fn uint8_t get_10ms_slice()
 * \brief checks if the 10ms slice is high
 *
 * \returns 0 if low
 */
uint8_t get_10ms_slice() {
  return time_slices & MS_10_SLICE;
}

/*
 * \fn uint8_t get_100ms_slice()
 * \brief checks if the 100ms slice is high
 *
 * \returns 0 if low
 */
uint8_t get_100ms_slice() {
  return time_slices & MS_100_SLICE;
}

/*
 * \fn uint8_t get_s_slice()
 * \brief checks if the s slice is high
 *
 * \returns 0 if low
 */
uint8_t get_s_slice() {
  return time_slices & S_SLICE;
}

#endif //_SRC_TIMESLICE_C
