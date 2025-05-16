#pragma once

#include "common.h"

/**
 * returns the current exception level (EL) of the CPU.
 */
uint32_t get_current_el();

/**
 * set the value of system register on EL2
 */
void configure_el2();

/**
 * set the value of system register on EL1
 */
void configure_el1();

/**
 * switches from el2 to el1 while perserving the current lr and sp
 */
void switch_to_el1();