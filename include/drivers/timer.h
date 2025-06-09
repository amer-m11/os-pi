#pragma once

#include "common.h"

#define TIMER_BASE_INTERVAL_CYCLES 10000000

/**
 * initializes the second of the four system timer interrupts (timer_1)
 * by setting its corrsponding compare register to TIMER_BASE_INTERVAL_CYCLES cycles.
 */
void timer1_init(void);

/**
 * Clears the timer_1 interrupt by writing 1 to the corresponding
 * bit in the control register
 */
void timer1_clear_interrupt(void);

/**
 * Sets the timer_1 compare register to trigger an interrupt after the specified number of cycles.
 */
void timer1_set_interval(uint32_t cycles);

/**
 * Note that the timer counter is a read-only register, so
 * to have periodic interrupts, we need to update the compare register
 * to the current timer value + TIMER_BASE_INTERVAL_CYCLES when handling the interrupt.
 */
void timer1_handle_interrupt(void);