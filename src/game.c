#include "game.h"
#include "board.h"
#include "main.h"

//变量有声明就必须有定义
button game_processing_btn_quit = NULL;
button game_processing_btn_option = NULL;
button game_quit_btn_confirm = NULL;
button game_quit_btn_cancel = NULL;
button game_end_btn_confirm = NULL;

single_image game_player_head_portrait = NULL;
single_image game_opponent_head_portrait = NULL;
single_image game_player_HP_image = NULL;
single_image game_opponent_HP_image = NULL;


int current_player = 1;
int valid_put_down_loc_x = -1;
int valid_put_down_loc_y = -1;

Uint32 game_start_time = 0;
Uint32 game_end_time = 0;

int game_score = 0;
int player_cur_HP = 0;
int opponent_cur_HP = 0;
int game_time = 0;
int round_refresh_rate = 0;

sequence_info player_sequence_info = NULL;
sequence_info opponent_sequence_info = NULL;

bool game_is_end = false;
bool is_confirming_game_quit = false;
bool put_down_is_valid = false;
bool game_is_end_settling = false;
bool check_put_down_is_allowed = false;
bool game_data_is_saved = false;
bool waiting_for_mouse_up = false;
bool last_click_is_valid = false;

void game_init() {

    init_board();

    //timer_start_counting_down();

    init_button(&game_processing_btn_option, true, 850, 850, 100, 50, COLOR_GRAY, "Option", Monaco, COLOR_WHITE);
    init_button(&game_processing_btn_quit, false, 850, 800, 100, 50, COLOR_GRAY, "Quit", Monaco, COLOR_WHITE);
    
    init_button(&game_quit_btn_confirm, false, 325, 475, 110, 50, COLOR_GRAY, "Confirm", Monaco, COLOR_BLACK);
    init_button(&game_quit_btn_cancel, false, 515, 475, 110, 50, COLOR_GRAY, "Cancel", Monaco, COLOR_BLACK);

    init_button(&game_end_btn_confirm, false, 400, 650, 150, 75, COLOR_GRAY, "Confirm", Monaco, COLOR_BLACK);

    init_image(&game_player_head_portrait, true, 650, 875, 50, 50, "images/Minecraft_Steve_face.png");
    init_image(&game_opponent_head_portrait, true, 100, 50, 50, 50, "images/Minecraft_Creeper_face.png");
    init_image(&game_player_HP_image, true, 525, 875, 30, 30, "images/HP2.jpeg");
    init_image(&game_opponent_HP_image, true, 175, 50, 30, 30, "images/HP2.jpeg");
    
    current_player = 1;

    game_is_end = false;
    is_confirming_game_quit = false;
    put_down_is_valid = false;
    game_is_end_settling = false;
    check_put_down_is_allowed = true;
    game_data_is_saved = false;
    //timer_start_time = 0;

    game_score = 0;
    player_cur_HP = 10;
    opponent_cur_HP = 10;

    round_refresh_rate = 3;

    player_sequence_info = (sequence_info)malloc(sizeof(Sequence_Info));
    opponent_sequence_info = (sequence_info)malloc(sizeof(Sequence_Info));

    player_sequence_info->max_sequence = 0;
    opponent_sequence_info->max_sequence = 0;
    player_sequence_info->cur_plus = 0;
    opponent_sequence_info->cur_plus = 0;


    game_start_time = SDL_GetTicks();
    game_end_time = 0;
    game_time = 0;

}


