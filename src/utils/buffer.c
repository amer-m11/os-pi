#include "utils/buffer.h"

void zero_buffer(char *buffer, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        buffer[i] = '\0';
    }
}

void clear_buffer(char *buffer, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\0')
        {
            break;
        }
        buffer[i] = '\0';
    }
}

void read_line(char *from_buffer, char *to_buffer, uint32_t to_buffer_size)
{
    uint32_t index = 0;
    while (index < to_buffer_size - 1)
    {
        char c = from_buffer[index];
        if (c == '\n' || c == '\r')
        {
            to_buffer[index] = '\0';
            break;
        }
        to_buffer[index] = c;
        index++;
    }
    to_buffer[to_buffer_size - 1] = '\0';
}

int string_equals(const char *first_str, const char *second_string)
{
    while (*first_str && (*first_str == *second_string))
    {
        first_str++;
        second_string++;
    }
    return *first_str == *second_string;
}
