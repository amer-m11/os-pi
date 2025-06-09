#pragma once

#include "common.h"

#define STREAM_BUFFER_SIZE 256

struct InputStream
{
    char buffer[STREAM_BUFFER_SIZE];
    uint32_t index; /* points to the spot where a new char will be written*/
    uint32_t buffer_size;
    uint8_t has_data;
};

void input_stream_init(struct InputStream *input_stream);
struct InputStream *get_input_stream(void);
int write_char(struct InputStream *input_stream, char character);
void write_string(struct InputStream *input_stream, const char *str);
void read_input_stream(struct InputStream *input_stream, char *buffer, uint32_t size);
void read_line_from_input_stream(struct InputStream *input_stream, char *buffer,
                                 uint32_t buffer_size);
void clear_input_stream(struct InputStream *input_stream);