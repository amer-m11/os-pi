#pragma once
#include "input_stream.h"

#define BOARD_SIZE 3
#define MAX_PLAYER_NAME 16

struct TicTacToe
{
    char board[BOARD_SIZE][BOARD_SIZE];
    char player1_name[MAX_PLAYER_NAME];
    char player2_name[MAX_PLAYER_NAME];
    char player1_symbol;
    char player2_symbol;
    int current_player; // 1 or 2
    struct InputStream *input_stream;
};

void tictac_init(struct TicTacToe *game, struct InputStream *input_stream);
void tictac_display_board(struct TicTacToe *game);
int tictac_make_move(struct TicTacToe *game, int row, int col);
int tictac_check_win(struct TicTacToe *game);
int tictac_is_board_full(struct TicTacToe *game);
void tictac_run(struct InputStream *input_stream);