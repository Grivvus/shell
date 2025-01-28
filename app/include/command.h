#pragma once

typedef struct args {
    char* arg;
    struct args* next;
} args_t;

typedef struct command {
    char* command;
    args_t* args;
} command_t;

void free_command(command_t* command);
void free_args(args_t* args);
