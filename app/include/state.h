#pragma once

typedef struct state {
    char* HOME_dir;
    char* curr_dir;
    int number_of_builtins;
    char** builtins;
} state_t;

state_t* get_state();
