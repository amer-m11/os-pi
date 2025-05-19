#pragma once

#include "common.h"

/**
 * enables IRQ by clearing the corresponding bit in the DAIF register.
 * see section C5.2.3 page C5-815 in the ARM Architecture Reference Manual version L.a
 */
void enable_irq(void);
void disable_irq(void);

/**
 * initial value 0x1c0 = 0001 1100 000
 */
uint64_t get_daif_register(void);