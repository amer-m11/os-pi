#include "mm.h"
#include "common.h"

static uint16_t mem_map[PAGING_PAGES] = {
    0,
};

uint64_t allocate_page()
{
    for (uint64_t i = 0; i < PAGING_PAGES; i++)
    {
        if (mem_map[i] == 0)
        {
            mem_map[i] = 1; // allocate
            return LOW_MEMORY + (i * PAGE_SIZE);
        }
    }
    return 0; // No free pages available
}

void free_page(uint64_t page)
{
    if (page < LOW_MEMORY || page > HIGH_MEMORY)
    {
        return; // Invalid page address
    }

    uint64_t index = (page - LOW_MEMORY) / PAGE_SIZE;
    if (index < PAGING_PAGES)
    {
        mem_map[index] = 0; // free
    }
}