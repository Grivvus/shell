#pragma once
#include <stdbool.h>
#include "command.h"

bool in_builtins(char* name);
int relative_to_absolute(char* relative, char* buffer, int buffer_size);
int parse_path(char* path);
char* find_executable(char* executable_name);
int search_directory(char* executable_name, char* dir_name);
char** prepair_args(command_t command);
