#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "builtins.h"

/*#ifndef __LOG*/
/*    #define __LOG*/
/*#endif*/

unsigned long read(char* input_buffer, int buffer_size);
int evaluate(char* command, int len);
void print(char* result);


unsigned long read(char* input_buffer, int buffer_size){
    printf("$ ");
    fflush(stdout);
    fgets(input_buffer, buffer_size, stdin);
    return strlen(input_buffer);
}

int evaluate(char* command, int len){
    char* command_cp = malloc(len);
    command[len - 1] = '\0';
    command_cp = strcpy(command_cp, command);
    #ifdef __LOG
        printf("%s\n", command_cp);
    #endif
    char* tokens = strtok(command, " ");
    char* builtins[] = {"exit", "echo", "type", "pwd", "cd"};
    int num_of_builtins = sizeof(builtins) / sizeof(builtins[0]);
    char result[100];
    for (int i = 0; i < num_of_builtins; i++){
        if (strcmp(builtins[i], tokens) == 0){
            if (i == 0){
                sh_exit(tokens);
            } else if (i == 1){
                sh_echo(tokens, len - 1);
            } else if (i == 2){
                sh_type(tokens, builtins, num_of_builtins);
            } else if (i == 3){
                sh_pwd();
            } else if (i == 4){
                sh_cd(tokens + 3);
            }
            return 0;
        }
    }
    int code = try_execute(command_cp);
    if (code == -3){
        sprintf(result, "%s: command not found", command);
        print(result);
    } else {
        return code;
    }
    return 0;
}

void print(char* result){
    printf("%s\n", result);
}

int main() {
    char input[100];
    while (1){
        unsigned long len = read(input, 100);
        #ifdef __LOG
            printf("[INFO] input len: %u\n", (unsigned)len);
        #endif
        evaluate(input, strlen(input));
    }
    
    return 0;
}
