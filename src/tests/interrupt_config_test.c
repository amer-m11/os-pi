#include "common.h"
#include "interrupt/daif.h"
#include "interrupt/handler.h"
#include "regs/peripherals/irq.h"
#include "tests/tests.h"
#include "utils/debug.h"
#include "utils/printf.h"

void test_cpu_interrupt_configuration(void)
{
    printf("\r\n\r\n");
    printf("=== TEST: CPU Interrupt Configuration ===\n\r");

    uint64_t vector_base = (uint64_t)get_vector_base_register();
    uint64_t expected_vector_base = (uint64_t)get_vectors_adr();
    printf("Vector base register: 0x%x (expected: 0x%x) - %s\r\n", vector_base,
           expected_vector_base, (vector_base == expected_vector_base) ? PASS_STR : FAIL_STR);

    uint64_t daif = get_daif_register();
    printf("DAIF register: 0x%x (expected: 0x140) - %s\r\n", daif,
           (daif == 0x140) ? PASS_STR : FAIL_STR);

    printf("\r\n\r\n");
}

void test_board_interrupt_configuration(void)
{
    printf("\r\n\r\n");
    printf("=== TEST: Board Interrupt Configuration ===\n\r");

    uint64_t irq0_pending_addr = (uint64_t)&REGS_IRQ->irq0_pending_0;
    printf("irq0_pending_0 addr: 0x%x (expected: 0xfe00b200) - %s\r\n", irq0_pending_addr,
           (irq0_pending_addr == 0xfe00b200) ? PASS_STR : FAIL_STR);

    uint64_t irq0_enable_addr = (uint64_t)&REGS_IRQ->irq0_enable_0;
    printf("irq0_enable_0 addr: 0x%x (expected: 0xfe00b210) - %s\r\n", irq0_enable_addr,
           (irq0_enable_addr == 0xfe00b210) ? PASS_STR : FAIL_STR);

    uint64_t aux_irq = AUX_IRQ;
    printf("AUX_IRQ: 0x%x (expected: 0x20000000) - %s\r\n", aux_irq,
           (aux_irq == 0x20000000) ? PASS_STR : FAIL_STR);

    uint64_t irq0_enable_value = REGS_IRQ->irq0_enable_0;
    printf("irq0_enable_0 value: 0x%x (expected: 0x2) - %s\r\n", irq0_enable_value,
           (irq0_enable_value == 0x2) ? PASS_STR : FAIL_STR); // 0x20000002

    printf("\r\n\r\n");
}