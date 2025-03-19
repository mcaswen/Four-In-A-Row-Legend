#include "store.h"

goods store_goods[2][3];
button store_quit_1 = NULL;
button store_quit_2 = NULL;
background store_background = NULL;
single_image store_account_coins = NULL;

void store_init() {

    for (int i = 0; i < 2; i++) {

        for (int j = 0; j < 3; j++) {
            
            store_goods[i][j] = (goods)malloc(sizeof(Goods));

            if (i == 0) {
                
                store_goods[i][j]->skin = (skin_type)(i * 3 + j + 1);           
                store_goods[i][j]->is_owned = cur_account->current_own_skins->owned[i * 3 + j + 1];
            
            } else {

                store_goods[i][j]->skin = (skin_type)(i * 3 + j + 2);           
                store_goods[i][j]->is_owned = cur_account->current_own_skins->owned[i * 3 + j + 2];
            
            }
            
            store_goods[i][j]->center_x = INITIAL_POSITION_X + j * GOODS_INTERVAL_X;
            store_goods[i][j]->center_y = INITIAL_POSITION_Y + i * GOODS_INTERVAL_Y;
            store_goods[i][j]->price = GOODS_PRICE;
            store_goods[i][j]->cell_size = GOODS_SIZE;

            char buy_promote[20];
            snprintf(buy_promote, sizeof(buy_promote), "%d", store_goods[i][j]->price);


            init_button(&(store_goods[i][j]->buy), true, store_goods[i][j]->center_x - 60, store_goods[i][j]->center_y + 75
                , BUTTON_SIZE_W, BUTTON_SIZE_H, COLOR_BROWN, buy_promote, Monaco, COLOR_WHITE);

            init_button(&(store_goods[i][j]->equip), false, store_goods[i][j]->center_x - 60, store_goods[i][j]->center_y + 75 + BUTTON_SIZE_H
                , BUTTON_SIZE_W, BUTTON_SIZE_H, COLOR_BROWN, "EQUIP", Monaco, COLOR_WHITE);

            init_button(&(store_goods[i][j]->owned), false, store_goods[i][j]->center_x - 60, store_goods[i][j]->center_y + 75
                , BUTTON_SIZE_W, BUTTON_SIZE_H, COLOR_GRAY, "OWNED", Monaco, COLOR_WHITE);

            init_button(&(store_goods[i][j]->equipped), false, store_goods[i][j]->center_x - 60, store_goods[i][j]->center_y + 75 + BUTTON_SIZE_H
                , BUTTON_SIZE_W, BUTTON_SIZE_H, COLOR_GRAY, "EQUIPPED", Monaco, COLOR_WHITE);

        }

    }

    init_button(&store_quit_1, true, 150, 825, 75, 100, COLOR_NONE, " ", Monaco, COLOR_WHITE);
    init_button(&store_quit_2, true, 785, 310, 75, 95, COLOR_NONE, " ", Monaco, COLOR_WHITE);
    
    init_image_background(&store_background, renderer, "images/game-shop-ui.jpg");

    init_image(&store_account_coins, true, 800, 25, 50, 50, "images/Gold_Icon.png");

}

void store_processing(SDL_Event* event) {

    SDL_RenderClear(renderer);
    
    render_background(store_background, renderer);

    render_image(renderer, store_account_coins);

    char char_coins[50];
    snprintf(char_coins, sizeof(char_coins), "%d", cur_account->coins);
    render_text(renderer, Monaco, char_coins, COLOR_WHITE, 880, 30, 100);

    render_button(store_quit_1, renderer);
    render_button(store_quit_2, renderer);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {

            display_goods(store_goods[i][j], i, j);

        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {

                if (button_click_check(store_goods[i][j]->buy)) {
                    
                    if (cur_account->coins > GOODS_PRICE) {
                        
                        if (i == 0) {
                        
                        store_goods[i][j]->is_owned = true;
                        cur_account->current_own_skins->owned[i * 3 + j + 1] = true; 
                        
                        } else {

                        store_goods[i][j]->is_owned = true;
                        cur_account->current_own_skins->owned[i * 3 + j + 2] = true; 

                        }

                        cur_account->coins -= GOODS_PRICE;
                    
                    }
                
                }

                if (button_click_check(store_goods[i][j]->equip)) {

                    if (i == 0) {

                        cur_account->current_own_skins->black_equipped = store_goods[i][j]->skin;

                    } else {

                        cur_account->current_own_skins->white_equipped = store_goods[i][j]->skin;

                    }

                }

            }

        }

        if (button_click_check(store_quit_1) || button_click_check(store_quit_2)) {
            
            store_end();
            current_state = MENU;

        }
    
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10);

}


void display_goods(goods good, int i, int j) {

    print_piece(renderer, good->skin, good->center_x, good->center_y, good->cell_size);

    if (!good->is_owned) {

        good->buy->is_existing = true;
        good->owned->is_existing = false;

    } else {

        good->owned->is_existing = true;
        good->buy->is_existing = false;

        if ((i == 0 && cur_account->current_own_skins->black_equipped != good->skin) ||
            (i == 1 && cur_account->current_own_skins->white_equipped != good->skin)) {

            good->equipped->is_existing = false;
            good->equip->is_existing = true;

            } else {

            good->equip->is_existing = false;
            good->equipped->is_existing = true;

            }
    
    }

    render_button(good->buy, renderer);
    render_button(good->owned, renderer);
    render_button(good->equip, renderer);
    render_button(good->equipped, renderer);

}


void store_end() {

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {

            destroy_button(store_goods[i][j]->buy);
            destroy_button(store_goods[i][j]->owned);
            destroy_button(store_goods[i][j]->equip);
            destroy_button(store_goods[i][j]->equipped);

            free(store_goods[i][j]);
            store_goods[i][j] = NULL;
        
        }

    }

    destroy_button(store_quit_1);
    destroy_button(store_quit_2);

    store_is_inited = false;

}