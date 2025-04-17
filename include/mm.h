#pragma once

#define PAGE_SHIFT	            12
#define TABLE_SHIFT 			9
#define SECTION_SHIFT			(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE   			(1 << PAGE_SHIFT)	
#define SECTION_SIZE			(1 << SECTION_SHIFT)	

#define LOW_MEMORY              (2 * SECTION_SIZE)

#ifndef __ASSEMBLER__

/**
 * Zeroes out a block of memory.
 * @param src The starting address of the memory block to zero out.
 * @param n The length of the memory block to zero out (in bytes).
 */
void memzero(unsigned long src, unsigned long n);

#endif