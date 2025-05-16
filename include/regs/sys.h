#pragma once

// see D24.2 General system control registers and C5.2 Special-purpose registers in
// AArch64-Reference-Manual version L.a

// ----------------- SCTLR_EL1, System Control Register (EL1) -----------------
// page D24-8227

#define SCTLR_RESERVED (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EE_LITTLE_ENDIAN (0 << 25)
#define SCTLR_EOE_LITTLE_ENDIAN (0 << 24)
#define SCTLR_I_CACHE_DISABLED (0 << 12)
#define SCTLR_D_CACHE_DISABLED (0 << 2)
#define SCTLR_MMU_DISABLED (0 << 0)
#define SCTLR_MMU_ENABLED (1 << 0)

// TODO: enable MMU once it is implemented
#define SCTLR_VALUE_MMU_DISABLED                                                                   \
    (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED |   \
     SCTLR_MMU_DISABLED)

// ----------------- HCR_EL2, Hypervisor Configuration Register (EL2) -----------------
// page D24-7579

#define HCR_RW (1 << 31)
#define HCR_VALUE HCR_RW

// ----------------- SPSR_EL2/3, Saved Program Status Register (EL3) -----------------
// page C5-890/C5-901
// 0b0000 EL0.
// 0b0100 EL1 with SP_EL0 (EL1t).
// 0b0101 EL1 with SP_EL1 (EL1h).
// 0b1000 EL2 with SP_EL0 (EL2t).
// 0b1001 EL2 with SP_EL2 (EL2h).

#define SPSR_MASK_ALL (7 << 6)
#define SPSR_EL1h (5 << 0)
#define SPSR_VALUE (SPSR_MASK_ALL | SPSR_EL1h)