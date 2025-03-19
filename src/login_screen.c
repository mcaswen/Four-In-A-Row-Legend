#include "login_screen.h" 

button log_in = NULL;
button registration = NULL;

button login_confirm = NULL;
button login_quit = NULL;
button login_id_textbox = NULL;
button login_password_textbox = NULL;

button registration_confirm = NULL;
button registration_quit = NULL;
button registration_id_textbox = NULL;
button registration_password_textbox = NULL;
button registration_reconfirm_password_textbox = NULL;

background login_background = NULL;

account cur_account = NULL;

bool login_is_processing;
bool registration_is_processing;
bool login_state;
bool temp_account_id_is_empty = true;
bool temp_account_password_is_empty = true;
bool temp_account_id_has_existed = true;
bool failed_to_find_account = true;

char temp_account_id[256];
char temp_account_password[256];
char output_account_password[256];

void login_screen_init() {

    login_is_processing = false;
    registration_is_processing = false;
    login_state = false;

    //初始按钮
    init_button(&log_in, true, 125, 700, 200, 100, COLOR_BROWN, "LOG IN", Monaco, COLOR_WHITE);
    init_button(&registration, true, 625, 700, 200, 100, COLOR_BROWN, "REGISTION", Monaco, COLOR_WHITE);
    
    //登陆按钮
    init_button(&login_confirm, false, 400, 600, 150, 75, COLOR_BROWN, "CONFIRM", Monaco, COLOR_WHITE);
    init_button(&login_quit, false, 750, 150, 50, 50, COLOR_GRAY, "X", Monaco, COLOR_WHITE);
    init_button(&login_id_textbox, false, 250, 275, 450, 50, COLOR_WHITE, " ", Monaco, COLOR_WHITE);
    init_button(&login_password_textbox, false, 250, 400, 450, 50, COLOR_WHITE, " ", Monaco, COLOR_WHITE);

    //注册按钮
    init_button(&registration_confirm, false, 400, 600, 150, 75, COLOR_BROWN, "CONFIRM", Monaco, COLOR_WHITE);
    init_button(&registration_quit, false, 750, 150, 50, 50, COLOR_GRAY, "X", Monaco, COLOR_WHITE);
    init_button(&registration_id_textbox, false, 250, 275, 450, 50, COLOR_WHITE, " ", Monaco, COLOR_WHITE);
    init_button(&registration_password_textbox, false, 250, 400, 450, 50, COLOR_WHITE, " ", Monaco, COLOR_WHITE);
    
    init_image_background(&login_background, renderer, "images/menu_background.jpg");

}

void login_screen_processing(SDL_Event* event) {

    SDL_RenderClear(renderer);
    
    render_background(login_background, renderer);
    
    char game_name1[256] = "FourInARow";
    char game_name2[256] = "Legend";
    
    render_text(renderer, PlaywriteINGuides_Regular, game_name1, COLOR_BLACK, 175, 50, 400);
    render_text(renderer, PlaywriteINGuides_Regular, game_name2, COLOR_BLACK, 500, 250, 400);

    render_button(log_in, renderer);
    render_button(registration, renderer);

    if (event->type == SDL_MOUSEBUTTONDOWN) {

        if (button_click_check(log_in)) {
            
            login_reset_status();

            if (login_is_processing) {

                init_login();

            }
    
        }

        if (button_click_check(registration)) {

            registration_reset_status();

            if (registration_is_processing) {

                init_registration();

            }

        }

        if (button_click_check(login_quit)) {

           login_reset_status();

        }

        if (button_click_check(registration_quit)) {

            registration_reset_status();

        }
    
    }

    if (login_is_processing) {
        
        login_processing(event);

        if (login_state) {
            
            login_screen_end();
            current_state = MENU;
            
        }

    }

    if (registration_is_processing) {
        
        registration_processing(event);

        if (login_state) {
            
            login_screen_end();
            current_state = MENU;
            
        }

    }
    
    SDL_RenderPresent(renderer);
    SDL_Delay(10);

}

void login_screen_end() {

    menu_is_inited = false;

    destroy_button(log_in);
    destroy_button(registration);
    destroy_button(login_id_textbox);
    destroy_button(login_password_textbox);
    destroy_button(registration_confirm);
    destroy_button(registration_quit);
    destroy_button(registration_id_textbox);
    destroy_button(registration_password_textbox);

    destroy_background(login_background);

}


