#include "board.h"

BOARD_STRUCT BOARD[BOARD_SIZE + 1][BOARD_SIZE + 1];
single_image pluses[BOARD_SIZE + 1][BOARD_SIZE + 1];

background board_background;

void init_board() {
    
    for (int i = 0; i <= BOARD_SIZE; i++) {
        
        for (int j = 0; j <= BOARD_SIZE; j++) {
            
            BOARD[i][j].piece = SKIN_LUCENCY;
            BOARD[i][j].is_plus = false;

            init_image( &pluses[i][j], false, 
                        SCREEN_PIDDING + j * CELL_SIZE - CELL_SIZE / 2, 
                        SCREEN_PIDDING + i * CELL_SIZE - CELL_SIZE / 2, 
                        CELL_SIZE, CELL_SIZE, 
                        "images/plus2.png");

        }
    }
}

void print_board(SDL_Renderer* renderer) {

    board_background = (background)malloc(sizeof(Background));

    init_color_background(&board_background, COLOR_WHITE); 
    render_background(board_background, renderer);

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);

    int border_width = 25;

    SDL_Rect board_rect = {
        SCREEN_PIDDING - border_width,
        SCREEN_PIDDING - border_width,
        BOARD_SIZE * CELL_SIZE + 2 * border_width,
        BOARD_SIZE * CELL_SIZE + 2 * border_width,

    };
    
    SDL_RenderFillRect(renderer, &board_rect);
    
    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);//黑色线条

    for (int i = 0; i <= BOARD_SIZE; i++) {

        //水平线
        SDL_RenderDrawLine(renderer,
                           SCREEN_PIDDING,
                           SCREEN_PIDDING + i * CELL_SIZE,
                           SCREEN_PIDDING + BOARD_SIZE * CELL_SIZE,
                           SCREEN_PIDDING + i * CELL_SIZE );

        //垂直线
        SDL_RenderDrawLine(renderer,
                           SCREEN_PIDDING + i * CELL_SIZE,
                           SCREEN_PIDDING,
                           SCREEN_PIDDING + i * CELL_SIZE,
                           SCREEN_PIDDING + BOARD_SIZE * CELL_SIZE);

    }


}

void print_piece(SDL_Renderer* renderer, skin_type skin, int center_x, int center_y, int cell_size) {

    bool is_valid = true;

    switch (skin) {
            
        case DEEP_BLUE:

            SDL_SetRenderDrawColor(renderer, 0, 0, 139, 255);
            break;

        case DEEP_GRAY:

            SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
            break;

        case DEEP_PURPLE:

            SDL_SetRenderDrawColor(renderer, 85, 26, 139, 255);
            break;


        case DEEP_BLACK:

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            break;

        case SHALLOW_WHILE:

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150); 
            break;

    
        case SHALLOW_PINK:

            SDL_SetRenderDrawColor(renderer, 255, 182, 193, 150); 
            break;
        
        case SHALLOW_ORANGE:

            SDL_SetRenderDrawColor(renderer, 255, 200, 124, 150); 
            break;
        
        case SHALLOW_GEREEN:

            SDL_SetRenderDrawColor(renderer, 144, 238, 144, 150); 
            break;
        
        default:

            is_valid = false;

            break;

    }
    if (is_valid) {
        
        int radius = cell_size / 3;

        for (int w = 0; w < radius * 2; w++)
            for (int h = 0; h < radius * 2; h++) {

                int dx = w - radius;
                int dy = h - radius;

                if (dx * dx + dy * dy <= radius * radius)
                    
                    SDL_RenderDrawPoint(renderer, center_x + dx, center_y + dy);

            }
    }

}

void print_board_piece(SDL_Renderer* renderer) {

    for (int i = 0; i <= BOARD_SIZE; i++)
        for (int j = 0; j <= BOARD_SIZE;j++) {
        
            int center_x = SCREEN_PIDDING + j * CELL_SIZE;
            int center_y = SCREEN_PIDDING + i * CELL_SIZE;
            
            print_piece(renderer, BOARD[i][j].piece, center_x, center_y, CELL_SIZE);

            if (BOARD[i][j].is_plus) {

                pluses[i][j]->is_existing = true;
                render_image(renderer, pluses[i][j]);

            } else {

                pluses[i][j]->is_existing = false;

            }
        
        }

}
