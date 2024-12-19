#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "builtins.h"

/*#define __LOG*/

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

void sh_exit(char* input){
    int code = atoi(input + 5);
    exit(code);
}

void sh_type(char* input, char** builtins, int num_of_builtins){
    input += 5;
    for (int i = 0; i < num_of_builtins; i++){
        if (strcmp(builtins[i], input) == 0){
            printf("%s is a shell builtin\n", input);
            return;
        }
    }
    char* absolute_executable = find_executable(input);
    if (absolute_executable != NULL){
        printf("%s is %s\n", input, absolute_executable);
        free(absolute_executable);
    } else {
        printf("%s: not found\n", input);
    }
}

void sh_pwd(void){
    char path[1024];
    if (getcwd(path, sizeof(path)) != NULL){
        printf("%s\n", path);
    } else {
        perror("getcwd");
    }
}

int sh_cd(char* dest){
    if (dest[0] == '/'){
        // absolute
        int res = chdir(dest);
        if (res != 0){
            printf("cd: %s: No such file or directory\n", dest);
        }
    } else {
        // relative
        exit(1);
    }
    return 0;
}

int relative_to_absolute(char* relative, char* buffer, int buffer_size){
    char* current_dir = getcwd(buffer, buffer_size);
    if (current_dir == NULL){
        perror("getcwd");
    }
    if (strcmp(relative, ".") == 0){

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
    int executable_path_len;
    int executable_name_len = strlen(executable_name);
    char* path = (char*)malloc(1000);
    void* path_clear = path;
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
            free(path_clear);
            return full_path;
        }
        int incr = strlen(path);
        path += (incr + 1);
        i += (incr + 1);
    }
    free(path_clear);
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

int try_execute(char *command){
    int exec_code = 0;
    int argc;
    char** args = prepair_args(command, &argc);
    char* absolute_path = find_executable(*args);
    if (absolute_path == NULL){
        return -3;
    }
    *args = absolute_path;

    int pid = fork();
    if (pid == 0){
        // it's awful, 2  random bytes coming from somewhere
        *(*(args + 1) + strlen(*(args + 1)) - 2) = '\0';
        exec_code = execv(*args, args);
        perror("execv");
        exit(1);
    } else if (pid < 0){
        perror("fork");
    } else {
        int status;
        waitpid(pid, &status, 0);
    }

    free(command);
    clear_after_execute(args);
    return exec_code;
}

char** prepair_args(char* command, int* argc){
    int len = strlen(command);
    int cnt = 0;
    char* args_text = malloc(len);
    for (int i = 0; i < len; i++){
        *(args_text + i) = *(command + i);
        if (*(args_text + i) == 32){
            cnt++;
            *(args_text + i) = '\0';
        }
    }
    *argc = cnt + 1;
    char** args_ptr = malloc(sizeof(char*) * (cnt + 2));
    for (int i = 0; i < (cnt + 1); i++){
        *(args_ptr + i) = args_text;
        args_text += (strlen(args_text) + 1);
    }
    *(args_ptr + cnt + 1) = (char*)0;

    return args_ptr;
}

int clear_after_execute(char** ptr){
    #ifdef __LOG
        printf("[INFO] releasing resourses\n");
    #endif
    free(*ptr);
    free(ptr);
    return 0;
}
