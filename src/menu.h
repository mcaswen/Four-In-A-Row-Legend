#ifndef MEUN_H
#define MEUN_H

#include "ground.h"
#include "main.h"
#include "login_screen.h"

extern button game_starting;
extern button store;
extern button recording;
extern button save;
extern button exitt;
extern button menu_save_confirm;

extern single_image menu_player_head_portrait;
extern single_image menu_account_coins;

extern background menu_background;

extern bool menu_save_is_confirming;
extern bool menu_save_successed;


void menu_init();
void menu_processing(SDL_Event* event);
void menu_end();

void confirm_save(SDL_Event* event);
void menu_button_reset();

#endif