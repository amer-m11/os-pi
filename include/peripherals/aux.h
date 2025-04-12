#pragma once

#include "common.h"

#include "peripherals/base.h"

// see Chapter 2. Auxiliaries in https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf
struct AuxRegs {
    volatile uint32_t irq;          // 0x00 Auxiliary Interrupt status
    volatile uint32_t enables;      // 0x04 Auxiliary enables
    volatile uint32_t reserved[14]; // 0x08 - 0x40
    volatile uint32_t mu_io;        // 0x40 Mini UART I/O Data
    volatile uint32_t mu_ier;       // 0x44 Mini UART Interrupt Enable
    volatile uint32_t mu_iir;       // 0x48 Mini UART Interrupt Identify
    volatile uint32_t mu_lcr;       // 0x4C Mini UART Line Control
    volatile uint32_t mu_mcr;       // 0x50 Mini UART Modem Control
    volatile uint32_t mu_lsr;       // 0x54 Mini UART Line Status
    volatile uint32_t mu_msr;       // 0x58 Mini UART Modem Status
    volatile uint32_t mu_scratch;   // 0x5C Mini UART Scratch
    volatile uint32_t mu_cntl;      // 0x60 Mini UART Extra Control
    volatile uint32_t mu_stat;      // 0x64 Mini UART Extra Status
    volatile uint32_t mu_baud;      // 0x68 Mini UART Baudrate
    // ... SPI is currently not implemented
};

#define REGS_AUX ((struct AuxRegs *) (PBASE + 0x00215000))