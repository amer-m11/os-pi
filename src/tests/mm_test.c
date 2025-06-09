#include "mm.h"
#include "tests/tests.h"
#include "utils/debug.h"
#include "utils/printf.h"

void test_memory_allocation(void)
{
    printf("\r\n\r\n");
    printf("=== TEST: Primitive Memory Allocation ===\n\r");
    printf("Low memory address: 0x%x\r\n", LOW_MEMORY);
    printf("High memory address: 0x%x\r\n", HIGH_MEMORY);
    uint64_t current_sp = get_sp();

    uint64_t page1 = allocate_page();
    printf("Allocated first page at address 0x%x (expected: 0x%x) - %s\r\n", page1, LOW_MEMORY,
           (page1 == LOW_MEMORY) ? PASS_STR : FAIL_STR);

    uint64_t page2 = allocate_page();
    printf("Allocated second page at address: 0x%x (expected: 0x%x) - %s\r\n", page2,
           LOW_MEMORY + PAGE_SIZE, (page2 == LOW_MEMORY + PAGE_SIZE) ? PASS_STR : FAIL_STR);

    uint64_t page3 = allocate_page();
    printf("Allocated third page at address: 0x%x (expected: 0x%x) - %s\r\n", page3,
           LOW_MEMORY + 2 * PAGE_SIZE, (page3 == LOW_MEMORY + 2 * PAGE_SIZE) ? PASS_STR : FAIL_STR);

    free_page(page2);
    printf("Freed second page at address: 0x%x\r\n", page2);

    uint64_t page4 = allocate_page();
    printf("Allocated new page at address: 0x%x (expected: 0x%x) - %s\r\n", page4, page2,
           (page4 == page2) ? PASS_STR : FAIL_STR);

    uint64_t page5 = allocate_page();
    printf("Allocated forth page at address: 0x%x (expected: 0x%x) - %s\r\n", page5,
           LOW_MEMORY + PAGE_SIZE * 3, (page5 == LOW_MEMORY + PAGE_SIZE * 3) ? PASS_STR : FAIL_STR);

    printf("current stack pointer: 0x%x (expected: 0x%x) - %s\r\n", get_sp(), current_sp,
           (get_sp() == current_sp) ? PASS_STR : FAIL_STR);

    printf("\r\n\r\n");
}