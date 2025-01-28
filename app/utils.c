#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/command.h"
#include "include/state.h"
#include "include/utils.h"

bool in_builtins(char* name) {
    state_t* state = get_state();
    for (int i = 0; i < state->number_of_builtins; i++) {
        if (strcmp(name, *(state->builtins + i)) == 0) {
            return true;
        }
    }
    return false;
}

int parse_path(char* path) {
    char* path_r = getenv("PATH");
    strcpy(path, path_r);
    int path_len = strlen(path);
    for (int i = 0; i < path_len; i++) {
        if (*(path + i) == ':') {
            *(path + i) = '\0';
        }
    }
    return path_len;
}

char* find_executable(char* executable_name) {
    int executable_path_len;
    int executable_name_len = strlen(executable_name);
    char path_arr[1024] = {0};
    char* path = (char*)path_arr;
    int path_len = parse_path(path);

    int i = 0;
    while (i < path_len) {
        if (search_directory(executable_name, path) == 1) {
            executable_path_len = strlen(path);
            char* full_path =
                (char*)malloc(executable_name_len + executable_path_len + 2);
            strcpy(full_path, path);
            *(full_path + executable_path_len) = '/';
            for (int j = 0; j < executable_name_len; j++) {
                *(full_path + executable_path_len + j + 1) =
                    *(executable_name + j);
            }
            *(full_path + executable_path_len + executable_name_len + 1) = '\0';
            return full_path;
        }
        int incr = strlen(path);
        path += (incr + 1);
        i += (incr + 1);
    }
    return NULL;
}

int search_directory(char* executable_name, char* dir_name) {
    DIR* dir = opendir(dir_name);
    struct dirent* entry;
    if (dir == NULL) {
        return -1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, executable_name) == 0) {
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

int relative_to_absolute(char* relative, char* buffer, int buffer_size) {
    state_t* state = get_state();
    char buf[1024] = {0};
    char* current_dir = getcwd(buf, 1024);

    if (strlen(current_dir) + strlen(relative) + 2 >= buffer_size){
        fprintf(stderr, "[ERROR] in 'relative_to_absolute': not enough space in buffer\n");
        return -1;
    }
    memcpy(buffer, current_dir, strlen(current_dir));
    *(buffer + strlen(current_dir)) = '/';
    memcpy(buffer + strlen(current_dir) + 1, relative, strlen(relative) + 1);

    return 0;
}

char** prepair_args(command_t command){
    int argc = 1;
    args_t* args = command.args;
    while (args->next != NULL){
        argc++;
        args = args->next;
    }
    /* argc + NULL */
    char** prepaired = malloc(sizeof(char *) * (argc + 1));
    args = command.args;
    for (int i = 0; i < argc; i++){
        *(prepaired + i) = args->arg;
        args = args->next;
    }
    *(prepaired + argc) = NULL;
    return prepaired;
}
