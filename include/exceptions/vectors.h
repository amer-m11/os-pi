#pragma once

#include "common.h"

/**
 * sets the vector base address register (VBAR_EL1)
 * see section D24.2.200 page D24-8484 in the ARM Architecture Reference Manual version L.a
 */
void init_exception_vectors_el1(void);

uint64_t *get_vectors_adr(void);
uint64_t *get_vector_base_register(void);