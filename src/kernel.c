#include "common.h"
#include "el.h"
#include "exceptions/daif.h"
#include "exceptions/vectors.h"
#include "io/mini_uart.h"
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

    // ----------------------- vector table -----------------------

    printf("----------------------- start of vector table debug -----------------------\r\n");
    printf("vector table location: 0x%x\r\n", get_vectors_adr());
    printf("vbar + 0x80*0, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 0));
    printf("content: 0x%x <- confirm = d2800030\r\n", *(get_vectors_adr() + 16 * 0));
    printf("vbar + 0x80*1, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 1));
    printf("content: 0x%x <- confirm = d2800050\r\n", *(get_vectors_adr() + 16 * 1));
    printf("vbar + 0x80*2, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 2));
    printf("content: 0x%x <- confirm = d2800070\r\n", *(get_vectors_adr() + 16 * 2));
    printf("vbar + 0x80*3, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 3));
    printf("content: 0x%x <- confirm = d2800090\r\n", *(get_vectors_adr() + 16 * 3));
    printf("vbar + 0x80*4, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 4));
    printf("content: 0x%x <- confirm = d28000b0\r\n", *(get_vectors_adr() + 16 * 4));
    printf("vbar + 0x80*5, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 5));
    printf("content: 0x%x <- confirm = d28000d0\r\n", *(get_vectors_adr() + 16 * 5));
    printf("vbar + 0x80*6, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 6));
    printf("content: 0x%x <- confirm = d28000f0\r\n", *(get_vectors_adr() + 16 * 6));
    printf("vbar + 0x80*7, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 7));
    printf("content: 0x%x <- confirm = d2800110\r\n", *(get_vectors_adr() + 16 * 7));
    printf("vbar + 0x80*8, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 8));
    printf("content: 0x%x <- confirm = d2800130\r\n", *(get_vectors_adr() + 16 * 8));
    printf("vbar + 0x80*9, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 9));
    printf("content: 0x%x <- confirm = d2800150\r\n", *(get_vectors_adr() + 16 * 9));
    printf("vbar + 0x80*10, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 10));
    printf("content: 0x%x <- confirm = d2800170\r\n", *(get_vectors_adr() + 16 * 10));
    printf("vbar + 0x80*11, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 11));
    printf("content: 0x%x <- confirm = d2800190\r\n", *(get_vectors_adr() + 16 * 11));
    printf("vbar + 0x80*12, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 12));
    printf("content: 0x%x <- confirm = d28001b0\r\n", *(get_vectors_adr() + 16 * 12));
    printf("vbar + 0x80*13, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 13));
    printf("content: 0x%x <- confirm = d28001d0\r\n", *(get_vectors_adr() + 16 * 13));
    printf("vbar + 0x80*14, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 14));
    printf("content: 0x%x <- confirm = d28001f0\r\n", *(get_vectors_adr() + 16 * 14));
    printf("vbar + 0x80*15, address: %x, content: %x\r\n", (get_vectors_adr() + 16 * 15));
    printf("content: 0x%x <- confirm = d2800210\r\n", *(get_vectors_adr() + 16 * 15));
    printf("----------------------- end of vector table debug -----------------------\r\n\r\n\r\n");

    // ----------------------- interrupts -----------------------

    printf("vector base register: 0x%x\r\n", get_vector_base_register());
    printf("DAIF register: 0x%x\r\n", get_daif_register());
    printf("Initializing exception vectors and enabling IRQ...\r\n");
    init_exception_vectors_el1();
    enable_irq();
    printf("vector base register: 0x%x <- confirm = 0x%x\r\n", get_vector_base_register(),
           get_vectors_adr());
    printf("DAIF register: 0x%x <- confirm = 0x140\r\n", get_daif_register());
    printf("\r\n\r\n");

    printf("All given input will be mirrored:\r\n");

    while (1)
    {
        char received_char = uart_recv();
        if (received_char == '\r')
        {
            uart_send('\r');
            uart_send('\n');
            continue;
        }
        uart_send(received_char);
    }
}