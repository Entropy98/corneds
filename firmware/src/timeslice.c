/*
 * \file time.c
 * \author Harper Weigle
 * \date Apr 15 2023
 * \brief timing functions to support backbone of time slice architecture
 */

#ifndef _SRC_TIMESLICE_C
#define _SRC_TIMESLICE_C

#include "pico/stdlib.h"

#include "timeslice.h"

// rp2040 defaults to a 6MHz ring oscillator clock
// Values found through experimentation
#define TICKS_PER_MS      837
#define TICKS_PER_10MS    8560
#define TICKS_PER_100MS   85608
#define TICKS_PER_S       822435

#define MS_SLICE      0x01
#define MS_10_SLICE   0x02
#define MS_100_SLICE  0x04
#define S_SLICE       0x08

#define MS_SLICE_MASK      0xFE
#define MS_10_SLICE_MASK   0xFD
#define MS_100_SLICE_MASK  0xFB
#define S_SLICE_MASK       0xF7
typedef struct slice_timers {
  uint32_t  s_timer;
  uint32_t  ms_100_timer;
  uint16_t  ms_10_timer;
  uint16_t  ms_timer;
}slice_timers_t;

static slice_timers_t timers;
static uint8_t time_slices = 0;
static uint8_t led_toggle = 0;
/*
 * \fn void update_time_slices()
 * \brief updates the time slices. Meant to be called every tick
 */
void update_time_slices() {
  if(timers.ms_timer >= TICKS_PER_MS) {
    time_slices |= MS_SLICE;
    timers.ms_timer = 0;
  }
  else {
    timers.ms_timer++;
  }
  if(timers.ms_10_timer >= TICKS_PER_10MS) {
    gpio_put(25, led_toggle);
    gpio_put(4, led_toggle);
    led_toggle = led_toggle ? 0 : 1;
    time_slices |= MS_10_SLICE;
    timers.ms_10_timer = 0;
  }
  else {
    timers.ms_10_timer++;
  }
  if(timers.ms_100_timer >= TICKS_PER_100MS) {
    time_slices |= MS_100_SLICE;
    timers.ms_100_timer = 0;
  }
  else {
    timers.ms_100_timer++;
  }
  if(timers.s_timer >= TICKS_PER_S) {
    time_slices |= S_SLICE;
    timers.s_timer = 0;
  }
  else {
    timers.s_timer++;
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
  time_slices &= MS_SLICE_MASK;
}

/*
 * \fn uint8_t get_ms_slice()
 * \brief checks if the ms slice is high
 *
 * \returns 0 if low
 */
uint8_t get_ms_slice() {
  return time_slices & MS_SLICE_MASK;
}

/*
 * \fn uint8_t get_10ms_slice()
 * \brief checks if the 10ms slice is high
 *
 * \returns 0 if low
 */
uint8_t get_10ms_slice() {
  return time_slices & MS_10_SLICE_MASK;
}

/*
 * \fn uint8_t get_100ms_slice()
 * \brief checks if the 100ms slice is high
 *
 * \returns 0 if low
 */
uint8_t get_100ms_slice() {
  return time_slices & MS_100_SLICE_MASK;
}

/*
 * \fn uint8_t get_s_slice()
 * \brief checks if the s slice is high
 *
 * \returns 0 if low
 */
uint8_t get_s_slice() {
  return time_slices & S_SLICE_MASK;
}

#endif //_SRC_TIMESLICE_C
