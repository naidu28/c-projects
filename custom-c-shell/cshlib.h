#ifndef CSH_LIB_H
#define CSH_LIB_H

int csh_num_builtins();
int csh_cd(char **args);
int csh_help(char **args);
int csh_exit(char **args);

char *builtin_str[];
int (*builtin_func[]) (char**);

#endif