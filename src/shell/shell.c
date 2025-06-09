#include "shell/shell.h"
#include "common.h"
#include "utils/buffer.h"

void shell_init(struct Shell *shell, struct InputStream *input_stream, const char *name)
{
    shell->input_stream = input_stream;
    int i;
    for (i = 0; i < APP_NAME_LENGTH && name[i] != '\0'; i++)
    {
        shell->name[i] = name[i];
    }
    shell->name[i] = '\0';
    zero_buffer(shell->buffer, STREAM_BUFFER_SIZE);
    input_stream_init(input_stream);
}

void get_shell_command(struct Shell *shell, char *command, uint32_t size)
{
    if (shell->input_stream->has_data)
    {
        read_line_from_input_stream(shell->input_stream, command, size);
    }
    else
    {
        command[0] = '\0';
    }
}

int run_shell_command(struct Shell *shell, const char *command)
{
    if (command[0] == '\0')
    {
        printf("No command entered.\n");
        return -1;
    }

    if (string_equals(command, "help"))
    {
        printf("Available commands:\n");
        printf("  help - Show this help message\n");
        printf("  ping - Respond with 'pong' (debugging)\n");
        printf("  tictac - Start the Tic Tac Toe game\n");
    }
    else if (string_equals(command, "clear"))
    {
        clear_input_stream(shell->input_stream);
        printf("Input stream cleared.\n");
    }
    else if (string_equals(command, "ping"))
    {
        printf("pong\n");
    }
    else if (string_equals(command, "tictac"))
    {
        printf("Starting Tic Tac Toe game...\n");
        printf("wait for it.......\n");
    }
    else
    {
        printf("Unknown command: %s\n", command);
        return -2;
    }
    return 0;
}

void shell_run(struct Shell *shell)
{
    char command[STREAM_BUFFER_SIZE];
    printf("running shell (%s)", shell->name);
    while (1)
    {
        if (!shell->input_stream->has_data)
        {
            continue;
        }
        get_shell_command(shell, command, STREAM_BUFFER_SIZE);
        if (run_shell_command(shell, command) < 0)
        {
            printf("Error running command: %s\n", command);
        }
    }
}
