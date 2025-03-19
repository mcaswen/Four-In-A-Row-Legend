#include "main.h"



enum GAMESTATE current_state;
bool game_is_inited;
bool menu_is_inited;
bool store_is_inited;
bool recordings_is_inited;
bool log_in_is_inited;

int main(int argc, const char* argv[]) {

    srand((unsigned int)time(NULL)); // 设置随机种子

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

        printf("SDL LOADING ERROR\n");

        return -1;

    }

    //初始化
    {
    
    ttf_init();
    windows_init();
    
    current_state = LOGIN;
    game_is_inited = false;
    menu_is_inited = false;
    store_is_inited = false;
    recordings_is_inited = false;
    log_in_is_inited = false;

    }
    
    int current_player = 1;
    
    Uint32 last_time = SDL_GetTicks();
    Uint32 interval = 500;  // 每0.5秒模拟一个事件

    while (current_state != EXIT) {

        while (SDL_PollEvent(&event)) {
            
            if (event.type == SDL_QUIT) {
                
                current_state = EXIT;

            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                
                printf("Mouse clicked at: (%d, %d)\n", event.button.x, event.button.y);
            
            }

            switch (current_state) {
                
                case LOGIN:

                    if (!log_in_is_inited) {

                        login_screen_init();
                        log_in_is_inited = true;

                    }

                    login_screen_processing(&event);
                    break;

                case MENU: 
                    
                    if (!menu_is_inited) {

                        menu_init();
                        menu_is_inited = true;

                    }
                    
                    menu_processing(&event);
                    break;

                case GAME:

                    if (!game_is_inited) {

                        game_init();
                        game_is_inited = true;

                    }

                    game_processing(&event);
                    break;

                case STORE:

                    if (!store_is_inited) {

                        store_init();
                        store_is_inited = true;

                    }

                    store_processing(&event);
                    break;
                
                case RECORDINGS:

                    if (!recordings_is_inited) {

                        recordings_init();
                        recordings_is_inited = true;

                    }

                    recordings_processing(&event);

                    break;
                
                case EXIT:

                    break;

            }
        
        }

        Uint32 current_time = SDL_GetTicks();
        
        if (current_time - last_time > interval) {
            
            SDL_Event custom_event;
            custom_event.type = SDL_USEREVENT;  // 自定义事件
            SDL_PushEvent(&custom_event);  // 将虚拟事件推送到队列中
            
            last_time = current_time;
        
        }
    
    }

    if (login_state) {
        
        //save_game_data(&cur_account);
        free_account(cur_account);
    
    }

    windows_clear();
    ttf_free();

    return 0;
}