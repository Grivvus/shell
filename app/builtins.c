#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
