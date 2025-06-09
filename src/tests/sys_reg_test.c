#include "el.h"
#include "regs/peripherals/auxiliaries.h"
#include "tests/tests.h"
#include "utils/printf.h"

void test_sys_configuration(void)
{

    printf("\r\n\r\n");
    printf("=== TEST: System Configuration ===\n\r");

    uint64_t sctlr = get_sctlr_el1();
    printf(
        "System Control Register (SCTLR_EL1): 0x%x (expected: 0x30d00800 or 0x30d00801) - %s\r\n",
        sctlr, (sctlr == 0x30d00800 || sctlr == 0x30d00801) ? PASS_STR : FAIL_STR);

    uint64_t hcr = get_hcr_el2();
    printf("Hypervisor Configuration Register (HCR_EL2): 0x%x (expected: 0x80000000) - %s\r\n", hcr,
           (hcr == 0x80000000) ? PASS_STR : FAIL_STR);

    uint64_t scr = get_scr_el3();
    printf("Secure Configuration Register (SCR_EL3): 0x%x (expected: 0x431) - %s\r\n", scr,
           (scr == 0x431) ? PASS_STR : FAIL_STR);

    uint64_t spsr = get_spsr_el1();
    printf("Saved Program Status Register (SPSR_EL1): 0x%x (expected: 0x10) - %s\r\n", spsr,
           (spsr == 0x10) ? PASS_STR : FAIL_STR);

    printf("\r\n\r\n");
}