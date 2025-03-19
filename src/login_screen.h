#ifndef LOGIN_SCREEN_H
#define LOGIN_SCREEN_H

#include "ground.h"
#include "main.h"

#define MAX_INPUT_LENGTH 20

extern button log_in;
extern button registration;

extern button login_confirm;
extern button login_quit;
extern button login_id_textbox;
extern button login_password_textbox;

extern button registration_confirm;
extern button registration_quit;
extern button registration_id_textbox;
extern button registration_password_textbox;
extern button registration_reconfirm_password_textbox;


extern char temp_account_id[256];
extern char temp_account_password[256];
extern char output_account_password[256];

//extern bool login_is_inited;

extern background login_background;

extern bool login_is_processing;
extern bool registration_is_processing;
extern bool login_state;

void login_screen_init();
void login_screen_processing(SDL_Event* event);
void login_screen_end();

void login_processing(SDL_Event* event);
void login_reset_status();
void init_login();

void registration_processing(SDL_Event* event);
void registration_reset_status();
void init_registration();

bool is_account_exist(char* id);
void init_account(account new_account, char* id, char* password);
void complete_registration();
bool load_account_from_database(account* cur_account, const char* input_id, const char* input_password);
void complete_login();

extern account cur_account;

extern bool temp_account_id_is_empty;
extern bool temp_account_password_is_empty;
extern bool temp_account_id_has_existed;
extern bool failed_to_find_account;


#endif