#pragma once

#include "common.h"
#include "regs/peripherals/base.h"

/**
 * see Chapter 6.5.3. ARMC in https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf.
 */
struct irq_regs
{
    volatile uint32_t irq0_pending_0;
    volatile uint32_t irq0_pending_1;
    volatile uint32_t irq0_pending_2;
    volatile uint32_t res0;
    volatile uint32_t irq0_enable_0;
    volatile uint32_t irq0_enable_1;
    volatile uint32_t irq0_enable_2;
    volatile uint32_t res1;
    volatile uint32_t irq0_disable_0;
    volatile uint32_t irq0_disable_1;
    volatile uint32_t irq0_disable_2;
};

/**
 * see chapter 6.2.4. VideoCore interrupts in
 * https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf.
 *
 * Each bit corresponds to a different interrupt source.
 */
enum vc_irqs
{
    AUX_IRQ = (1 << 29),
    TIMER_0_IRQ = (1 << 0),
    TIMER_1_IRQ = (1 << 1),
    TIMER_2_IRQ = (1 << 2),
    TIMER_3_IRQ = (1 << 3),
};

#define REGS_IRQ ((struct irq_regs *)(PBASE + 0x0000B200))