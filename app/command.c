#include <stdlib.h>

#include "include/command.h"

void free_command(command_t* command) {
    free_args(command->args);
    free(command);
}

void free_args(args_t* args){
    if (args->next != NULL){
        free_args(args->next);
    }
    free(args->arg);
    free(args);
}