//登陆
void login_processing(SDL_Event* event) {

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);

    int border_width = 200;

    SDL_Rect board_rect = {
        SCREEN_PIDDING,
        SCREEN_PIDDING,
        2 * border_width + 250,
        2 * border_width + 150,

    };

    SDL_RenderFillRect(renderer, &board_rect);
    
    render_button(login_confirm, renderer);
    render_button(login_quit, renderer);
    render_button(login_id_textbox, renderer);
    render_button(login_password_textbox, renderer);

    const char* login_promote_id = "Please enter your account's ID:";
    const char* login_promote_password = "Please enter your account's password:";

    render_text(renderer, Monaco, (char*)login_promote_id, COLOR_WHITE, 250, 225, 500);
    render_text(renderer, Monaco, (char*)login_promote_password, COLOR_WHITE, 250, 350, 600);
    
    render_line(renderer, Inter_Italic_VariableFont_opsz_wght, temp_account_id, COLOR_BLACK, 260, 290);
    render_line(renderer, Inter_Italic_VariableFont_opsz_wght, output_account_password, COLOR_BLACK, 260, 415);

    if (failed_to_find_account) {

        const char* login_promote_failed = "Unknown Account";

        render_text(renderer, Monaco, (char*)login_promote_failed, COLOR_RED, 375, 550, 400);

    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {

        if (!button_click_check(login_id_textbox) &&login_id_textbox->is_pressed) {

            login_id_textbox->is_pressed = !login_id_textbox->is_pressed;

        }

        if (!button_click_check(login_password_textbox) &&login_password_textbox->is_pressed) {

            login_password_textbox->is_pressed = !login_password_textbox->is_pressed;

        }

        if (button_click_check(login_confirm)) {

            complete_login();

        }
    
    }       
    
    if (login_id_textbox->is_pressed) {

            if (event->type == SDL_TEXTINPUT) {

                if (strlen(temp_account_id) + strlen(event->text.text) < MAX_INPUT_LENGTH) {
                    
                    strcat(temp_account_id, event->text.text); 

                }

            }

            //printf("%s\n", temp_account_id);

            if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE) {

                int temp_len = strlen(temp_account_id);

                if (temp_len > 0) {

                    temp_account_id[temp_len - 1] = '\0'; //退格

                }

            }

    }

    if (login_password_textbox->is_pressed) {

        if (event->type == SDL_TEXTINPUT) {

            if (strlen(temp_account_password) + strlen(event->text.text) < MAX_INPUT_LENGTH) {
                    
                strcat(temp_account_password, event->text.text); 

                memset(output_account_password, '*', strlen(temp_account_password));

            }

        }

        //printf("%s\n", temp_account_id);

        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE) {

            int temp_len1 = strlen(temp_account_password);
            int temp_len2 = strlen(output_account_password);

            if (temp_len1 > 0 && temp_len2 > 0) {

                temp_account_password[temp_len1 - 1] = '\0'; //退格
                output_account_password[temp_len2 - 1] = '\0';

            }

        }

    }
}

void login_reset_status() {

    login_is_processing = !login_is_processing;
    failed_to_find_account = !failed_to_find_account;

    log_in->is_existing = !log_in->is_existing;
    registration->is_existing = !registration->is_existing;
    login_confirm->is_existing = !login_confirm->is_existing;
    login_quit->is_existing = !login_quit->is_existing;
    login_id_textbox->is_existing = !login_id_textbox->is_existing;
    login_password_textbox->is_existing = !login_password_textbox->is_existing;

}

void init_login() {

    memset(temp_account_id, 0, sizeof(temp_account_id));
    memset(temp_account_password, 0, sizeof(temp_account_password));
    memset(output_account_password, 0, sizeof(output_account_password));
    
    temp_account_id[0] = '\0';
    temp_account_password[0] = '\0';
    output_account_password[0] = '\0';

}

