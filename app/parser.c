#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include/parser.h"

command_t parse_command(char* src) {
    command_t command = {};
    command.args = parse_args(src);
    command.command = command.args->arg;
    return command;
}

args_t* parse_args(char* args) {
    args_t* args_container = malloc(sizeof(args_t));
    args_t* res = args_container;
    char** tokens = parse(args);
    args_container->arg = *(tokens);
    // while there's more args, we're parse them
    for (int i = 1; *(tokens + i) != NULL; i++){
        args_t* new_arg = malloc(sizeof(args_t));
        new_arg->arg = *(tokens + i);
        args_container->next = new_arg;
        args_container = new_arg;
    }
    args_container->next = NULL;
    free(tokens);

    return res;
}

char** parse(char* src) {
    char* tok_buf[512] = {NULL};
    int len = strlen(src);
    int tokens_cnt = 0;
    int src_offset = 0;
    while (src_offset < len) {
        char buffer[512] = {0};
        src_offset = next_token(src, len, src_offset, buffer, 512);
        char* tok = malloc(sizeof(char) * (strlen(buffer) + 1));
        memcpy(tok, buffer, strlen(buffer) + 1);
        tok_buf[tokens_cnt] = tok;
        tokens_cnt++;
    }
    char** tokens = malloc(sizeof(char*) * (tokens_cnt + 1));
    for (int i = 0; i < tokens_cnt; i++){
        *(tokens + i) = tok_buf[i];
    }
    *(tokens + tokens_cnt) = NULL;
    return tokens;
}

int next_token(char* src, int len, int offset, char* buffer, int buffer_size) {
    int res;
    if (*(src + offset) == '\''){
        res = parse_signle_quoted_string(src + offset, len - offset, buffer, buffer_size);
        offset += res;
    } else if (*(src + offset) == '"'){
        res = parse_double_quited_string(src + offset, len - offset, buffer, buffer_size);
        offset += res;
    } else {
        int i = 0;
        while (offset < len && *(src + offset) != ' ' && *(src + offset) != '\n'){
            /* на данный момент не учитывает экранирование через \ без кавычек */
            *(buffer + i) = *(src + offset);
            i++; offset++;
        }
        *(buffer + i) = '\0';
    }
    return offset + 1;
}

int parse_signle_quoted_string(char* src, int len, char* buffer,
                               int buffer_size) {

}

int parse_double_quited_string(char* src, int len, char* buffer,
                                 int buffer_size) {

}
