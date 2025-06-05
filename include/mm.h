#pragma once
#include "regs/peripherals/base.h"

#define PAGE_SHIFT 18
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (2 * SECTION_SIZE)
#define HIGH_MEMORY PBASE

#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES (PAGING_MEMORY / PAGE_SIZE)

#ifndef __ASSEMBLER__
#include "common.h"

/**
 * Zeroes out a block of memory.
 * @param src The starting address of the memory block to zero out.
 * @param n The length of the memory block to zero out (in bytes).
 */
void memzero(uint64_t src, uint64_t n);

/** primitive page allocation. TODO: update when MMU is enabled */
uint64_t allocate_page(void);
/** primitive page allocation. TODO: update when MMU is enabled */
void free_page(uint64_t page);

#endif