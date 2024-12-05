#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
    int num_of_builins = 2;
    char builtins[2][4] = {"echo", "exit"};
    printf("$ ");
    fflush(stdout);
    
    char input[100];
    fgets(input, 100, stdin);
    int len = strlen(input);
    input[len-1] = '\0';
    for (int i = 0; i < num_of_builins; i++){
        if (strcmp((char*)input, (char*)builtins[i]) == 0){
            printf("do something\n");
            exit(0);
        }
    }
    
    printf("%s: command not found\n", (char*)input);
    exit(1);
    return 0;
}
