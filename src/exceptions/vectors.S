




/**
 * Exception vectors.
 */
.align	11
.globl vectors
vectors:
    .align  7
    mov x16, #1
    eret

    .align  7
    mov x16, #2
    eret
    
    .align  7
    mov x16, #3
    eret

    .align  7
    mov x16, #4
    eret

    .align  7
    mov x16, #5
    eret

    .align  7
    mov x16, #6
    eret
    
    .align  7
    mov x16, #7
    eret

    .align  7
    mov x16, #8
    eret

    .align  7
    mov x16, #9
    eret

    .align  7
    mov x16, #10
    eret

    .align  7
    mov x16, #11
    eret

    .align  7
    mov x16, #12
    eret

    .align  7
    mov x16, #13
    eret

    .align  7
    mov x16, #14
    eret

    .align  7
    mov x16, #15
    eret

    .align  7
    mov x16, #16
    eret

.globl init_exception_vectors_el1
init_exception_vectors_el1:
    // load VBAR_EL1 with vector table address
    adr	x0, vectors
    msr	vbar_el1, x0 // Vector Base Address Register
    ret

.globl get_vectors_adr
get_vectors_adr:
    adr x0, vectors
    ret

.globl get_vector_base_register
get_vector_base_register:
    mrs x0, vbar_el1
    ret