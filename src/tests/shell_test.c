#include "common.h"
#include "shell/input_stream.h"
#include "shell/shell.h"
#include "utils/buffer.h"
#include "utils/printf.h"
void test_input_stream_struct()
{
    printf("\r\n\r\n");
    printf("testing input_stream struct...\r\n");
    input_stream_init(get_input_stream());
    printf("Input buffer initialized.\r\n");
    printf("Buffer size: %d (expected = %d)\r\n", get_input_stream()->buffer_size,
           STREAM_BUFFER_SIZE);
    printf("Input index: %d (expected = 0)\r\n", get_input_stream()->index);
    printf("Input buffer: '%s' (expected = '')\r\n", get_input_stream()->buffer);
    printf("Has data: %d (expected = 0)\r\n", get_input_stream()->has_data);

    printf("\r\n\r\n");

    printf("Writing 'First line\\n' to input buffer...\r\n");
    write_string(get_input_stream(), "First line\n");
    printf("Buffer size: %d (expected = %d)\r\n", get_input_stream()->buffer_size,
           STREAM_BUFFER_SIZE);
    printf("Input index: %d (expected = 11)\r\n", get_input_stream()->index);
    printf("Input buffer: %s (expected = first line\\n)\r\n", get_input_stream()->buffer);
    printf("Has data: %d (expected = 1)\r\n", get_input_stream()->has_data);

    printf("\r\n\r\n");

    printf("Reading line from input buffer...\r\n");
    char read_buffer[64];
    read_line_from_input_stream(get_input_stream(), read_buffer, 64);
    printf("Read line: %s (expected = first line)\r\n", read_buffer);

    printf("\r\n\r\n");

    printf("reading first 64 bytes from input buffer...\r\n");
    read_input_stream(get_input_stream(), read_buffer, 64);
    printf("Read buffer: %s (expected = first line\\n)\r\n", read_buffer);

    printf("\r\n\r\n");

    printf("writing 'Another line\\n' to input buffer...\r\n");
    write_string(get_input_stream(), "Another line\n");
    printf("Buffer size: %d (expected = %d)\r\n", get_input_stream()->buffer_size,
           STREAM_BUFFER_SIZE);
    printf("Input index: %d (expected = 24)\r\n", get_input_stream()->index);
    printf("Input buffer: %s (expected = first line\\nAnother line\\n)\r\n",
           get_input_stream()->buffer);
    printf("Has data: %d (expected = 1)\r\n", get_input_stream()->has_data);

    printf("\r\n\r\n");

    printf("Reading line from input buffer...\r\n");
    read_line_from_input_stream(get_input_stream(), read_buffer, 64);
    printf("Read buffer: %s (expected = first line\\n)\r\n", read_buffer);

    printf("\r\n\r\n");

    printf("reading first 64 bytes from input buffer...\r\n");
    read_input_stream(get_input_stream(), read_buffer, 64);
    printf("Input buffer: %s (expected = first line\\nAnother line\\n)\r\n",
           get_input_stream()->buffer);

    printf("\r\n\r\n");

    printf("Clearing input buffer...\r\n");
    clear_input_stream(get_input_stream());
    printf("Buffer size: %d (expected = %d)\r\n", get_input_stream()->buffer_size,
           STREAM_BUFFER_SIZE);
    printf("Input index: %d (expected = 0)\r\n", get_input_stream()->index);
    printf("Input buffer: %s (expected = '')\r\n", get_input_stream()->buffer);
    printf("Has data: %d (expected = 0)\r\n", get_input_stream()->has_data);

    printf("\r\n\r\n");

    printf("Writing 'Test after clear' to input buffer...\r\n");
    write_string(get_input_stream(), "Test after clear");
    printf("Buffer size: %d (expected = %d)\r\n", get_input_stream()->buffer_size,
           STREAM_BUFFER_SIZE);
    printf("Input index: %d (expected = 16)\r\n", get_input_stream()->index);
    printf("Input buffer: %s (expected = Test after clear)\r\n", get_input_stream()->buffer);
    printf("Has data: %d (expected = 0)\r\n", get_input_stream()->has_data);

    printf("\r\n\r\n");

    printf("Writing 'ing\\n' to input buffer...\r\n");
    write_string(get_input_stream(), "ing\n");
    printf("Buffer size: %d (expected = %d)\r\n", get_input_stream()->buffer_size,
           STREAM_BUFFER_SIZE);
    printf("Input index: %d (expected = 20)\r\n", get_input_stream()->index);
    printf("Input buffer: %s (expected = Test after clearing\\n)\r\n", get_input_stream()->buffer);
    printf("Has data: %d (expected = 1)\r\n", get_input_stream()->has_data);
    printf("\r\n\r\n");

    printf("finished testing input_stream struct.\r\n");

    printf("\r\n\r\n");
}

