#pragma once

#include "common.h"

#include "peripherals/base.h"

// see Chapter 5. GPIO in https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf

struct GpioPinData {
    volatile uint32_t reserved;
    volatile uint32_t data[2];
};

struct GpioRegs {
    volatile uint32_t func_select[6];           // 0x00 - 0x18 Function Select 0-5
    struct GpioPinData set;                     // 0x18 - 0x24 Pin Output Set 0-1
    struct GpioPinData clear;                   // 0x24 - 0x30 Pin Output Clear 0-1
    struct GpioPinData level;                   // 0x30 - 0x3C Pin Level 0-1
    struct GpioPinData event_detect_status;     // 0x3C - 0x48 Pin Event Detect Status 0-1
    struct GpioPinData rising_enable;           // 0x48 - 0x54 Pin Rising Edge Detect Enable 0-1
    struct GpioPinData falling_enable;          // 0x54 - 0x60 Pin Falling Edge Detect Enable 0-1
    struct GpioPinData high_enable;             // 0x60 - 0x6C Pin High Detect Enable 0-1
    struct GpioPinData low_enable;              // 0x6C - 0x78 Pin Low Detect Enable 0-1
    struct GpioPinData async_rising_enable;     // 0x78 - 0x84 Pin Async. Rising Edge Detect 0
    struct GpioPinData async_falling_enable;    // 0x84 - 0x90 Pin Async. Falling Edge Detect 0
    volatile uint32_t reserved;                 // 0x90 - 0x94 TODO: compare to BCM2711
    volatile uint32_t pull_up_down_enable;      // 0x94 - 0x98 TODO: compare to BCM2711
    volatile uint32_t pull_up_down_clock[2];    // 0x98 - 0xA4 TODO: compare to BCM2711
};

#define REGS_GPIO ((struct GpioRegs *) (PBASE + 0x00200000))