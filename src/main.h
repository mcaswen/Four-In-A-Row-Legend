#ifndef MAIN_H
#define MAIN_H

#include "ground.h"
#include "game.h"
#include "menu.h"
#include "login_screen.h"
#include "store.h"
#include "recordings.h"

enum GAMESTATE {
    
    LOGIN = 0,
    MENU = 1,
    GAME = 2,
    STORE = 3,
    RECORDINGS = 4,
    EXIT = 5,

};

extern enum GAMESTATE current_state;
extern bool game_is_inited;
extern bool menu_is_inited;
extern bool store_is_inited;
extern bool recordings_is_inited;

#endif