void test_shell(void)
{
    printf("\r\n\r\n");
    printf("Testing shell functionality...\r\n");

    // Test 1: Shell initialization
    printf("=== Test 1: Shell Initialization ===\r\n");
    struct Shell shell;
    input_stream_init(get_input_stream());
    shell_init(&shell, get_input_stream(), "test_sh");

    printf("Shell name: %s (expected = test_sh)\r\n", shell.name);
    printf("Shell input stream buffer size: %d (expected = %d)\r\n",
           shell.input_stream->buffer_size, STREAM_BUFFER_SIZE);
    printf("Shell input stream index: %d (expected = 0)\r\n", shell.input_stream->index);
    printf("Shell input stream has_data: %d (expected = 0)\r\n", shell.input_stream->has_data);

    printf("\r\n");

    // Test 2: Help command
    printf("=== Test 2: Help Command ===\r\n");
    clear_input_stream(
        shell.input_stream); // Fix: Use shell.input_stream instead of get_input_stream()
    write_string(shell.input_stream, "help\n");
    char command[SHELL_BUFFER_SIZE];
    get_shell_command(&shell, command, SHELL_BUFFER_SIZE);
    printf("Command extracted: %s (expected = help)\r\n", command);
    int result = run_shell_command(&shell, command);
    printf("Help command result: %d (expected = 0)\r\n", result);

    printf("\r\n");

    // Test 3: Ping command
    printf("=== Test 3: Ping Command ===\r\n");
    clear_input_stream(shell.input_stream);
    write_string(shell.input_stream, "ping\n");
    get_shell_command(&shell, command, SHELL_BUFFER_SIZE);
    printf("Command extracted: %s (expected = ping)\r\n", command);
    result = run_shell_command(&shell, command);
    printf("Ping command result: %d (expected = 0)\r\n", result);

    printf("\r\n");

    // Test 4: Tictac command
    printf("=== Test 4: Tictac Command ===\r\n");
    clear_input_stream(shell.input_stream);
    write_string(shell.input_stream, "tictac\n");
    get_shell_command(&shell, command, SHELL_BUFFER_SIZE);
    printf("Command extracted: %s (expected = tictac)\r\n", command);
    result = run_shell_command(&shell, command);
    printf("Tictac command result: %d (expected = 0)\r\n", result);

    printf("\r\n");

    // Test 5: Clear command
    printf("=== Test 5: Clear Command ===\r\n");
    clear_input_stream(shell.input_stream);
    write_string(shell.input_stream, "clear\n");
    get_shell_command(&shell, command, SHELL_BUFFER_SIZE);
    printf("Command extracted: %s (expected = clear)\r\n", command);
    result = run_shell_command(&shell, command);
    printf("Clear command result: %d (expected = 0)\r\n", result);
    printf("Input stream has_data after clear: %d (expected = 0)\r\n",
           shell.input_stream->has_data);

    printf("\r\n");

    // Test 6: Unknown command
    printf("=== Test 6: Unknown Command ===\r\n");
    clear_input_stream(shell.input_stream);
    write_string(shell.input_stream, "unknowncommand\n");
    get_shell_command(&shell, command, SHELL_BUFFER_SIZE);
    printf("Command extracted: %s (expected = unknowncommand)\r\n", command);
    result = run_shell_command(&shell, command);
    printf("Unknown command result: %d (expected = -2)\r\n", result);

    printf("\r\n");

    // Test 7: Empty command
    printf("=== Test 7: Empty Command ===\r\n");
    clear_input_stream(shell.input_stream);
    command[0] = '\0';
    result = run_shell_command(&shell, command);
    printf("Empty command result: %d (expected = -1)\r\n", result);

    printf("\r\n");

    // Test 8: get_shell_command with no data
    printf("=== Test 8: Get Command With No Data ===\r\n");
    clear_input_stream(shell.input_stream);
    get_shell_command(&shell, command, SHELL_BUFFER_SIZE);
    printf("Command with no data: '%s' (expected = '')\r\n", command);

    printf("\r\n");

    // Test 9: Multiple commands in sequence
    printf("=== Test 9: Multiple Commands Sequence ===\r\n");
    const char *test_commands[] = {"help", "ping", "tictac", "clear"};
    int expected_results[] = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++)
    {
        clear_input_stream(shell.input_stream);
        write_string(shell.input_stream, test_commands[i]);
        write_string(shell.input_stream, "\n");
        get_shell_command(&shell, command, SHELL_BUFFER_SIZE);
        printf("Command %d: %s (expected = %s)\r\n", i + 1, command, test_commands[i]);
        result = run_shell_command(&shell, command);
        printf("Result %d: %d (expected = %d)\r\n", i + 1, result, expected_results[i]);
    }

    printf("\r\n");

    printf("Shell comprehensive testing completed.\r\n");
}