void game_processing(SDL_Event* event) {

        SDL_RenderClear(renderer);

        print_board(renderer);
        print_board_piece(renderer);
        
        render_button(game_processing_btn_option, renderer);        
        render_button(game_processing_btn_quit, renderer);
        
        render_image(renderer, game_player_head_portrait);
        render_image(renderer, game_opponent_head_portrait);

        render_image(renderer, game_player_HP_image);
        render_image(renderer, game_opponent_HP_image);

        render_text(renderer, Monaco, cur_account->ID, COLOR_BLACK, 650, 845, 400);
        
        const char* ai_name = "Master";
        render_text(renderer, Monaco, (char*)ai_name, COLOR_BLACK, 75, 20, 400);


        char game_player_HP_text[16];
        snprintf(game_player_HP_text, sizeof(game_player_HP_text), "x %d", player_cur_HP);
        render_line(renderer, Monaco, game_player_HP_text, COLOR_RED, 575, 875);

        char game_opponent_HP_text[16];
        snprintf(game_opponent_HP_text, sizeof(game_opponent_HP_text), "x %d", opponent_cur_HP);
        render_line(renderer, Monaco, game_opponent_HP_text, COLOR_RED, 225, 50);

        char game_player_plus_text[16];
        snprintf(game_player_plus_text, sizeof(game_player_HP_text), "PLUS: %d", player_sequence_info->cur_plus);
        render_line(renderer, Monaco, game_player_plus_text, COLOR_BLACK, 525, 850);

        char game_opponent_plus_text[16];
        snprintf(game_opponent_plus_text, sizeof(game_opponent_plus_text), "PLUS: %d", opponent_sequence_info->cur_plus);
        render_line(renderer, Monaco, game_opponent_plus_text, COLOR_BLACK, 175, 25);

        char game_score_text[16];
        snprintf(game_score_text, sizeof(game_score_text), "SCORE: %d", game_score);
        render_line(renderer, Monaco, game_score_text, COLOR_BLACK, 750, 50);
        
        
        
        for (int i = 0; i < player_sequence_info->max_sequence; i++) {

            int center_x = 625 - i * 31;
            int center_y = 925;

            print_piece(renderer, cur_account->current_own_skins->black_equipped, center_x, center_y, 30);
            
        }
        
        for (int i = 0; i < opponent_sequence_info->max_sequence; i++) {

            int center_x = 185 + i * 31;
            int center_y = 100;

            print_piece(renderer, DEEP_BLACK, center_x, center_y, 30);
            
        }
        
        
        if (is_confirming_game_quit) {

            game_confirm_quit(event);

        }

        if (current_player == 1) {

            const char* player_promote1 = "Now is your turn";
            const char* player_promote2 = "Please put down your piece";

            render_text(renderer, Monaco, (char*)player_promote1, COLOR_BLACK, 300, 0, 400);
            render_text(renderer, Monaco, (char*)player_promote2, COLOR_BLACK, 300, 50, 400);
        
        }
        
        if (current_player == 0) {

            const char* opponent_promote1 = "Now is your opponent's turn";
            render_text(renderer, Monaco, (char*)opponent_promote1, COLOR_BLACK, 300, 0, 500);

        }
  
        if (!is_confirming_game_quit && check_put_down_is_allowed)
            
            put_down_is_valid = check_valid_put_down(renderer);

        if (event->type == SDL_MOUSEBUTTONDOWN) {

            if (button_click_check(game_processing_btn_quit)) {
                
                game_quit_btn_confirm->is_existing = !game_quit_btn_confirm->is_existing;
                game_quit_btn_cancel->is_existing = !game_quit_btn_cancel->is_existing;
                is_confirming_game_quit = !is_confirming_game_quit;

                //printf("QUIT!\n");

            }
            
            else if (button_click_check(game_processing_btn_option)) {   
                
                game_processing_btn_quit->is_existing = !game_processing_btn_quit->is_existing;

            }
        
            else if (put_down_is_valid && current_player == 0 && !waiting_for_mouse_up) {

                if (opponent_put_down())

                    waiting_for_mouse_up = true;

                else

                    waiting_for_mouse_up = false;

            }

            else if (put_down_is_valid && current_player == 1 && !waiting_for_mouse_up) {

                if (player_put_down()) 

                    waiting_for_mouse_up = true;

                else 

                    waiting_for_mouse_up = false;
            
            
            }      
        
        }

        if (event->type == SDL_MOUSEBUTTONUP && waiting_for_mouse_up) {
            
                round_end();
            
            waiting_for_mouse_up = false;

        }       
        
        if (game_is_end) {

            game_end();
            current_state = MENU;

        }
        
        if (game_is_end_settling) {

            game_processing_btn_option->is_existing = false;
            game_end_settling(event);

        }

        //timer_update();
        //render_timer();
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);

}

