#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtins.h"

void sh_echo(char* input, int len){
    input += 5;
    int i = 5;
    while (i < len){
        printf("%s", input);
        i += strlen(input) + 1;
        input += (strlen(input) + 1);
        if (i < len){
            printf(" ");
        }
    }
    printf("\n");
}

void sh_exit(char* input, int len){
    int code = atoi(input + 5);
    exit(code);
}

void sh_type(char* input, int len, char* builtins, int num_of_builtins){
    input += 5;
    for (int i = 0; i < num_of_builtins; i++){
        if (strcmp(builtins, input) == 0){
            printf("%s is a shell builtin\n", input);
            return;
        }
        builtins += strlen(builtins) + 1;
    }
    char* absolute_executable = find_executable(input);
    if (absolute_executable != NULL){
        printf("%s is %s\n", input, absolute_executable);
    } else {
    printf("%s: not found\n", input);
    }
}

int parse_path(char* path){
    char* path_r = getenv("PATH");
    strcpy(path, path_r);
    int path_len = strlen(path);
    for (int i = 0; i < path_len; i++){
        if (*(path + i) == ':'){
            *(path + i) = '\0';
        }
    }
    return path_len;
}

char* find_executable(char* executable_name){
    char* params = "";
    int executable_path_len;
    int executable_name_len = strlen(executable_name);
    char* path = (char*)malloc(1000);
    int path_len = parse_path(path);

    int i = 0;
    while (i < path_len){
        if (search_directory(executable_name, path) == 1){
            executable_path_len = strlen(path);
            char* full_path = (char*)malloc(executable_name_len + executable_path_len + 2);
            strcpy(full_path, path);
            *(full_path + executable_path_len) = '/';
            for (int j = 0; j < executable_name_len; j++){
                *(full_path + executable_path_len + j + 1) = *(executable_name + j);
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

int search_directory(char* executable_name, char* dir_name){
    DIR* dir = opendir(dir_name);
    struct dirent* entry;
    if (dir == NULL){
        return -1;
    }
    while ((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, executable_name) == 0){
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}
