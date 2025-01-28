#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "include/state.h"

static state_t* state_singleton = NULL;

state_t* get_state(){
    if (state_singleton == NULL) {
        state_singleton = malloc(sizeof(state_t));
        char buff[128];
        char* curr_dir = getcwd(buff, 128);
        state_singleton->curr_dir = malloc(sizeof(char) * strlen(curr_dir));
        memcpy(state_singleton->curr_dir, curr_dir, strlen(curr_dir));
        char* home_dir = getenv("HOME");
        state_singleton->HOME_dir = home_dir;
        state_singleton->number_of_builtins = 5;
        char** builtins = malloc(sizeof(char*) * state_singleton->number_of_builtins);
        /* strlen(builtin) + '\0' */
        char* cd = malloc(sizeof(char) * (2 + 1));
        memcpy(cd, "cd", 2 + 1);
        char* pwd = malloc(sizeof(char) * (3 + 1));
        memcpy(pwd, "pwd", 3 + 1);
        char* echo = malloc(sizeof(char) * (4 + 1));
        memcpy(echo, "echo", 4 + 1);
        char* exit = malloc(sizeof(char) * (4 + 1));
        memcpy(exit, "exit", 4 + 1);
        char* type = malloc(sizeof(char) * (4 + 1));
        memcpy(type, "type", 4 + 1);
        *builtins = cd;
        *(builtins + 1) = pwd;
        *(builtins + 2) = echo;
        *(builtins + 3) = exit;
        *(builtins + 4) = type;
        state_singleton->builtins = builtins;
    }
    return state_singleton;
}