void game_end() {

    game_is_inited = false;
    
    destroy_button(game_processing_btn_option);
    destroy_button(game_processing_btn_quit);
    destroy_button(game_quit_btn_cancel);
    destroy_button(game_quit_btn_confirm);
    
    destroy_image(game_player_head_portrait);
    destroy_image(game_opponent_head_portrait);
    destroy_image(game_opponent_HP_image);
    destroy_image(game_player_HP_image);

}

void game_confirm_quit(SDL_Event* event) {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int border_width = 100;

    SDL_Rect board_rect = {
        SCREEN_PIDDING + 150,
        SCREEN_PIDDING + 150,
        2 * border_width + 150,
        2 * border_width + 50,

    };

    SDL_RenderFillRect(renderer, &board_rect);

    render_button(game_quit_btn_confirm, renderer);
    render_button(game_quit_btn_cancel, renderer);

    const char* game_quit_promote = "Do you want to quit? Match Data won't be saved.";

    render_text(renderer, Monaco, (char*)game_quit_promote, COLOR_BLACK, 340, 330, 300);

    if (event->type == SDL_MOUSEBUTTONDOWN) {

        if (button_click_check(game_quit_btn_cancel)) {

            is_confirming_game_quit = false;
            game_quit_btn_confirm->is_existing = false;
            game_quit_btn_cancel->is_existing = false;

        }

        if (button_click_check(game_quit_btn_confirm)) {

            game_is_end = true;

        }

    }

}

bool check_valid_put_down(SDL_Renderer* renderer) {

    int x, y;
    SDL_GetMouseState(&x, &y);

    bool put_down_is_valid = false;

    for (int i = 0; i <= BOARD_SIZE; i++)
        for (int j = 0; j <= BOARD_SIZE; j++) {

            int center_x = SCREEN_PIDDING + j * CELL_SIZE;
            int center_y = SCREEN_PIDDING + i * CELL_SIZE;

            int top_left_x = center_x - CELL_SIZE / 2;
            int top_left_y = center_y - CELL_SIZE / 2;

            if (x >= top_left_x && x <= top_left_x + CELL_SIZE && y >= top_left_y && y <= top_left_y + CELL_SIZE) {

                SDL_Rect valid_put_down_rect = {

                    top_left_x,
                    top_left_y,
                    CELL_SIZE,
                    CELL_SIZE,

                };
                
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                SDL_RenderFillRect(renderer, &valid_put_down_rect);
                
                valid_put_down_loc_x = i;
                valid_put_down_loc_y = j;

                put_down_is_valid = true;

                return put_down_is_valid;

            }
        
        }

    return put_down_is_valid;

}


bool player_put_down() {

    if (BOARD[valid_put_down_loc_x][valid_put_down_loc_y].piece != cur_account->current_own_skins->black_equipped && 
        BOARD[valid_put_down_loc_x][valid_put_down_loc_y].piece != cur_account->current_own_skins->white_equipped) {
        
        BOARD[valid_put_down_loc_x][valid_put_down_loc_y].piece = cur_account->current_own_skins->black_equipped;
        return true;

    } else 
        return false;

}

