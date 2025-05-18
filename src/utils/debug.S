
.globl get_x16
get_x16:
    mov x0, x16
    ret

.globl set_x16
set_x16:
    mov x16, x0
    ret

.globl get_vector_base_register
get_vector_base_register:
    mrs x0, vbar_el1
    ret

.globl get_daif
get_daif:
    mrs x0, daif
    ret