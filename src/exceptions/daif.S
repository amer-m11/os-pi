

.globl get_daif_register
get_daif_register:
    mrs x0, daif
    ret


.globl enable_irq
enable_irq:
    msr daifclr, #2 // clears second interrupt mask bit
    ret

.globl disable_irq
disable_irq:
    msr daifset, #2
    ret