bool opponent_put_down() {

   //printf("ai is putting down!\n");

    if (BOARD[valid_put_down_loc_x][valid_put_down_loc_y].piece != cur_account->current_own_skins->black_equipped && 
        BOARD[valid_put_down_loc_x][valid_put_down_loc_y].piece != cur_account->current_own_skins->white_equipped) {
        
        BOARD[valid_put_down_loc_x][valid_put_down_loc_y].piece = cur_account->current_own_skins->white_equipped;
        return true;

    } else 
        return false;

}
/*
int check_sequence(int x, int y, int dx, int dy, skin_type player_skin, int* temp_x, int* temp_y) {
    
    int cnt = 0;
    int max_cnt = 0;

    for (int i = 0; i <= 7; i++) {  
        
        int nx = x + i * dx;  
        
        int ny = y + i * dy; 

        if (nx < 0 || nx > BOARD_SIZE || ny < 0 || ny > BOARD_SIZE) 
            
            break; 
        
        if (BOARD[nx][ny].piece == player_skin) {
            
            temp_x[cnt] = nx;
            temp_y[cnt] = ny;
            cnt++;

            if (cnt > max_cnt) {
            
                max_cnt = cnt; 

            }
        
        } 
        
        else {
            
            break;  
        
        }
    
    }

    return max_cnt;
}

void get_max_sequence(skin_type player_skin, sequence_info seq_info) {
    
    int temp_x[8], temp_y[8];

    int max_sequence = 0;

    for (int x = 0; x <= BOARD_SIZE; x++) 
        for (int y = 0; y <= BOARD_SIZE; y++) {
            
            if (BOARD[x][y].piece == player_skin) {
    
                int horizontal = check_sequence(x, y, 1, 0, player_skin, temp_x, temp_y);  // 水平
                
                int vertical = check_sequence(x, y, 0, 1, player_skin, temp_x, temp_y);    // 垂直
                
                int left_diag = check_sequence(x, y, 1, 1, player_skin, temp_x, temp_y);   // 左斜
                
                int right_diag = check_sequence(x, y, 1, -1, player_skin, temp_x, temp_y); // 右斜

                if (horizontal > max_sequence) {
                    
                    max_sequence = horizontal;

                    for (int i = 0; i < max_sequence; i++) {
                        
                        seq_info->x[i] = temp_x[i];
                        seq_info->y[i] = temp_y[i];
                    
                    }
                }
                
                if (vertical > max_sequence) {
                    
                    max_sequence = vertical;
                
                    for (int i = 0; i < max_sequence; i++) {
                        
                        seq_info->x[i] = temp_x[i];
                        seq_info->y[i] = temp_y[i];
                    
                    }
                }
                
                if (left_diag > max_sequence) {
                
                    max_sequence = left_diag;

                    for (int i = 0; i < max_sequence; i++) {
                        
                        seq_info->x[i] = temp_x[i];
                        seq_info->y[i] = temp_y[i];
                    
                    }
                }
                
                if (right_diag > max_sequence) {
                
                    max_sequence = right_diag;
            
                    for (int i = 0; i < max_sequence; i++) {
                        
                        seq_info->x[i] = temp_x[i];
                        seq_info->y[i] = temp_y[i];
                    
                    }
                
                }
            }
        }

    seq_info->max_sequence = max_sequence;
}

void check_plus(sequence_info player_info, sequence_info opponent_info) {

    int temp_player_cur_plus = 0;
    int temp_opponent_cur_plus = 0;

    for (int i = 0; i < player_info->max_sequence; i++) {

        printf("player: %d %d\n", player_info->x[i], player_info->y[i]);

        if (BOARD[player_info->x[i]][player_info->y[i]].is_plus) {

                temp_player_cur_plus++;

            }

    }
    
    for (int i = 0; i < opponent_info->max_sequence; i++) {

        printf("opponent: %d %d\n", player_info->x[i], player_info->y[i]);

        if (BOARD[opponent_info->x[i]][opponent_info->y[i]].is_plus) {

                temp_opponent_cur_plus++;

            }

    }

    player_sequence_info->cur_plus = temp_player_cur_plus;
    opponent_sequence_info->cur_plus = temp_opponent_cur_plus;

}
*/
int check_sequence(int x, int y, int dx, int dy, skin_type player_skin, int* temp_x, int* temp_y, int* max_plus_cnt) {
    
    int cnt = 0;
    int max_cnt = 0;
    int plus_cnt = 0;

    for (int i = 0; i <= 7; i++) {  
        
        int nx = x + i * dx;  
        
        int ny = y + i * dy; 

        if (nx < 0 || nx > BOARD_SIZE || ny < 0 || ny > BOARD_SIZE) 
            
            break; 
        
        if (BOARD[nx][ny].piece == player_skin) {
            
            temp_x[cnt] = nx;
            temp_y[cnt] = ny;
            cnt++;

            if (BOARD[nx][ny].is_plus) {

                plus_cnt++;

            }

            if (cnt > max_cnt) {
            
                max_cnt = cnt; 

            }
        
        } 
        
        else {
            
            break;  
        
        }
    
    }

    *max_plus_cnt = plus_cnt;
    return max_cnt;
}