bool load_account_from_database(account *cur_account, const char* input_id, const char* input_password) {

    FILE* data = fopen(DATA_FILE, "r+");  // 打开数据文件

    char line[1024];
    int width = 20;

    // 遍历数据文件，查找目标账号
    while (fgets(line, sizeof(line), data)) {
        
        // 跳过空行
        if (line[0] == '\n') continue;

        char id[width], password[width];
        int coins, games_amount;
        char owned_skins[width * SKIN_COUNT];
        skin_type black_equipped, white_equipped;

        // %20[^\n]：无视空格读入20个字符的数据，直到遇到换行符
        sscanf(line, "%20s %20s %d %d %20[^\n] %d %d", 
                id, password, &coins, &games_amount, owned_skins, 
                &black_equipped, &white_equipped);

        if (strcmp(id, input_id) == 0 && strcmp(password, input_password) == 0) {

                strncpy((*cur_account)->ID, id, sizeof((*cur_account)->ID) - 1);
                strncpy((*cur_account)->password, password, sizeof((*cur_account)->password) - 1);
                
                (*cur_account)->coins = coins;
                (*cur_account)->games_amount = games_amount;

                (*cur_account)->current_own_skins = (own_skins)malloc(sizeof(own_skins));
                
                char* token = strtok(owned_skins, " ");
                
                for (int i = 0; token && i < SKIN_COUNT; i++) {
                    
                    (*cur_account)->current_own_skins->owned[i] = atoi(token);
                    token = strtok(NULL, " ");
                
                }
                
                (*cur_account)->current_own_skins->black_equipped = black_equipped;
                (*cur_account)->current_own_skins->white_equipped = white_equipped;

                for (int i = 1; i <= games_amount; i++) {
                    
                    if (!fgets(line, sizeof(line), data)) 
                        break;  // 跳过无效行

                    int game_index;
                    char result[width];
                    int full_time, score;

                    sscanf(line + (width * 7), "%d %d %d %20s", &game_index, &full_time, &score, result);

                    if (i == 1) printf("%d %d %d %s", game_index, full_time, score, result);

                    (*cur_account)->current_games_recording[i] = (games_recording)malloc(sizeof(Games_Recording));
                    (*cur_account)->current_games_recording[i]->full_time = full_time;
                    (*cur_account)->current_games_recording[i]->score = score;
                    strcpy((*cur_account)->current_games_recording[i]->result, result);
                    (*cur_account)->current_games_recording[i]->index = game_index;
                }

                // 跳过当前账号结束后的空行
                while (fgets(line, sizeof(line), data) && line[0] == '\n');

                printf("LOG IN!\n");

                fclose(data);
                return true;
            
        }
    }

    printf("FAILED TO LOG IN\n");

    fclose(data);
    return false;
}

 
void complete_login() {

    cur_account = (account)malloc(sizeof(Account));

    if (load_account_from_database(&cur_account, temp_account_id, temp_account_password)) {

        login_state = true;

    } else {

        failed_to_find_account = true;

    }

}

//注册界面
void registration_processing(SDL_Event* event) {

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);

    int border_width = 200;

    SDL_Rect board_rect = {
        SCREEN_PIDDING,
        SCREEN_PIDDING,
        2 * border_width + 250,
        2 * border_width + 150,

    };

    SDL_RenderFillRect(renderer, &board_rect);
    
    render_button(registration_confirm, renderer);
    render_button(registration_quit, renderer);
    render_button(registration_id_textbox, renderer);
    render_button(registration_password_textbox, renderer);

    const char* registration_promote_id = "Please set your account's ID:";
    const char* registration_promote_password = "Please set your account's password:";


    render_text(renderer, Monaco, (char*)registration_promote_id, COLOR_WHITE, 250, 225, 500);
    render_text(renderer, Monaco, (char*)registration_promote_password, COLOR_WHITE, 250, 350, 600);
    
    render_line(renderer, Inter_Italic_VariableFont_opsz_wght, temp_account_id, COLOR_BLACK, 260, 290);
    render_line(renderer, Inter_Italic_VariableFont_opsz_wght, output_account_password, COLOR_BLACK, 260, 415);

    if (temp_account_id_is_empty) {

        const char* registration_promote_id_is_empty = "Invalid ID";

        render_text(renderer, Monaco, (char*)registration_promote_id_is_empty, COLOR_RED, 250, 250, 400);

    }

    if (temp_account_password_is_empty) {

        const char* registration_promote_password_is_empty = "Invalid Password";

        render_text(renderer, Monaco, (char*)registration_promote_password_is_empty, COLOR_RED, 250, 375, 400);


    }

    if (temp_account_id_has_existed) {

        const char* registration_promote_account_is_existed = "ID has existed!";

        render_text(renderer, Monaco, (char*)registration_promote_account_is_existed, COLOR_RED, 375, 525, 400);

    }



    if (event->type == SDL_MOUSEBUTTONDOWN) {

        if (!button_click_check(registration_id_textbox) &&registration_id_textbox->is_pressed) {

            registration_id_textbox->is_pressed = !registration_id_textbox->is_pressed;

        }

        if (!button_click_check(registration_password_textbox) &&registration_password_textbox->is_pressed) {

            registration_password_textbox->is_pressed = !registration_password_textbox->is_pressed;

        }
    
        if (button_click_check(registration_confirm)) {

            if (!strlen(temp_account_id)) {

                temp_account_id_is_empty = true;

            } else {

                temp_account_id_is_empty = false;

            }
            
            if (!strlen(temp_account_password)) {
                
                temp_account_password_is_empty = true;

            } else {

                temp_account_password_is_empty = false;

            }

            if (!temp_account_id_is_empty && !temp_account_password_is_empty) {

                complete_registration();

            }
        }

    }   
    
    
    if (registration_id_textbox->is_pressed) {

        if (event->type == SDL_TEXTINPUT) {

            if (strlen(temp_account_id) + strlen(event->text.text) < MAX_INPUT_LENGTH) {
                    
                    strcat(temp_account_id, event->text.text); 

            }

        }

        //printf("%s\n", temp_account_id);

        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE) {

            int temp_len = strlen(temp_account_id);

            if (temp_len > 0) {

                temp_account_id[temp_len - 1] = '\0'; //退格

            }

        }

    }

    if (registration_password_textbox->is_pressed) {

        if (event->type == SDL_TEXTINPUT) {

            if (strlen(temp_account_password) + strlen(event->text.text) < MAX_INPUT_LENGTH) {
                    
                strcat(temp_account_password, event->text.text); 

                memset(output_account_password, '*', strlen(temp_account_password));

            }

        }

        //printf("%s\n", temp_account_id);

        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE) {

            int temp_len1 = strlen(temp_account_password);
            int temp_len2 = strlen(output_account_password);

            if (temp_len1 > 0 && temp_len2 > 0) {

                temp_account_password[temp_len1 - 1] = '\0'; //退格
                output_account_password[temp_len2 - 1] = '\0';

            }

        }

    }

}

