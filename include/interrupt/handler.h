#pragma once

#include "common.h"

void enable_interrupt_controller(void);

void show_invalid_entry_message(uint32_t type, uint64_t esr, uint64_t address);
void handle_irq(void);

/**
 * sets the vector base address register (VBAR_EL1)
 * see section D24.2.200 page D24-8484 in the ARM Architecture Reference Manual version L.a
 */
void init_exception_vectors_el1(void);

uint64_t *get_vectors_adr(void);
uint64_t *get_vector_base_register(void);