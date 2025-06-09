#include "shell/shell.h"
#include "common.h"
#include "utils/buffer.h"
#include "utils/printf.h"

int64_t shell_space[SHELL_BUFFER_SIZE + 32];
struct Shell *SHELL = (struct Shell *)shell_space; // Fix: Cast to

struct Shell *get_shell(void)
{
    return SHELL;
}

void shell_init(struct Shell *shell, struct InputStream *input_stream, const char *name)
{
    shell->input_stream = input_stream;
    int index = 0;
    for (index = 0; index < APP_NAME_LENGTH - 1 && name[index] != '\0'; index++)
    {
        shell->name[index] = name[index];
    }
    shell->name[index] = '\0';
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
        printf("No command entered.\r\n");
        return -1;
    }

    if (string_equals(command, "help"))
    {
        printf("Available commands:\r\n");
        printf("  help - Show this help message\r\n");
        printf("  ping - Respond with 'pong' (debugging)\r\n");
        printf("  tictac - Start the Tic Tac Toe game\r\n");
    }
    else if (string_equals(command, "clear"))
    {
        clear_input_stream(shell->input_stream);
        printf("Input stream cleared.\r\n");
    }
    else if (string_equals(command, "ping"))
    {
        printf("pong\n");
    }
    else if (string_equals(command, "tictac"))
    {
        // Return a special code to indicate game mode
        return 1;
    }
    else
    {
        printf("Unknown command: %s\r\n", command);
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
            printf("Error running command: %s\r\n", command);
        }
    }
}
