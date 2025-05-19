
#include "interrupt/handler.h"
#include "common.h"
#include "io/mini_uart.h"
#include "regs/peripherals/auxiliaries.h"
#include "utils/printf.h"

#include "regs/peripherals/irq.h"

void enable_interrupt_controller()
{
    // only activates the aux interrupt
    REGS_IRQ->irq0_enable_0 = AUX_IRQ;
}

const char *const entry_error_messages[] = {
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",   "FIQ_INVALID_EL1t",   "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",   "FIQ_INVALID_EL1h",   "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64", "FIQ_INVALID_EL0_64", "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32", "FIQ_INVALID_EL0_32", "ERROR_INVALID_EL0_32"};

void show_invalid_entry_message(uint32_t type, uint64_t esr, uint64_t address)
{
    printf("%s, ESR: %x, address: %x\r\n", entry_error_messages[type], esr, address);
}

void handle_irq(void)
{
    uint32_t irq = REGS_IRQ->irq0_pending_0;
    // known issue: the IRQ0_PENDING0 register keeps triggering with value 0x20000000 indicating
    // that an aux interrupt is pending, even if there is none - i.e. the AUX_IRQ register is
    // cleared indicating that no aux interrupt is pending and the second and third bit in
    // AUX_MU_IIR_REG are also cleared also indicating that no mini uart interrupts are pending.
    // printf("pending interrupt: 0x%x\r\n", irq);
    // printf("aux irq status: 0x%x\r\n", REGS_AUX->irq);
    // printf("interrupt identifier: 0x%x\r\n", REGS_AUX->mu_iir);
    while (irq)
    {
        if (irq & AUX_IRQ)
        {
            irq &= ~AUX_IRQ;

            while ((REGS_AUX->mu_iir & 4) == 4)
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
        else
        {
            printf("unsupported interrupt. pending register: 0x%x\r\n", irq);
            while (1)
            {
                // hang
            };
        }
    }
}