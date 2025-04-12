#pragma once

#include "common.h"

void delay(uint64_t ticks);
void put32(uint64_t address, uint32_t value);
uint32_t get32(uint64_t address);