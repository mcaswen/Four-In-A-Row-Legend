#ifndef RECORDINGS_H
#define RECORDINGS_H

#include "ground.h"
#include "main.h"

#define START_X 500
#define START_Y 200
#define CELL_WIDTH 100
#define CELL_HEIGHT 50
#define MAX_ROWS 10




void render_table(SDL_Renderer* renderer);

void recordings_init();
void recordings_processing(SDL_Event* event);
void recordings_end();

extern background recordings_background;

extern button the_last_page;
extern button the_next_page;
extern button recordings_quit;

extern int start_index;

#endif