void registration_reset_status() {

    registration_is_processing = !registration_is_processing;
    
    log_in->is_existing = !log_in->is_existing;
    registration->is_existing = !registration->is_existing;
    
    registration_confirm->is_existing = !registration_confirm->is_existing;
    registration_quit->is_existing = !registration_quit->is_existing;
    registration_id_textbox->is_existing = !registration_id_textbox->is_existing;
    registration_password_textbox->is_existing = !registration_password_textbox->is_existing;

    temp_account_id_is_empty = !temp_account_id_is_empty;
    temp_account_password_is_empty = !temp_account_password_is_empty;
    temp_account_id_has_existed = !temp_account_id_has_existed;

}

void init_registration() {

    memset(temp_account_id, 0, sizeof(temp_account_id));
    memset(temp_account_password, 0, sizeof(temp_account_password));
    memset(output_account_password, 0, sizeof(output_account_password));
    
    temp_account_id[0] = '\0';
    temp_account_password[0] = '\0';
    output_account_password[0] = '\0';

}



bool is_account_exist(char* id) {

    FILE* data = fopen(DATA_FILE, "r");

    char exising_id[100];

    while (fscanf(data, "%s", exising_id) == 1) {

        if (strcmp(exising_id, id) == 0) {

            fclose(data);
            return true;

        }

    }

    fclose(data);
    return false;

}

void init_account(account new_account, char* id, char* password) {

    strncpy(new_account->ID, id, sizeof(new_account->ID) - 1);
    new_account->ID[sizeof(new_account->ID) - 1] = '\0';

    strncpy(new_account->password, password, sizeof(new_account->password) - 1);
    new_account->password[sizeof(new_account->password) - 1] = '\0';

    new_account->coins = 0;

    //皮肤
    new_account->current_own_skins = (own_skins)malloc(sizeof(own_skins));

    for (int i = 0; i < SKIN_COUNT; i++) {

        new_account->current_own_skins->owned[i] = false;

    }

    new_account->current_own_skins->owned[0] = true;
    new_account->current_own_skins->owned[4] = true;

    new_account->current_own_skins->black_equipped = DEEP_BLACK;
    new_account->current_own_skins->white_equipped = SHALLOW_WHILE;

    //对局记录

    /*for (int i = 0; i < 99; i++) {

    new_account->current_games_recording[i] = (games_recording)malloc(sizeof(Games_Recording));
    
    }*/
    
    new_account->current_games_recording[0] = (games_recording)malloc(sizeof(Games_Recording));

    new_account->games_amount = 0;

    new_account->current_games_recording[new_account->games_amount]->index = 0;

}

void complete_registration() {

    if (is_account_exist(temp_account_id)) {

        temp_account_id_has_existed = true;

        return;

    } else {

        temp_account_id_has_existed = false;

    }

    cur_account = (account)malloc(sizeof(Account));

    init_account(cur_account, temp_account_id, temp_account_password);

    create_account_to_database(cur_account, 20);
    
    login_state = true;

}   