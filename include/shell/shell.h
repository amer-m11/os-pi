#pragma once
#include "common.h"
#include "shell/input_stream.h"

#define APP_NAME_LENGTH 8
#define SHELL_BUFFER_SIZE 256

struct Shell
{
    char name[APP_NAME_LENGTH];
    struct InputStream *input_stream;
    char buffer[STREAM_BUFFER_SIZE];
};

struct Shell *get_shell(void);
void shell_init(struct Shell *shell, struct InputStream *input_stream, const char *name);
void get_shell_command(struct Shell *shell, char *command, uint32_t size);
int run_shell_command(struct Shell *shell, const char *command);
void shell_run(struct Shell *shell);