void update_max_sequence(int x, int y, int dx, int dy, skin_type player_skin,  
                         sequence_info seq_info, int* temp_x, int* temp_y) {
    int plus_cnt = 0; 
    int current_sequence = check_sequence(x, y, dx, dy, player_skin, temp_x, temp_y, &plus_cnt);

    if (current_sequence > seq_info->max_sequence || (current_sequence == seq_info->max_sequence 
        && plus_cnt > seq_info->cur_plus)) {
        
        seq_info->max_sequence = current_sequence;
        seq_info->cur_plus = plus_cnt;

        for (int i = 0; i < seq_info->max_sequence; i++) {
            
            seq_info->x[i] = temp_x[i];
            seq_info->y[i] = temp_y[i];
        
        }
    }
}

void get_max_sequence(skin_type player_skin, sequence_info seq_info) {
    
    int temp_x[8], temp_y[8];   

    for (int x = 0; x <= BOARD_SIZE; x++) 
        for (int y = 0; y <= BOARD_SIZE; y++) {
            
            if (BOARD[x][y].piece == player_skin) {
    
                // 水平方向
                update_max_sequence(x, y, 1, 0, player_skin, 
                                    seq_info, temp_x, temp_y);

                // 垂直方向
                update_max_sequence(x, y, 0, 1, player_skin, 
                                    seq_info, temp_x, temp_y);

                // 右斜方向
                update_max_sequence(x, y, 1, 1, player_skin, 
                                    seq_info, temp_x, temp_y);

                // 左斜方向
                update_max_sequence(x, y, 1, -1, player_skin, 
                                    seq_info, temp_x, temp_y);
            }
        }
}

void refresh_piece(sequence_info seq_info) {

    for (int i = 0; i < seq_info->max_sequence; i++) {
        
        BOARD[seq_info->x[i]][seq_info->y[i]].piece = SKIN_LUCENCY;
        BOARD[seq_info->x[i]][seq_info->y[i]].is_plus = false;
    
    }

}

void player_refresh_piece(int cur_player_max_sequence) {

    int plus_loc = get_random_number(0, cur_player_max_sequence - 1);
    
    printf("plus.loc:%d\n", plus_loc);

    BOARD[player_sequence_info->x[plus_loc]][player_sequence_info->y[plus_loc]].is_plus = true;
    
    player_sequence_info->max_sequence = 0;

}

void opponent_refresh_piece(int cur_opponent_max_sequence) {

    int plus_loc = get_random_number(0, cur_opponent_max_sequence - 1);

    printf("plus.loc:%d\n", plus_loc);

    BOARD[opponent_sequence_info->x[plus_loc]][opponent_sequence_info->y[plus_loc]].is_plus = true;

    opponent_sequence_info->max_sequence = 0;

}

void round_refresh_piece() {

    int random = get_random_number(1, 10);

    if (random < round_refresh_rate) {

        int plus_loc_x = get_random_number(1, BOARD_SIZE);
        int plus_loc_y = get_random_number(1, BOARD_SIZE);

        if (BOARD[plus_loc_x][plus_loc_y].piece == SKIN_LUCENCY) {

            BOARD[plus_loc_x][plus_loc_y].is_plus = true;
        
        }

        else {

            round_refresh_piece();

        }

    }

}

//当两人的连珠数量都大于等于四时， 会结算先手连珠-4 ，后手连珠-3， 再先手连珠-0， 后手连珠-4
/*
void round_end() {
    

    printf("round\n");

    get_max_sequence(cur_account->current_own_skins->black_equipped, player_sequence_info);
    get_max_sequence(SHALLOW_WHILE, opponent_sequence_info);

    int player_max = player_sequence_info->max_sequence;
    int opponent_max = opponent_sequence_info->max_sequence;

    printf("%d %d\n", player_max, opponent_max);

    if (player_max >= 4 && opponent_max >= 4) {
        
        printf("2");

        refresh_piece(player_sequence_info);  
        refresh_piece(opponent_sequence_info);  

        //连珠更少的一方扣除双方连珠数量差的血量
        if (player_max > opponent_max) {
            
            opponent_cur_HP -= (player_max - opponent_max);  
            
            game_score += player_max * 10;  
        } 
        
        else {
            
            player_cur_HP -= (opponent_max - player_max); 
        
        }

        //若双方连珠数量相等，不扣除血量


    } else if (player_max >= 4) {
        
        printf("1");

        refresh_piece(player_sequence_info);  
        
        opponent_cur_HP -= player_max;  
        
        game_score += player_max * 10;  

    } else if (opponent_max >= 4) {
        
        refresh_piece(opponent_sequence_info);  
        
        player_cur_HP -= opponent_max;  

    }

    if (player_cur_HP < 0 || opponent_cur_HP < 0) {

        //game_end_settling();


    }


    // 重置回合状态
    get_max_sequence(cur_account->current_own_skins->black_equipped, player_sequence_info);
    get_max_sequence(SHALLOW_WHILE, opponent_sequence_info);
    
    current_player = !current_player;


}*/


