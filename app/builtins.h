void sh_echo(char* input, int len);
void sh_exit(char* input, int len);
void sh_type(char* input, int len, char* builtins, int num_of_builtins);
int parse_path(char* path);
char* find_executable(char* executable_name);
int search_directory(char* executable_name, char* dir_name);
