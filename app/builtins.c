#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "include/builtins.h"
#include "include/command.h"
#include "include/state.h"
#include "include/utils.h"

void sh_echo(command_t command){
    args_t *arg = command.args->next;
    while (arg != NULL){
        printf("%s ", arg->arg);
        arg = arg->next;
    }
    printf("\n");
}

void sh_exit(command_t command){
    if (command.args->next == NULL){
        exit(0);
    }

    int exit_code = atoi(command.args->next->arg);
    if (*command.args->next->arg == '0' || exit_code != 0){
        exit(exit_code);
    } else {
        fprintf(stderr, "exit code is not an integer\n");
    }
}

void sh_type(command_t command){
    if (command.args->next == NULL){
        fprintf(stderr, "no name for type\n");
        return;
    }
    char* name = command.args->next->arg;
    if (in_builtins(name)){
        printf("%s is a shell builtin\n", name);
        return;
    }
    char* abs_exec_path = find_executable(name);
    if (abs_exec_path != NULL){
        printf("%s is %s\n", name, abs_exec_path);
    } else {
        fprintf(stderr, "%s not found\n", name);
    }
    free(abs_exec_path);
}

void sh_pwd(command_t command){
    char buf[256] = {0};
    getcwd(buf, 256);
    printf("%s\n", buf);
    /*state_t* state = get_state();*/
    /*printf("%s\n", state->curr_dir);*/
}

void sh_cd(command_t command){
    char* dest;
    char* new_CWD = malloc(sizeof(char) * 512);
    state_t* state = get_state();
    free(state->curr_dir);
    state->curr_dir = new_CWD;
    if (command.args->next != NULL){
        dest = command.args->next->arg;
    } else {
        dest = "~";
    }
    if (dest[0] == '/'){
        // absolute
        int res = chdir(dest);
        memcpy(new_CWD, dest, strlen(dest) + 1);
        if (res != 0){
            printf("cd: %s: No such file or directory\n", dest);
        }
    } else if (strcmp(dest, "~") == 0){
        int res = chdir(state->HOME_dir);
        /* allocate strlen + '\0' */
        memcpy(state->curr_dir, state->HOME_dir, strlen(state->HOME_dir) + 1);
    } else {
        // relative
        char* buffer = new_CWD;
        int rel_to_abs = relative_to_absolute(dest, buffer, 512);
        if (rel_to_abs == -1){
            printf("[ERROR] unexpected error while transforming relative path to absolute\n");
            exit(-1);
        }
        int res = chdir(buffer);
        if (res != 0){
            printf("cd: %s: No such file or directory\n", dest);
            printf("%s\n", buffer);
        }
    }
}