void round_end() {
    
    if (current_player == 1) {

        game_score += 10;

    }
    
    static bool pending_resolution = false;  // 用于标记是否有未处理的连珠状态

    // 检查当前状态
    get_max_sequence(cur_account->current_own_skins->black_equipped, player_sequence_info);
    get_max_sequence(cur_account->current_own_skins->white_equipped, opponent_sequence_info);

    int player_max = player_sequence_info->max_sequence;
    int opponent_max = opponent_sequence_info->max_sequence;

    if (pending_resolution) {

        if (player_max >= 4 && opponent_max >= 4) {
            
            int cur_player_max_sequence = player_sequence_info->max_sequence;
            int cur_opponent_max_sequence = opponent_sequence_info->max_sequence;

            refresh_piece(player_sequence_info);
            refresh_piece(opponent_sequence_info);

            if (player_max > opponent_max) {
                
                opponent_cur_HP -= ((player_max + player_sequence_info->cur_plus) - (opponent_max + opponent_sequence_info->cur_plus));
                game_score += player_max * 10 + player_sequence_info->cur_plus * 20;
            
            } 
            
            else if (player_max < opponent_max) {
                
                player_cur_HP -= ((opponent_max + opponent_sequence_info->cur_plus) - (player_max + player_sequence_info->cur_plus));
            
            }
            
            else {

                if (opponent_sequence_info->cur_plus > player_sequence_info->cur_plus) {

                    player_cur_HP -= ((opponent_max + opponent_sequence_info->cur_plus) - (player_max + player_sequence_info->cur_plus));

                } else {

                    opponent_cur_HP -= ((player_max + player_sequence_info->cur_plus) - (opponent_max + opponent_sequence_info->cur_plus));
                    game_score += player_max * 10 + player_sequence_info->cur_plus * 20;

                }
            
            }

            player_refresh_piece(cur_player_max_sequence);
            opponent_refresh_piece(cur_opponent_max_sequence);     
        
        } 
        
        else if (player_max >= 4) {
            
            int cur_player_max_sequence = player_sequence_info->max_sequence;

            refresh_piece(player_sequence_info);
            opponent_cur_HP -= (player_max + player_sequence_info->cur_plus);
            game_score += player_max * 10;
        
            player_refresh_piece(cur_player_max_sequence);
        
        } 
        
        else if (opponent_max >= 4) {
            
            int cur_opponent_max_sequence = opponent_sequence_info->max_sequence;

            refresh_piece(opponent_sequence_info);
            player_cur_HP -= (opponent_max + opponent_sequence_info->cur_plus);
        
            opponent_refresh_piece(cur_opponent_max_sequence);
        
        }

        // 清空待结算状态
        opponent_sequence_info->cur_plus = 0;
        player_sequence_info->cur_plus = 0;
        pending_resolution = false;

    } 
    
    else {
        
        // 当前回合的检查
        if (player_max >= 4 || opponent_max >= 4) {

            // 进入待结算状态
            pending_resolution = true;
        
        }
    
    }

    if (player_cur_HP <= 0 || opponent_cur_HP <= 0) {
        
        game_is_end_settling = true;
    
    }

    // 换手
    get_max_sequence(cur_account->current_own_skins->black_equipped, player_sequence_info);
    get_max_sequence(cur_account->current_own_skins->white_equipped, opponent_sequence_info);

    current_player = !current_player;

    
    //刷新plus
    round_refresh_piece();

}

