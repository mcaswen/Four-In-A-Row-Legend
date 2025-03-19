#ifndef STORE_H
#define STORE_H

#include "ground.h"
#include "menu.h"
#include "board.h"

#define INITIAL_POSITION_X 325
#define INITIAL_POSITION_Y 450
#define GOODS_INTERVAL_X 175
#define GOODS_INTERVAL_Y 250
#define GOODS_SIZE 150
#define GOODS_PRICE 200

#define BUTTON_SIZE_W 125
#define BUTTON_SIZE_H 50


typedef struct GOODS {

    skin_type skin;

    bool is_owned;

    int price;
    int center_x;
    int center_y;
    int cell_size;

    button buy;
    button equip;
    button owned;
    button equipped;

} Goods;

typedef Goods* goods;

void store_init();
void store_processing(SDL_Event* event);
void store_end();

void display_goods(goods good, int i, int j);

extern goods store_goods[2][3];
extern button store_quit_1;
extern button store_quit_2;
extern background store_background;

extern single_image store_account_coins;

#endif