void sh_echo(char* input, int len);
void sh_exit(char* input);
void sh_type(char* input, char** builtins, int num_of_builtins);
int parse_path(char* path);
char* find_executable(char* executable_name);
int search_directory(char* executable_name, char* dir_name);
int try_execute(char* command);
char** prepair_args(char* command, int* argc);
int clear_after_execute(char** ptr);
