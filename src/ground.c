#include "ground.h"


bool RUNNING = true;
bool menu_state = true;
bool game_state = false;
bool store_state = false;
bool recordings_state = false;



//初始化，防止编译错误
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;


//窗口初始化
void windows_init() {

    printf("Initializing window...\n");

    window = SDL_CreateWindow(

        "CHESS",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_SIZE,
        SCREEN_SIZE,
        SDL_WINDOW_SHOWN
                
    );

    
    if (!window) {
    
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    
    }
    
    else {
        
        printf("Window created\n");

    }
    
    printf("Rendering board...\n");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
    
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    
    }

    else {
        
        printf("Board rendered\n");

    }

    SDL_SetRenderDrawColor(renderer, 240, 255, 150, 100);

}

void windows_clear() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}


//背景
void init_color_background(background* bkg, SDL_Color color) {

    *bkg = (background)malloc(sizeof(Background));

    (*bkg)->type = BACKGROUND_COLOR;

    (*bkg)->color = color;

}

void init_image_background(background* bkg, SDL_Renderer* renderer, const char* file_path) {

    *bkg = (background)malloc(sizeof(Background));

    (*bkg)->type = BACKGROUND_IMAGE;

    //snprintf(bkg->image.file_path, sizeof(bkg->image.file_path), "%s", file_path);
    (*bkg)->image.texture = IMG_LoadTexture(renderer, file_path);

}

void destroy_background(background bkg) {

    if (bkg->type == BACKGROUND_IMAGE) {

        SDL_DestroyTexture(bkg->image.texture);
        bkg->image.texture = NULL;

    }

    free(bkg);

}

void render_background(background bkg, SDL_Renderer* renderer) {

    if (bkg->type == BACKGROUND_COLOR) {

        SDL_SetRenderDrawColor(renderer, bkg->color.r, bkg->color.b, bkg->color.g, bkg->color.a);
        SDL_RenderClear(renderer);

    }
    else if (bkg->type == BACKGROUND_IMAGE) {

        SDL_RenderCopy(renderer, bkg->image.texture, NULL, NULL);

    }
}

// 文本

void render_line(SDL_Renderer* renderer, TTF_Font* FONT, char* text, SDL_Color color, int x, int y) {
    
    SDL_Surface* text_surface = TTF_RenderText_Blended(FONT, text, color);
    
    if (!text_surface) {
        
        printf("Failed to create text surface: %s\n", TTF_GetError());
        return;
    
    }
    
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    
    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};
    
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
   
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);

}


void render_text(SDL_Renderer* renderer, TTF_Font* FONT, char* input_text, SDL_Color color, int x, int y, int max_width) {

    int text_length = strlen(input_text);
    char line[256] = {0};
    int line_width = 0;
    int line_count = 0;
    int i = 0;//文本索引

    //按字符一个个处理，整合成单词后放到一行中
    while (i < text_length) {
        
        int word_width = 0;
        int j = i;//当前单词索引

        //计算单词宽度
        while (j < text_length && input_text[j] != ' ' && input_text[j] != '\n') {
            
            char temp[2] = {input_text[j], '\0'}; //处理单个字符时的临时字符串，为了之后的TTF_SizeText调用
            int char_w = 0, char_h = 0;
            TTF_SizeText(FONT, temp, &char_w, &char_h); // 获取字母宽度
            word_width += char_w;
            j++;
        
        }

        //若当前无法容纳该单词，则渲染当前行并换行
        if (line_width + word_width > max_width) {
            //如果当前行有内容，先渲染该行
            if (line_width > 0) {
            line[line_width] = '\0';
            
            render_line(renderer, FONT, line, color, x, y + line_count * TTF_FontHeight(FONT));

            //重置当前信息
            memset(line, 0, sizeof(line));
            line_count++;
            
            }
            
            line_width = 0;
        }
        
        //若可以容纳，则复制单词到当前行
        if (j - i < sizeof(line) - line_width - 1) { // 确保不超出缓冲区
            
             strncat(line, &input_text[i], j - i);
        
        }
        
        line_width += word_width;
        
        //若单词后有空格则添加空格
        if (j < text_length && input_text[j] == ' ') {

            if (line_width + word_width <= max_width) { // 确保空格不会导致换行
                
                strncat(line, " ", 1);
                line_width++;
            
            }
            
            j++;

        }

        i = j;    
    
    }

    //渲染最后一行
    if (line_width > 0) {
        
        line[line_width] = '\0';
        render_line(renderer, FONT, line, color, x, y + line_count * TTF_FontHeight(FONT));
    }

}

