#include <string.h>
#include <stdio.h>

#include "include/command.h"
#include "include/eval.h"
#include "include/parser.h"

const static int MAX_INPUT_SIZE = 100;

int main() {
    char input[MAX_INPUT_SIZE];
    while (1){
        printf("$ ");
        fflush(stdout);
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL){
            return 0;
        }
        int len = strlen(input);
        input[len - 1] = '\0';
        command_t command = parse_command(input);
        evaluate(command);
        free_args(command.args);
    }
    
    return 0;
}

