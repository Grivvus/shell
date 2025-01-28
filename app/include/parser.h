#pragma once

#include "command.h"

command_t parse_command(char* src);
args_t* parse_args(char* args);
char** parse(char* src);
int next_token(char* src, int len, int offset, char* buffer, int buffer_size);
int parse_signle_quoted_string(char* src, int len, char* buffer,
                               int buffer_size);
int parse_double_quited_string(char* src, int len, char* buffer,
                                 int buffer_size);
