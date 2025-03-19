#ifndef BOARD_H
#define BOARD_H
#include "ground.h"

typedef struct {

    skin_type piece;
    bool is_plus;

} BOARD_STRUCT;

extern BOARD_STRUCT BOARD[BOARD_SIZE + 1][BOARD_SIZE + 1];

extern background board_background;

void init_board();
void print_board(SDL_Renderer* renderer);
void print_piece(SDL_Renderer* renderer, skin_type skin, int center_x, int center_y, int cell_size);
void print_board_piece(SDL_Renderer* renderer);

#endif