void game_end_settling(SDL_Event *event) {

    current_player = -1;

    check_put_down_is_allowed = false;
    
    game_end_btn_confirm->is_existing = true;

    if (!game_end_time) {
    
        game_end_time = SDL_GetTicks();
    
    }
    
    game_time = (game_end_time - game_start_time) / 1000;
    
    if (!game_data_is_saved) {

        save_game_data(&cur_account);

    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int border_width = 100;

    SDL_Rect board_rect = {
        SCREEN_PIDDING + 150,
        SCREEN_PIDDING + 150,
        2 * border_width + 150,
        2 * border_width + 250,

    };
    
    SDL_RenderFillRect(renderer, &board_rect);

    render_button(game_end_btn_confirm, renderer);

    if (opponent_cur_HP <= 0) {
    
        const char* game_result_promote = "You Win!"; 
        render_text(renderer, Monaco, (char*)game_result_promote, COLOR_BLACK, 400, 325, 200);
    
    }

    else {
        
        const char* game_result_promote = "Try Again!"; 
        render_text(renderer, Monaco, (char*)game_result_promote, COLOR_BLACK, 400, 325, 200);

    }

    char game_score_promote[20];
    snprintf(game_score_promote, sizeof(game_score_promote), "SCORE: %d", game_score);

    render_text(renderer, Monaco, game_score_promote, COLOR_BLACK, 400, 400, 200);    

    int game_coins = game_score / 10;

    char game_coins_promote[20];
    snprintf(game_coins_promote, sizeof(game_coins_promote), "Coins Gained: %d", game_coins);

    render_text(renderer, Monaco, game_coins_promote, COLOR_BLACK, 400, 475, 200);

    char game_time_promote[20];
    snprintf(game_time_promote, sizeof(game_time_promote), "Time Used: %d", game_time);

    render_text(renderer, Monaco, game_time_promote, COLOR_BLACK, 400, 550, 200);

    if (event->type == SDL_MOUSEBUTTONDOWN && button_click_check(game_end_btn_confirm)) {
        
        game_end();
        current_state = MENU;
    
    }

}

void save_game_data(account* cur_account) {

    if (!login_state) {

        return;

    }

    (*cur_account)->games_amount++;

    (*cur_account)->current_games_recording[(*cur_account)->games_amount] = (games_recording)malloc(sizeof(Games_Recording));

    (*cur_account)->current_games_recording[(*cur_account)->games_amount]->index = (*cur_account)->games_amount;

     if (opponent_cur_HP <= 0) {

        strcpy((*cur_account)->current_games_recording[(*cur_account)->games_amount]->result, "WIN");
    
    }

    else {

        strcpy((*cur_account)->current_games_recording[(*cur_account)->games_amount]->result, "LOSE");

    }

    (*cur_account)->current_games_recording[(*cur_account)->games_amount]->score = game_score;

    int game_coins = game_score / 10;
    
    (*cur_account)->coins += game_coins;
    
    //printf("%d\n", game_coins);

    (*cur_account)->current_games_recording[(*cur_account)->games_amount]->full_time = game_time;

    game_data_is_saved = true;

    //printf("%d %d %s\n", (*cur_account)->current_games_recording[(*cur_account)->games_amount]->index, (*cur_account)->current_games_recording[(*cur_account)->games_amount]->full_time, (*cur_account)->current_games_recording[(*cur_account)->games_amount]->result);

}


/*
//倒计时
void timer_start_counting_down() {

    timer_start_time = SDL_GetTicks();
    timer_remaining_time = TIMER_DURATION;

}

void timer_update() {

    Uint32 cur_time = SDL_GetTicks();

    timer_remaining_time = TIMER_DURATION - (cur_time - timer_start_time) / 1000;

    printf("%d\n", timer_remaining_time);

}

void render_timer() {

    char timer_text[16];

    snprintf(timer_text, sizeof(timer_text), "%d s left", timer_remaining_time);

    render_text(renderer, Monaco, timer_text, COLOR_BLACK, 800, 75, 200);

    printf("1\n");
}
*/