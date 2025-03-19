#include "recordings.h"

background recordings_background = NULL;

button the_last_page = NULL;
button the_next_page = NULL;
button recordings_quit = NULL;

int start_index = 1;

void recordings_init() {

    init_button(&the_last_page, true, 500, 800, 100, 50, COLOR_BROWN, "LAST", Monaco, COLOR_WHITE);
    init_button(&the_next_page, true, 700, 800, 100, 50, COLOR_BROWN, "NEXT", Monaco, COLOR_WHITE);
    init_button(&recordings_quit, true, 0, 0, 100, 50, COLOR_GRAY, "<-", Monaco, COLOR_WHITE);

    init_image_background(&recordings_background, renderer, "images/menu_background.jpg");

    start_index = 1;

}

void recordings_processing(SDL_Event* event) {

    SDL_RenderClear(renderer);

    render_background(recordings_background, renderer);

    render_button(the_last_page, renderer);
    render_button(the_next_page, renderer);
    render_button(recordings_quit, renderer);
    
    render_table(renderer);

    const char* recordings_promote = "GAMES RECORDINGS";

    render_text(renderer, Bigger_Monaco, (char*)recordings_promote, COLOR_BLACK, 250, 50, 1000);

    for (int row = 0; row < MAX_ROWS && (start_index + row) <= cur_account->games_amount; row++) {
        
        games_recording record = cur_account->current_games_recording[start_index + row];
        
        render_int_to_char(renderer, Monaco, record->index, COLOR_BLACK, START_X + 10, START_Y + (row + 1) * CELL_HEIGHT + 5);

        render_int_to_char(renderer, Monaco, record->full_time, COLOR_BLACK, START_X + CELL_WIDTH + 10, START_Y + (row + 1) * CELL_HEIGHT + 5);

        render_int_to_char(renderer, Monaco, record->score, COLOR_BLACK, START_X + 2 * CELL_WIDTH + 10, START_Y + (row + 1) * CELL_HEIGHT + 5);

        render_line(renderer, Monaco, record->result, COLOR_BLACK, START_X + 3 * CELL_WIDTH + 10, START_Y + (row + 1) * CELL_HEIGHT + 5);
    
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {

         if (button_click_check(the_last_page)) {
            
            if (start_index > 0) {
                
                start_index -= MAX_ROWS;
            
            }
        
        }

        if (button_click_check(the_next_page)) {
            
            if (start_index + MAX_ROWS <= cur_account->games_amount) {
                
                start_index += MAX_ROWS;
            
            }
        
        }

        if (button_click_check(recordings_quit)) {

            recordings_end();
            current_state = MENU;

        }

    }

    if (start_index + MAX_ROWS > cur_account->games_amount) {
        
        the_next_page->is_existing = false; 
    
    } else {
        
        the_next_page->is_existing = true; 
    
    }

    if (start_index == 1) {
        
        the_last_page->is_existing = false; 
    
    } else {
        
        the_last_page->is_existing = true; 
    
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
    
}

void recordings_end() {

    destroy_background(recordings_background);
    
    destroy_button(the_last_page);
    destroy_button(the_next_page);
    destroy_button(recordings_quit);

    recordings_is_inited = false;

}

void render_table(SDL_Renderer* renderer) {

    const char* headers[] = { "Index", "Time", "Score", "Result" };
    int num_columns = sizeof(headers) / sizeof(headers[0]);

    int table_width = CELL_WIDTH * num_columns;
    int table_height = CELL_HEIGHT * (MAX_ROWS + 1); 

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


    for (int i = 0; i <= MAX_ROWS + 1; i++) {
        
        int y = START_Y + i * CELL_HEIGHT;
        
        SDL_RenderDrawLine(renderer, START_X, y, START_X + table_width, y);
    
    }

    for (int j = 0; j <= num_columns; j++) {
        
        int x = START_X + j * CELL_WIDTH;
        
        SDL_RenderDrawLine(renderer, x, START_Y, x, START_Y + table_height);
    
    }

    for (int col = 0; col < num_columns; col++) {
        
        render_line(renderer, Monaco, (char*)headers[col], COLOR_BLACK, START_X + col * CELL_WIDTH + 10, START_Y + 5);
    
    }

}