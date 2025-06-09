#pragma once

#include "common.h"

/**
 * returns the current exception level (EL) of the CPU.
 */
uint32_t get_current_el();

/**
 * set the value of system registers on EL3
 */
void configure_el3();

/**
 * set the value of system registers on EL2
 */
void configure_el2();

/**
 * set the value of system registers on EL1
 */
void configure_el1();

/**
 * switches from el2 to el1 while perserving the current lr and sp
 */
void switch_to_el1_from_el2();

/**
 * switches from el3 to el1 while perserving the current lr and sp
 */
void switch_to_el1_from_el3();

uint64_t get_scr_el3();
uint64_t get_hcr_el2();
uint64_t get_sctlr_el1();
uint64_t get_spsr_el1();
uint64_t get_elr_el1();