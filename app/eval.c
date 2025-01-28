#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "include/builtins.h"
#include "include/eval.h"
#include "include/utils.h"

int evaluate(command_t command) {
    int exec_res = 0;
    if (strcmp("exit", command.command) == 0) {
        sh_exit(command);
    } else if (strcmp("echo", command.command) == 0) {
        sh_echo(command);
    } else if (strcmp("type", command.command) == 0) {
        sh_type(command);
    } else if (strcmp("pwd", command.command) == 0) {
        sh_pwd(command);
    } else if (strcmp("cd", command.command) == 0) {
        sh_cd(command);
    } else {
        exec_res = try_execute(command);
    }
    return exec_res;
}

int try_execute(command_t command){
    int exec_code = 0;
    char** args = prepair_args(command);
    #ifdef __DEBUG
    for (int i = 0; *(args + i) != NULL; i++){
        printf("%s\n", *(args + i));
    }
    #endif
    char* absolute_path = find_executable(*args);
    if (absolute_path == NULL){
        printf("%s: command not found\n", *args);
        return -3;
    }

    int pid = fork();
    if (pid == 0){
        exec_code = execv(absolute_path, args);
        perror("execv");
        exit(1);
    } else if (pid < 0){
        perror("fork");
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
    free(args);
    free(absolute_path);
    return exec_code;
}