void render_int_to_char(SDL_Renderer* renderer, TTF_Font* FONT, int temp, SDL_Color color, int x, int y) {

    char char_temp[100];
    
    snprintf(char_temp, sizeof(char_temp), "%d", temp);
    
    render_line(renderer, Monaco, char_temp, color , x, y);

}



//图片
void init_image(single_image* img, bool image_is_existing, int x, int y, int w, int h, const char* file_path) {

    *img = (single_image)malloc(sizeof(Single_Image));

    SDL_Surface* original_surface = IMG_Load(file_path);

    SDL_Surface *scaled_surface = SDL_CreateRGBSurface(                  
                                                        0, w, h,
                                                        original_surface->format->BitsPerPixel,
                                                           original_surface->format->Rmask,
                                                        original_surface->format->Gmask,
                                                        original_surface->format->Bmask,
                                                        original_surface->format->Amask);

    SDL_BlitScaled(original_surface, NULL, scaled_surface, NULL);

    (*img)->texture = SDL_CreateTextureFromSurface(renderer, scaled_surface);

    (*img)->is_existing = image_is_existing;
    (*img)->w = w;
    (*img)->h = h;
    (*img)->x = x;
    (*img)->y = y;

    SDL_FreeSurface(original_surface);
    SDL_FreeSurface(scaled_surface);

}

void render_image(SDL_Renderer* renderer, single_image img) {

    if (img->is_existing && img->texture) {
        
        SDL_Rect dest_rect = {img->x, img->y, img->w, img->h};

        SDL_RenderCopy(renderer, img->texture, NULL, &dest_rect);
    
    }
}

void destroy_image(single_image img) {

    if (img->texture) {

        SDL_DestroyTexture(img->texture);
        img->texture = NULL;
        img->is_existing = 0;

    }

    free(img);
    img = NULL;

}

//按钮
void init_button(button* btn, bool btn_is_existing, int x, int y, int w, int h, 
                SDL_Color btn_color, const char* btn_text, TTF_Font* Font, SDL_Color text_color) {

    *btn = (button)malloc(sizeof(Button));
    
    (*btn)->is_existing = btn_is_existing;
    (*btn)->rect.x = x;
    (*btn)->rect.y = y;
    (*btn)->rect.w = w;
    (*btn)->rect.h = h;

    (*btn)->color = btn_color;
    strncpy((*btn)->text, btn_text, sizeof((*btn)->text) - 1);
    
    // 类型转换为字符串
    (*btn)->text[sizeof((*btn)->text) - 1] = '\0';

    (*btn)->font = Font;
    (*btn)->text_color = text_color;

}

void render_button(button btn, SDL_Renderer* renderer) {

    if (btn->is_existing == false)
        return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  

    SDL_SetRenderDrawColor(renderer, btn->color.r, btn->color.g, btn->color.b, btn->color.a);
    
    SDL_RenderFillRect(renderer, &(btn->rect));

    //绘制文本
    SDL_Surface *text_surface = TTF_RenderText_Blended(btn->font, btn->text, btn->text_color); 
    
    if (!text_surface) {
        
        printf("Failed to create text surface: %s\n", TTF_GetError());
        return;
    
    } // 提前返回，避免访问无效内存
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    
    SDL_Rect text_rect;

    text_rect.w = text_surface->w;
    text_rect.h = text_surface->h;
    text_rect.x = btn->rect.x + (btn->rect.w - text_rect.w) / 2;
    text_rect.y = btn->rect.y + (btn->rect.h - text_rect.h) / 2;

    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    SDL_DestroyTexture(texture);

    SDL_FreeSurface(text_surface);

}

