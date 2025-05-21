#pragma once

#include "common.h"
#include "regs/peripherals/base.h"

struct system_timer_regs
{
    volatile uint32_t control;             // 0x00
    volatile uint32_t counter_lower_half;  // 0x04
    volatile uint32_t counter_higher_half; // 0x08
    volatile uint32_t compare0;            // 0x0C
    volatile uint32_t compare1;            // 0x10
    volatile uint32_t compare2;            // 0x14
    volatile uint32_t compare3;            // 0x18
};

enum timer_control_bits
{
    TIMER0_CONTROL_BIT = 1 << 0,
    TIMER1_CONTROL_BIT = 1 << 1,
    TIMER2_CONTROL_BIT = 1 << 2,
    TIMER3_CONTROL_BIT = 1 << 3,
};

#define REGS_TIMER ((struct system_timer_regs *)(PBASE + 0x00003000))