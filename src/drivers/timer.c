#include "drivers/timer.h"
#include "common.h"
#include "regs/peripherals/timer.h"
#include "scheduler/sched.h"
#include "utils/printf.h"

void timer1_init(void)
{
    timer1_set_interval(TIMER_BASE_INTERVAL_CYCLES);
}

void timer1_clear_interrupt(void)
{
    REGS_TIMER->control = TIMER1_CONTROL_BIT;
}

void timer1_set_interval(uint32_t cycles)
{
    REGS_TIMER->compare1 = REGS_TIMER->counter_lower_half + cycles;
}

void timer1_handle_interrupt(void)
{
    LOG("\r\nhandling timer1 interrupts\r\n");
    timer1_set_interval(TIMER_BASE_INTERVAL_CYCLES);
    timer1_clear_interrupt();
    scheduler_tick();
}
