#include "utils/debug.h"
#include "utils/printf.h"

void alarm()
{
    printf("[ALARM]: %x\n\r", get_x16());
    while (1)
    {
    }
}