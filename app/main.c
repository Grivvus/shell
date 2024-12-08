#include <string.h>
#include <stdio.h>

unsigned long read(char* input_buffer, int buffer_size);
void evaluate(char* command, int len);
void print(char* result);

unsigned long read(char* input_buffer, int buffer_size){
    printf("$ ");
    fflush(stdout);
    fgets(input_buffer, buffer_size, stdin);
    return strlen(input_buffer);
}

void evaluate(char* command, int len){
    command[len - 1] = '\0';
    char builtins[2][4] = {"echo", "exit"};
    int num_of_builtins = 2;
    char result[100];
    for (int i = 0; i < num_of_builtins; i++){
        if (strcmp(builtins[i], command) == 0){
            print(builtins[i]);
            return;
        }
    }
    sprintf(result, "%s: command not found", command);
    print(result);

}

void print(char* result){
    printf("%s\n", result);
}

int main() {
    char input[100];
    while (1){
        read(input, 100);
        evaluate(input, strlen(input));
    }
    
    return 0;
}
