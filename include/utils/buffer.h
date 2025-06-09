#pragma once

#include "common.h"

void zero_buffer(char *buffer, uint32_t size);
void clear_buffer(char *buffer, uint32_t size);
void read_line(char *from_buffer, char *to_buffer, uint32_t to_buffer_size);
int string_equals(const char *first_str, const char *second_string);