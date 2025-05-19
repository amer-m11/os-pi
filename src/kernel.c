#include "common.h"
#include "el.h"
#include "interrupt/daif.h"
#include "interrupt/handler.h"
#include "interrupt/vectors.h"
#include "io/mini_uart.h"
#include "regs/peripherals/irq.h"
#include "utils.h"
#include "utils/debug.h"
#include "utils/printf.h"

void putc(void *pointer, char char_to_send)
{
    (void)pointer; // Ignore unused parameter
    uart_send(char_to_send);
}

void kernel_main(void)
{
    // ----------------------- first alive signal -----------------------
    uart_init();
    uart_send('\r');
    uart_send('\n');
    uart_send('\r');
    uart_send('\n');
    uart_send_string("Kernel is up and running!\r\n");
    uart_send_string("\r\n\r\n");

    // ######################## start of setup ########################

    // ----------------------- printf -----------------------
    uart_send_string("Initializing printf...");
    init_printf(0, putc);
    uart_send_string("done\r\n\r\n\r\n");

    // ----------------------- exception levels configuration -----------------------
    printf("Kernel running on exception level: %d <- confirm = 3\r\n", get_current_el());
    printf("System Control Register (SCTLR_EL1): 0x%x\r\n", get_sctlr_el1());
    printf("Hypervisor Configuration Register (HCR_EL2): 0x%x\r\n", get_hcr_el2());
    printf("Secure Configuration Register (SCR_EL3): 0x%x\r\n", get_scr_el3());
    printf("Saved Program Status Register (SPSR_EL1): 0x%x\r\n", get_spsr_el1());
    printf("Configuring exception levels...\r\n");
    configure_el1();
    configure_el2();
    configure_el3();
    printf("System Control Register (SCTLR_EL1): 0x%x <- confirm = 0x30d00800 or 0x30d00801\r\n",
           get_sctlr_el1());
    printf("Hypervisor Configuration Register (HCR_EL2): 0x%x <- confirm = 0x80000000\r\n",
           get_hcr_el2());
    printf("Secure Configuration Register (SCR_EL3): 0x%x <- confirm = 0x431\r\n", get_scr_el3());
    printf("Switching to EL1...\r\n");
    switch_to_el1_from_el3();
    printf("Kernel running on exception level: %d <- confirm = 1\r\n", get_current_el()); // EL1
    printf("Saved Program Status Register (SPSR_EL1): 0x%x <- TODO: confirm that 0x10 is the "
           "correct value\r\n",
           get_spsr_el1());
    uart_send_string("\r\n\r\n");

    // ----------------------- x16 -----------------------
    // set x16 to zero for later use as a debugging register
    // NOTE: x16 is usually used to hold the system call number
    printf("x16: 0x%x\r\n", get_x16());
    set_x16(0);
    printf("x16: 0x%x <- confirm = 0x0\r\n", get_x16());
    uart_send_string("\r\n\r\n");

    // ----------------------- interrupts cpu specific -----------------------

    printf("vector base register: 0x%x\r\n", get_vector_base_register());
    printf("DAIF register: 0x%x\r\n", get_daif_register());
    printf("Initializing exception vectors and enabling IRQ...\r\n");
    init_exception_vectors_el1();
    enable_irq();
    printf("vector base register: 0x%x <- confirm = 0x%x\r\n", get_vector_base_register(),
           get_vectors_adr());
    printf("DAIF register: 0x%x <- confirm = 0x140\r\n", get_daif_register());
    printf("\r\n\r\n");

    // ----------------------- interrupts pi board specific -----------------------
    printf("irq0_pending_0 addr: 0x%x <- confirm = 0xfe00b200\r\n", &REGS_IRQ->irq0_pending_0);
    printf("irq0_enable_0 addr: 0x%x <- confirm = 0xfe00b200\r\n", &REGS_IRQ->irq0_enable_0);
    printf("AUX_IRQ : 0x%x <- confirm = 0x20000000\r\n", AUX_IRQ);
    printf("enabling interrupt controller...\r\n");
    enable_interrupt_controller();
    printf("irq0_enable_0: 0x%x <- confirm = 0x20000000\r\n", REGS_IRQ->irq0_enable_0);

    printf("\r\n\r\n");

    // ######################## end of setup ########################

    printf("All given input will be mirrored:\r\n");

    while (1)
    {
        // delay(10000000);
    }
}