#include "shell/tictac.h"
#include "io/mini_uart.h"
#include "utils/buffer.h"
#include "utils/printf.h"

void tictac_init(struct TicTacToe *game, struct InputStream *input_stream)
{
    game->input_stream = input_stream;
    game->current_player = 1;

    // Initialize the board with empty spaces
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            game->board[i][j] = ' ';
        }
    }

    char buffer[STREAM_BUFFER_SIZE];

    // Get player 1 name
    printf("Player 1, enter your name: ");
    clear_input_stream(input_stream);

    // Handle UART input directly here
    while (!input_stream->has_data)
    {
        char received_char = uart_recv();
        if (received_char == '\r')
        {
            uart_send('\r');
            uart_send('\n');
            write_char(input_stream, '\n');
        }
        else
        {
            uart_send(received_char);
            write_char(input_stream, received_char);
        }
    }

    read_line_from_input_stream(input_stream, buffer, STREAM_BUFFER_SIZE);
    printf("Player 1 name entered: %s\r\n", buffer);

    // Copy name
    int i = 0;
    while (i < MAX_PLAYER_NAME - 1 && buffer[i] != '\0')
    {
        game->player1_name[i] = buffer[i];
        i++;
    }
    game->player1_name[i] = '\0';

    // Get player 2 name
    printf("Player 2, enter your name: ");
    clear_input_stream(input_stream);

    // Handle UART input directly here
    while (!input_stream->has_data)
    {
        char received_char = uart_recv();
        if (received_char == '\r')
        {
            uart_send('\r');
            uart_send('\n');
            write_char(input_stream, '\n');
        }
        else
        {
            uart_send(received_char);
            write_char(input_stream, received_char);
        }
    }

    read_line_from_input_stream(input_stream, buffer, STREAM_BUFFER_SIZE);

    // Copy name
    i = 0;
    while (i < MAX_PLAYER_NAME - 1 && buffer[i] != '\0')
    {
        game->player2_name[i] = buffer[i];
        i++;
    }
    game->player2_name[i] = '\0';

    // Set symbols
    printf("%s, choose your symbol (X/O): ", game->player1_name);
    clear_input_stream(input_stream);

    // Handle UART input directly here
    while (!input_stream->has_data)
    {
        char received_char = uart_recv();
        if (received_char == '\r')
        {
            uart_send('\r');
            uart_send('\n');
            write_char(input_stream, '\n');
        }
        else
        {
            uart_send(received_char);
            write_char(input_stream, received_char);
        }
    }

    read_line_from_input_stream(input_stream, buffer, STREAM_BUFFER_SIZE);

    if (buffer[0] == 'O' || buffer[0] == 'o' || buffer[0] == '0')
    {
        game->player1_symbol = 'O';
        game->player2_symbol = 'X';
    }
    else
    {
        game->player1_symbol = 'X';
        game->player2_symbol = 'O';
    }
}

void tictac_display_board(struct TicTacToe *game)
{
    printf("\r\n");
    printf("  0 1 2\r\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%c", game->board[i][j]);
            if (j < BOARD_SIZE - 1)
            {
                printf("|");
            }
        }
        printf("\r\n");
        if (i < BOARD_SIZE - 1)
        {
            printf("  -+-+-\r\n");
        }
    }
    printf("\r\n");
}

int tictac_make_move(struct TicTacToe *game, int row, int col)
{
    // Check if position is valid
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
    {
        printf("Position (%d,%d) is outside the board! Use values 0-2.\r\n", row, col);
        return 0;
    }

    // Check if position is already taken
    if (game->board[row][col] != ' ')
    {
        printf("Position (%d,%d) is already taken! Choose another position.\r\n", row, col);
        return 0;
    }

    // Make move
    game->board[row][col] =
        (game->current_player == 1) ? game->player1_symbol : game->player2_symbol;
    return 1;
}

