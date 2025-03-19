#include "menu.h"

button game_starting = NULL;
button store = NULL;
button recording = NULL;
button exitt = NULL;
button save = NULL;
button menu_save_confirm = NULL;

background menu_background = NULL;

single_image menu_player_head_portrait = NULL;
single_image menu_account_coins = NULL;

bool menu_save_is_confirming = false;
bool menu_save_successed = false;

void menu_init() {

    /* 
    strcpy(temp_account_id, "123");
    strcpy(temp_account_password, "123");

    cur_account = (account)malloc(sizeof(Account));

    init_account(cur_account, temp_account_id, temp_account_password);
    */
    
    init_button(&game_starting, true, 700, 475, 200, 50, COLOR_BROWN, "START", Monaco, COLOR_WHITE);
    init_button(&store, true, 700, 575, 200, 50, COLOR_BROWN, "STORE", Monaco, COLOR_WHITE);
    init_button(&recording, true, 700, 675, 200, 50, COLOR_BROWN, "RECRODINGS", Monaco, COLOR_WHITE);
    init_button(&save, true, 700, 775, 200, 50, COLOR_BROWN, "SAVE", Monaco, COLOR_WHITE);
    init_button(&exitt, true, 700, 875, 200, 50, COLOR_BROWN, "EXIT", Monaco, COLOR_WHITE);
    init_button(&menu_save_confirm, false, 425, 475, 100, 50, COLOR_GRAY, "Confirm", Monaco, COLOR_WHITE);


    init_image(&menu_player_head_portrait, true, 50, 875, 50, 50, "images/Minecraft_Steve_face.png");
    init_image(&menu_account_coins, true, 250, 875, 50, 50, "images/Gold_Icon.png");
    
    init_image_background(&menu_background, renderer, "images/menu_background.jpg");

}

void menu_processing(SDL_Event* event) {

    SDL_RenderClear(renderer);
    
    render_background(menu_background, renderer);
    
    const char* game_name1 = "FourInARow";
    const char* game_name2 = "Legend";

    render_text(renderer, PlaywriteINGuides_Regular, (char*)game_name1, COLOR_BLACK, 175, 50, 400);
    render_text(renderer, PlaywriteINGuides_Regular, (char*)game_name2, COLOR_BLACK, 500, 250, 400);
    render_text(renderer, Monaco, cur_account->ID, COLOR_BLACK, 125, 885, 500);
    
    char char_coins[50];
    snprintf(char_coins, sizeof(char_coins), "%d", cur_account->coins);
    render_text(renderer, Monaco, char_coins, COLOR_BLACK, 325, 885, 500);

    render_button(game_starting, renderer);
    render_button(store, renderer);
    render_button(recording, renderer);
    render_button(save, renderer);
    render_button(exitt, renderer);

    render_image(renderer, menu_player_head_portrait);
    render_image(renderer, menu_account_coins);
    
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        
        if (button_click_check(game_starting)) {
            
            menu_end();
            current_state = GAME;
        
        }

        if (button_click_check(store)) {
            
            menu_end();
            current_state = STORE;
        
        }

        if (button_click_check(recording)) {
            
            menu_end();
            current_state = RECORDINGS;
        
        }

        if (button_click_check(save)) {

            menu_button_reset();
            
            if (save_account_data_to_database(cur_account, 20)) {
                
                menu_save_successed = true;
                
            } else {

                menu_save_successed = false;

            }
            
            menu_save_confirm->is_existing = true;
            menu_save_is_confirming = true;

        }
        
        if (button_click_check(exitt)) {
            
            menu_end();
            current_state = EXIT;
        
        }
   
    }

    if (menu_save_is_confirming) {

        confirm_save(event);

    }
    
    
    SDL_RenderPresent(renderer);
    SDL_Delay(10);

}

void menu_end() {

    menu_is_inited = false;
    
    destroy_button(game_starting);
    destroy_button(store);
    destroy_button(recording);
    destroy_button(exitt);

    destroy_image(menu_player_head_portrait);
    destroy_image(menu_account_coins);

    destroy_background(menu_background);

}

void menu_button_reset() {

    game_starting->is_existing = !game_starting->is_existing;
    store->is_existing = !store->is_existing;
    recording->is_existing = !recording->is_existing;
    save->is_existing = !save->is_existing;
    exitt->is_existing =  !exitt->is_existing;

}

void confirm_save(SDL_Event* event) {

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);

    int border_width = 100;

    SDL_Rect board_rect = {
        SCREEN_PIDDING + 150,
        SCREEN_PIDDING + 150,
        2 * border_width + 150,
        2 * border_width + 50,

    };

    SDL_RenderFillRect(renderer, &board_rect);

    render_button(menu_save_confirm, renderer);

    if (menu_save_successed) {

        const char* menu_save_successed_promote = "Save Completed!";
        render_text(renderer, Monaco, (char*)menu_save_successed_promote, COLOR_WHITE, 380, 350, 400); 

    } else {

        const char* menu_save_failed_promote = "Save Failed";
        render_text(renderer, Monaco, (char*)menu_save_failed_promote, COLOR_WHITE, 380, 350, 400); 

    }

    if (button_click_check(menu_save_confirm) && event->type == SDL_MOUSEBUTTONDOWN) {

        menu_save_is_confirming = false;
        menu_button_reset();
        menu_save_confirm->is_existing = false;

    }

}