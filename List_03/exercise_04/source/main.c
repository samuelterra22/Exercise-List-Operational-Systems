#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/******************************************************************************
 * No presente exercício, um interpretador de comandos deve ser criado. Ele
 * deve apresentar um sinal que está aguardando comandos do usuário (por
 * exemplo ">"), receber um comando, separá-lo utilizando strtok e executar
 * comandos com uma função da família exec (para isso o fork também deve ser
 * utilizado). O shell deve aguardar o processo teminar com wait. Além de
 * executar comandos externos, o shell deve ter também os seguintes comandos
 * internos: cd (utilizando a chamada de sistema chdir), mkdir, rmdir, mv, exit
 * (para terminar o shell). Também deve ser possível a execução de 2 comandos
 * ligados por pipe (por exemplo: "ls | grep teste"). dup2
 *****************************************************************************/

#define FALSE 0
#define TRUE 1

#define MAX_COMMAND_LENGTH 100

int num_of_empty_spaces(char *string) {
	int empty_spaces = 0;

	/* search empty spaces */
	for (int i = 0; i < strlen(string); ++i)
		if (string[i] == ' ')
			empty_spaces++;

	return empty_spaces;
}

char **explode_command(char *str) {
	const char s[2] = " ";
	char *token;
	int i = 0;

	int blanks = num_of_empty_spaces(str);

	/* alloc memory for array arguments */
	char **ret = malloc(sizeof(char *) * (blanks + 2));

	/* get the first token */
	token = strtok(str, s);

	while (token != NULL) {
		ret[i] = token;
		token = strtok(NULL, s);
		i++;
	}

	ret[blanks + 1] = NULL;

	return ret;
}

void execute_pipe_command(char *first_command, char *second_command) {
	int fd[2];
	pid_t pid1, pid2;
	char command_base[] = "/bin/";

	if (pipe(fd) < 0) {
		printf("Pipe error\n");
		return;
	}

	pid1 = fork();
	if (pid1 < 0) {
		printf("Fork error\n");
		return;
	}

	if (pid1 == 0) {
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);

		char **args1 = explode_command(first_command);
		printf("args1[0]: %s\n", args1[0]);
		char *path_to_bin_command_1 = strcat(command_base, args1[0]);
		if (execve(path_to_bin_command_1, args1, 0) < 0) {
			printf("Command not found in %s\n", path_to_bin_command_1);
			exit(0);
		}
	} else {
		pid2 = fork();

		if (pid2 < 0) {
			printf("Fork error");
			return;
		}

		if (pid2 == 0) {
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);

			char **args2 = explode_command(second_command);
			char *path_to_bin_command_2 = strcat(command_base, args2[0]);
			if (execve(path_to_bin_command_2, args2, 0) < 0) {
				printf("Command not found in %s\n", path_to_bin_command_2);
				exit(0);
			}
		} else {
			while (wait(NULL) < 0);
			printf("> ");
		}
	}
}

void execute_simple_command(char *command, char **args) {
	pid_t pid;

	pid = fork();

	if (pid == 0) {
		if (execvp(command, args) == -1) {
			printf("Erro ao executar o comando!\n");
			_exit(EXIT_SUCCESS);
		}
	} else {
		/* aguarda processo filho */
		while (wait(NULL) > 0);
		printf("> ");
	}
}

int main(int argc, const char *argv[]) {

	char command[MAX_COMMAND_LENGTH];
	int exit_shell = FALSE;

	char command_base[] = "/bin/";

	printf("Custom bash. Type 'exit' to quit.\n");
	printf("> ");
	while (!exit_shell) {
		/* read command from terminal */
		fgets(command, MAX_COMMAND_LENGTH, stdin);

		/* remove the 'enter' key */
		command[strlen(command) - 1] = '\0';

		char command_bkp[MAX_COMMAND_LENGTH];
		strcpy(command_bkp, command);

		strcat(command_base, command);

		if (strcmp(command_bkp, "exit") != 0) {

			/* quebra a linha no pipe (|) */
			char *first_command = strtok(command_bkp, "|");
			char *second_command = strtok(NULL, "");

			/* execute a simple command */
			if (second_command == NULL) {
				char **argvs = explode_command(command);
				execute_simple_command(command_base, argvs);
			}
				/* execute command with pipe */
			else {
				execute_pipe_command(first_command, second_command);
			}

			printf("\n");
		} else {
			/* sai do bash */
			exit_shell = TRUE;
			printf("God bye!\n");
		}

		/* 'reseta' o valor de command_base */
		strcpy(command_base, "/bin/");
	}

	return EXIT_SUCCESS;
}
