#ifndef GROUND_H
#define GROUND_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string.h>

//窗口尺寸
#define BOARD_SIZE 10
#define CELL_SIZE 65
#define SCREEN_PIDDING 150
#define SCREEN_SIZE BOARD_SIZE * CELL_SIZE + 2 * SCREEN_PIDDING //950

//数据库路径
#define DATA_FILE "data/database.db"

//按钮
typedef struct BUTTON {

    SDL_Rect rect;
    SDL_Color color;
    char text[50];
    TTF_Font *font;
    SDL_Color text_color;
    int is_pressed;
    int is_existing;

}  Button;

typedef Button* button;

void init_button(button* btn,bool button_is_existing, int x, int y, int w, int h,
                SDL_Color btn_color, const char* btn_text, TTF_Font* Font, SDL_Color text_color);
void render_button(button btn, SDL_Renderer* renderer);
bool button_click_check(button btn);
void destroy_button(button btn);

//账号状态与数据库
typedef enum {

    DEEP_BLACK,
    DEEP_PURPLE,
    DEEP_BLUE,
    DEEP_GRAY,
    
    SHALLOW_WHILE,
    SHALLOW_PINK,
    SHALLOW_GEREEN,
    SHALLOW_ORANGE,
    
    SKIN_COUNT,
    SKIN_LUCENCY,

} skin_type;

typedef struct OWN_SKINS {

    bool owned[SKIN_COUNT];
    skin_type black_equipped;
    skin_type white_equipped;

} Own_Skins;

typedef Own_Skins* own_skins;

typedef struct GAMES_RECORDING {

    int full_time;
    int score;
    char result[10];
    int index;

} Games_Recording;

typedef Games_Recording* games_recording;

typedef struct ACCOUNT {

    char ID[100];
    char password[100];
    int coins;
    own_skins current_own_skins;
    games_recording current_games_recording[100];
    int games_amount;

} Account;

typedef Account *account;

void print_fixed_width(FILE* file, char* str, int width);
void create_account_to_database(account acc, int width);
void free_account(account acc);
bool save_account_data_to_database(account acc, int width);

//窗口与渲染
extern SDL_Window* window;
extern SDL_Renderer* renderer;
void windows_init();
void windows_clear();

//运行与事件
extern bool RUNNING;
extern SDL_Event event;

//字体
extern TTF_Font* Courier;
extern TTF_Font* MarkerFelt;
extern TTF_Font* Monaco;
extern TTF_Font* Bigger_Monaco;
extern TTF_Font* PlaywriteINGuides_Regular;
extern TTF_Font* Inter_Italic_VariableFont_opsz_wght;

void ttf_init();
void ttf_free();

//颜色
#define COLOR_WHITE (SDL_Color){255, 255, 255, 255}
#define COLOR_BLACK (SDL_Color){0, 0, 0, 255}
#define COLOR_BLUE  (SDL_Color){0, 0, 255, 255}
#define COLOR_RED (SDL_Color){255, 0, 0, 255}
#define COLOR_GREEN (SDL_Color){0, 255, 0, 255}
#define COLOR_GRAY  (SDL_Color){128, 128, 128, 255}
#define COLOR_BROWN (SDL_Color){101, 67, 33, 255}
#define COLOR_NONE (SDL_Color){255, 255, 255, 0}

//文本
void render_line(SDL_Renderer* renderer, TTF_Font* FONT, char* text, SDL_Color color, int x, int y);
void render_text(SDL_Renderer* renderer, TTF_Font* FONT, char* input_text, SDL_Color color, int x, int y, int max_width);
void render_int_to_char(SDL_Renderer* renderer, TTF_Font* FONT, int temp, SDL_Color color, int x, int y);

//背景
typedef enum {

    BACKGROUND_COLOR,
    BACKGROUND_IMAGE,

} Background_type;

typedef struct BACKGROUND{
    Background_type type;

    union {

        SDL_Color color;

        struct {

            SDL_Texture* texture;
            //char file_path[256];

        } image;

    };
 
} Background;

typedef Background* background;

void init_color_background(background* bkg, SDL_Color color);
void init_image_background(background* bkg, SDL_Renderer* renderer, const char* file_path);
void render_background(background bkg, SDL_Renderer* renderer);
void destroy_background(background bkg);

//图片
typedef struct {

    bool is_existing;
    SDL_Texture *texture;
    int x;
    int y;
    int w;
    int h;
    //char file_path[256];

} Single_Image;

typedef Single_Image* single_image;

void init_image(single_image* img, bool image_is_existing, int x, int y, int w, int h, const char* file_path);
void render_image(SDL_Renderer* renderer, single_image img);
void destroy_image(single_image img);

//随机数获取
int get_random_number(int lower, int upper);

#endif

