#pragma once
#ifndef __ASSEMBLER__ // Exclude the following for assembly files

#include "common.h"

/**
 * x16 is used as use as a debugging register.
 * NOTE: x16 is usually used to hold the system call number
 */
uint64_t get_x16(void);
void set_x16(uint64_t);
uint64_t get_sp(void);

void alarm(void);

#endif