#ifndef GAME_H
#define GAME_H
#include "ground.h"

#define TIMER_DURATION 30

//游戏按钮
extern button game_processing_btn_quit;
extern button game_processing_btn_option;
extern button game_quit_btn_confirm;
extern button game_quit_btn_cancel;
extern button game_end_btn_confirm;
//素材
extern single_image game_player_head_portrait;
extern single_image game_opponent_head_portrait;
extern single_image game_player_HP_image;
extern single_image game_opponent_HP_image;

bool check_valid_put_down(SDL_Renderer* renderer);
bool player_put_down();
bool opponent_put_down();

typedef struct SEQUENCE_INFO {

    int x[7];
    int y[7];
    int max_sequence;
    int cur_plus;

} Sequence_Info;

typedef Sequence_Info* sequence_info;

int check_sequence(int x, int y, int dx, int dy, skin_type player_skin, int* temp_x, int* temp_y, int* max_plus_cnt);
void update_max_sequence(int x, int y, int dx, int dy, skin_type player_skin, 
sequence_info seq_info, int* temp_x, int* temp_y);
void get_max_sequence(skin_type player_skin, sequence_info seq_info);
void refresh_piece(sequence_info seq_info);

void player_refresh_piece(int cur_player_max_sequence);
void opponent_refresh_piece(int cur_opponent_max_sequence);
void round_refresh_piece();

void round_end();

void game_processing(SDL_Event* event);
void game_end();
void game_init();
void game_confirm_quit(SDL_Event* event);
void game_end_settling(SDL_Event* event);
void save_game_data(account* cur_account);

/*
void timer_start_counting_down();
void timer_update();
void render_timer();
*/


//游戏状态
extern Uint32 game_start_time;
extern Uint32 game_end_time;

extern int current_player;
extern int valid_put_down_loc_x;
extern int valid_put_down_loc_y;

extern int game_score;
extern int player_cur_HP;
extern int opponent_cur_HP;
extern int game_time;

extern int round_refresh_rate;

extern sequence_info player_sequence_info;
extern sequence_info opponent_sequence_info;
/*
extern Uint32 timer_start_time;
extern int timer_remaining_time;
*/

extern bool game_is_end;
extern bool is_confirming_game_quit;
extern bool put_down_is_valid;
extern bool game_is_end_settling;
extern bool check_put_down_is_allowed;
extern bool game_data_is_saved;
extern bool waiting_for_mouse_up;
extern bool last_click_is_valid;



#endif