bool button_click_check(button btn) {
    
    if (!btn->is_existing)
        
        return false;
        
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x >= btn->rect.x && x <= btn->rect.x + btn->rect.w 
            && y >= btn->rect.y && y <= btn->rect.y + btn->rect.h
        
        ) {
            
            btn->is_pressed = !btn->is_pressed;

            return true;
        }

    return false;

}

void destroy_button(button btn) {

    btn->is_existing = 0;
    free(btn);
    btn = NULL;

}

//字体
TTF_Font* Courier = NULL;
TTF_Font* MarkerFelt = NULL;
TTF_Font* Monaco = NULL;
TTF_Font* Bigger_Monaco = NULL;
TTF_Font* PlaywriteINGuides_Regular = NULL;
TTF_Font* Inter_Italic_VariableFont_opsz_wght = NULL;

void ttf_init() {

    TTF_Init();
    
    if (TTF_Init() == -1) {
        
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return;
    
    }
    
    Courier = TTF_OpenFont("ttf/Courier.ttf", 24);
    MarkerFelt = TTF_OpenFont("ttf/MarkerFelt.ttf", 24);
    Monaco = TTF_OpenFont("ttf/Monaco.ttf", 24);
    Bigger_Monaco = TTF_OpenFont("ttf/Monaco.ttf", 70);
    PlaywriteINGuides_Regular = TTF_OpenFont("ttf/PlaywriteINGuides-Regular.ttf", 100);
    Inter_Italic_VariableFont_opsz_wght = TTF_OpenFont("ttf/Inter-Italic-VariableFont_opsz,wght.ttf", 16);

    if (!Monaco) {

        printf("ttf loading error!\n");

    }

}

void ttf_free() {
    
    if (Courier) TTF_CloseFont(Courier);
    if (MarkerFelt) TTF_CloseFont(MarkerFelt);
    if (Monaco) TTF_CloseFont(Monaco);
    if (Inter_Italic_VariableFont_opsz_wght) TTF_CloseFont(Inter_Italic_VariableFont_opsz_wght);
    if (PlaywriteINGuides_Regular) TTF_CloseFont(PlaywriteINGuides_Regular);
    if (Bigger_Monaco) TTF_CloseFont(Bigger_Monaco);

    TTF_Quit();
    
}

//账号与数据库操作
void print_fixed_width(FILE* file, char* str, int width) {

    fprintf(file, "%-*.*s", width, width, str);

}

void change_int_to_char(FILE* file, int temp, int width) {

    char char_temp[width];
    snprintf(char_temp, sizeof(char_temp), "%d", temp);
    print_fixed_width(file, char_temp, width);

}


void create_account_to_database(account acc, int width) {

    FILE* data = fopen(DATA_FILE, "r+"); 

    long last_account_pos = 0;
    char line[256];

    while (fgets(line, sizeof(line), data)) {
        last_account_pos = ftell(data);
    }

    for (int i = 0; i < 200; i++) {

        fprintf(data, "              \n");

    }

    print_fixed_width(data, acc->ID, width);
    print_fixed_width(data, acc->password, width);
    change_int_to_char(data, acc->coins, width);
    change_int_to_char(data, acc->games_amount, width);

    char char_own_skins[width];
    memset(char_own_skins, 0, sizeof(char_own_skins));

    for (int i = 0; i < SKIN_COUNT; i++) {
        
        char temp_single_skin[width];
        snprintf(temp_single_skin, sizeof(temp_single_skin), "%d", acc->current_own_skins->owned[i]);
        strcat(char_own_skins, temp_single_skin);
        strcat(char_own_skins, " ");
    
    }

    print_fixed_width(data, char_own_skins, width);
    change_int_to_char(data, acc->current_own_skins->black_equipped, width);
    change_int_to_char(data, acc->current_own_skins->white_equipped, width);

    fclose(data);
}