int tictac_check_win(struct TicTacToe *game)
{
    char symbol = (game->current_player == 1) ? game->player1_symbol : game->player2_symbol;

    // Check rows
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (game->board[i][0] == symbol && game->board[i][1] == symbol &&
            game->board[i][2] == symbol)
        {
            return 1;
        }
    }

    // Check columns
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (game->board[0][i] == symbol && game->board[1][i] == symbol &&
            game->board[2][i] == symbol)
        {
            return 1;
        }
    }

    // Check diagonals
    if (game->board[0][0] == symbol && game->board[1][1] == symbol && game->board[2][2] == symbol)
    {
        return 1;
    }

    if (game->board[0][2] == symbol && game->board[1][1] == symbol && game->board[2][0] == symbol)
    {
        return 1;
    }

    return 0;
}

int tictac_is_board_full(struct TicTacToe *game)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (game->board[i][j] == ' ')
            {
                return 0;
            }
        }
    }
    return 1;
}

// Custom string length function
int get_string_length(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

// Custom number parsing function
int parse_two_digits(const char *buffer, int *row, int *col)
{
    int len = get_string_length(buffer);

    // Try parsing with space format "1 2"
    if (len >= 3 && buffer[1] == ' ')
    {
        if (buffer[0] >= '0' && buffer[0] <= '9' && buffer[2] >= '0' && buffer[2] <= '9')
        {
            *row = buffer[0] - '0';
            *col = buffer[2] - '0';
            return 1;
        }
    }
    // Try parsing without space format "12"
    else if (len == 2)
    {
        if (buffer[0] >= '0' && buffer[0] <= '9' && buffer[1] >= '0' && buffer[1] <= '9')
        {
            *row = buffer[0] - '0';
            *col = buffer[1] - '0';
            return 1;
        }
    }

    return 0;
}

void tictac_run(struct InputStream *input_stream)
{
    struct TicTacToe game;
    char buffer[STREAM_BUFFER_SIZE];
    int row, col;

    tictac_init(&game, input_stream);
    printf("\r\n=== TIC TAC TOE ===\r\n");
    printf("%s (%c) vs %s (%c)\r\n\r\n", game.player1_name, game.player1_symbol, game.player2_name,
           game.player2_symbol);

    int game_over = 0;

    while (!game_over)
    {
        tictac_display_board(&game);

        char player_symbol = (game.current_player == 1) ? game.player1_symbol : game.player2_symbol;
        const char *player_name =
            (game.current_player == 1) ? game.player1_name : game.player2_name;

        printf("%s's turn (%c). Enter row column (e.g. '1 2' or '12') or 'exit': ", player_name,
               player_symbol);

        // Get input - handle UART directly
        clear_input_stream(input_stream);
        while (!input_stream->has_data)
        {
            char received_char = uart_recv();
            if (received_char == '\r')
            {
                uart_send('\r');
                uart_send('\n');
                write_char(input_stream, '\n');
            }
            else
            {
                uart_send(received_char);
                write_char(input_stream, received_char);
            }
        }

        read_line_from_input_stream(input_stream, buffer, STREAM_BUFFER_SIZE);

        // Check for exit command
        if (string_equals(buffer, "exit"))
        {
            printf("Game ended. Returning to shell.\r\n");
            return;
        }

        // Parse move coordinates using custom function
        if (!parse_two_digits(buffer, &row, &col))
        {
            printf("Invalid input format! Use 'row col' or 'rowcol' (e.g. '1 2' or '12').\r\n");
            continue;
        }

        if (!tictac_make_move(&game, row, col))
        {
            continue;
        }

        // Check for win
        if (tictac_check_win(&game))
        {
            tictac_display_board(&game);
            printf("%s (%c) wins!\r\n", player_name, player_symbol);
            game_over = 1;
            continue;
        }

        // Check for draw
        if (tictac_is_board_full(&game))
        {
            tictac_display_board(&game);
            printf("It's a draw!\r\n");
            game_over = 1;
            continue;
        }

        // Switch players
        game.current_player = (game.current_player == 1) ? 2 : 1;
    }

    printf("Game over! Returning to shell.\r\n");
}