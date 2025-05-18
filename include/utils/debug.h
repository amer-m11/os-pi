#ifndef __ASSEMBLER__ // Exclude the following for assembly files

#include "common.h"

uint64_t get_x16(void);
void set_x16(uint64_t);

uint64_t *get_vector_base_register(void);
uint64_t get_daif(void);

#endif