bool save_account_data_to_database(account acc, int width) {

    FILE* data = fopen(DATA_FILE, "r+");

    char line[256];
    long pos = 0;
    bool account_is_found = false;
    int stored_games_account = 0;
    
    //查找账号id
    while (true) {
        
        pos = ftell(data); //获取当前读取位置

        if (fgets(line, sizeof(line), data)) {

            if (strncmp(line, acc->ID, strlen(acc->ID)) == 0) {//通过strncmp指定line中被比较字符串的大小

                account_is_found = true;
                
                sscanf(line + width * 3, "%d", &stored_games_account);

                break;
            
            }
        
        }
    
    }

    if (account_is_found) {

        fseek(data, pos, SEEK_SET);

    } else {

       return false; //若未搜取到，则返回假

    }

    print_fixed_width(data, acc->ID, 20);
    print_fixed_width(data, acc->password, 20);

    change_int_to_char(data, acc->coins, 20);
    change_int_to_char(data, acc->games_amount, 20);

    char char_own_skins[width];

    memset(char_own_skins, 0, sizeof(char_own_skins));

    for (int i = 0; i < SKIN_COUNT; i++) {

        char temp_single_skin[width];

        snprintf(temp_single_skin, sizeof(temp_single_skin), "%d", acc->current_own_skins->owned[i]);
        strcat(char_own_skins, temp_single_skin);
        strcat(char_own_skins, " ");

    }

    print_fixed_width(data, char_own_skins, 20);

    change_int_to_char(data, acc->current_own_skins->black_equipped, width);
    change_int_to_char(data, acc->current_own_skins->white_equipped, width);

    for (int i = 1; i <= acc->games_amount; i++) {
        
        fprintf(data, "\n%*s", 7 * width, " ");

        change_int_to_char(data, acc->current_games_recording[i]->index, 20);
        change_int_to_char(data, acc->current_games_recording[i]->full_time, 20);
        change_int_to_char(data, acc->current_games_recording[i]->score, 20);

        print_fixed_width(data, acc->current_games_recording[i]->result, 20);

    }

    fprintf(data, "\n");

    fseek(data, pos, SEEK_END);

    fclose(data);

    return true;

}

/*
bool save_account_data_to_database(account acc, int width) {
    
    FILE* data = fopen(DATA_FILE, "r+");

    char id[20];
    char password[20];
    long pos = -1;
    char line[256];

    while (true) {
        
        fseek(data, pos, SEEK_SET);

        fgets(line, sizeof(line), data);

        sscanf(line, "%s %s", id, password);
        if (strcmp(acc->ID, id) == 0)
            
            break;
        
    
    }

    if (pos == -1) {

        return false;

    }


    print_fixed_width(data, acc->ID, 20);
    print_fixed_width(data, acc->password, 20);
    change_int_to_char(data, acc->coins, 20);
    change_int_to_char(data, acc->games_amount, 20);

    char char_own_skins[width];
    memset(char_own_skins, 0, sizeof(char_own_skins));
    for (int i = 0; i < SKIN_COUNT; i++) {
        
        char temp_single_skin[width];
        
        snprintf(temp_single_skin, sizeof(temp_single_skin), "%d", acc->current_own_skins->owned[i]);
        
        strcat(char_own_skins, temp_single_skin);
        strcat(char_own_skins, " ");
    
    }
    
    print_fixed_width(data, char_own_skins, 20);
    
    change_int_to_char(data, acc->current_own_skins->black_equipped, width);
    change_int_to_char(data, acc->current_own_skins->white_equipped, width);

    for (int i = 1; i <= acc->games_amount; i++) {
        
        fprintf(data, "\n%*s", 7 * width, " ");
        
        change_int_to_char(data, acc->current_games_recording[i]->index, 20);
        change_int_to_char(data, acc->current_games_recording[i]->full_time, 20);
        change_int_to_char(data, acc->current_games_recording[i]->score, 20);
        print_fixed_width(data, acc->current_games_recording[i]->result, 20);
    
    }

    return true;

}
*/

void free_account(account acc) {
    
    for (int i = 1; i < acc->games_amount; i++) {
        
        free(acc->current_games_recording[i]);
    
    }
    
    free(acc->current_own_skins);
    free(acc);

}

/*
//时间控制
Uint32 timer_callback(Uint32 interval, void* param) {
    // 定时器触发的逻辑
    printf("Timer triggered!\n");
    return interval; // 返回值决定下一次触发的时间间隔（毫秒）
}
*/

//随机数获取
int get_random_number(int lower, int upper) {

    return rand() % (upper - lower + 1) + lower;

}

