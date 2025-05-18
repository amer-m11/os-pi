
PHDRS
{
  text PT_LOAD FLAGS(5);    /* RX */
  rodata PT_LOAD FLAGS(4);  /* R */
  data PT_LOAD FLAGS(6);    /* RW */
}

SECTIONS
{
    .text.boot : { *(.text.boot) } :text
    .text :  { *(.text) } :text
    .rodata : { *(.rodata) } :rodata
    .data : { *(.data) } :data
    . = ALIGN(0x8);
    bss_begin = .;
    .bss : { *(.bss*) } :data
    bss_end = .;
}