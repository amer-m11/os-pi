#include "shell/input_stream.h"
#include "utils/buffer.h"

char input_buffer[STREAM_BUFFER_SIZE] = {'\0'};

struct InputStream input_stream_object = {
    .buffer = input_buffer, .index = 0, .buffer_size = STREAM_BUFFER_SIZE, .has_data = 0};

struct InputStream *INPUT_STREAM = &input_stream_object;

void input_stream_init(struct InputStream *input_stream)
{
    zero_buffer(input_stream->buffer, input_stream->buffer_size);
    input_stream->index = 0;
}

int write_char(struct InputStream *input_stream, char c)
{
    if (input_stream->index < input_stream->buffer_size - 1)
    {
        input_stream->buffer[input_stream->index++] = c;
        input_stream->buffer[input_stream->index] = '\0';
        if (c == '\n' || c == '\r')
        {
            input_stream->has_data = 1;
        }
        return 0;
    }
    clear_input_stream(input_stream); // <- primitive error handling.. TODO
    return -1;
}
void write_string(struct InputStream *input_stream, const char *str)
{
    while (*str != '\0')
    {
        if (write_char(input_stream, *str) < 0)
        {
            break;
        }
        str++;
    }
}

void read_input_stream(struct InputStream *input_stream, char *buffer, uint32_t size)
{
    for (uint32_t i = 0; i < size && i < input_stream->index; i++)
    {
        buffer[i] = input_stream->buffer[i];
        if (input_stream->buffer[i] == '\0')
        {
            break;
        }
    }
    buffer[size - 1] = '\0';
}

void read_line_from_input_stream(struct InputStream *input_stream, char *buffer,
                                 uint32_t buffer_size)
{
    read_line(input_stream->buffer, buffer, buffer_size);
}
void clear_input_stream(struct InputStream *input_stream)
{
    clear_buffer(input_stream->buffer, input_stream->buffer_size);
    input_stream->index = 0;
    input_stream->has_data = 0;
}