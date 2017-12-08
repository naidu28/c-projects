#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cshlib.h"

char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char**) = {
	&csh_cd,
	&csh_help,
	&csh_exit
};

int csh_num_builtins() 
{
	return sizeof(builtin_str) / sizeof(char *);
}

int csh_cd(char **args) 
{
	if (args[1] == NULL) {
		fprintf(stderr, "csh: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("csh");
		}
	}
	return 1;
}

int csh_help(char **args) 
{
	int i;
	printf("Sai Naidu's CSH\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < csh_num_builtins(); i++) {
		printf(" %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}


int csh_exit(char **args)
{
	return 0;
}
