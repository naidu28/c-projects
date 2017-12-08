#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "cshlib.h"

#define CSH_RL_BUFSIZE 1024
char *csh_read_line(void)
{
	int bufsize = CSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer) {
		fprintf(stderr, "csh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		// Read a character
		c = getchar();

		// If we hit EOF, replace it with a null character and return
		if (c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else {
			buffer[position] = c;
		}
		position++;

		// If we have exceeded the buffer, reallocate
		if (position >= bufsize) {
			bufsize += CSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
				fprintf(stderr, "csh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define CSH_TOK_BUFSIZE 64
#define CSH_TOK_DELIM " \t\r\n\a"
char **csh_split_line(char *line)
{
	int bufsize = CSH_TOK_BUFSIZE, position = 0;
	// we malloc because the number of tokens is unknown
	// so we will need to dynamically allocate memory
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "csh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, CSH_TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += CSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "csh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, CSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

int csh_launch(char **args)
{
	// fork, makes a new process
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// child process
		if (execvp(args[0], args) == -1) {
			perror("csh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// error forking
		perror("csh");
	} else {
		// parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
	// exec
	// wait
}

int csh_execute(char **args)
{
	int i;

	if (args[0] == NULL) {
		return 1;
	}

	for (i = 0; i < csh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	return csh_launch(args);
}

void csh_loop(void)
{
	char *line;
	char **args;
	int status;

	do {
		printf("> ");
		line = csh_read_line();
		args = csh_split_line(line);
		status = csh_execute(args);

		free(line);
		free(args);
	} while (status);
}

int main(int argc, char **argv)
{
	// load config files

	// run command loop
	csh_loop();

	// perform shutdown/cleanup
	return EXIT_SUCCESS;
}
