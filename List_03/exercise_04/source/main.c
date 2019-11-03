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
	int spaces = 0;

	/* search empty spaces */
	for (int i = 0; i < strlen(string); ++i)
		if (string[i] == ' ')
			spaces++;

	return spaces;
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

void execute_simple_command(char *command, char **args) {
	if (execvp(command, args) == -1) {
		printf("Erro ao executar o comando!\n");
		_exit(EXIT_SUCCESS);
	}
}

int main(int argc, const char *argv[]) {

	char command[MAX_COMMAND_LENGTH];
	int exit_shell = FALSE;

	pid_t pid;
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
			pid = fork();

			if (pid == 0) {
				char **argvs = explode_command(command);

				execute_simple_command(command_base, argvs);

				printf("\n");
			} else {
				while (wait(NULL) > 0);
				printf("> ");
			}
		} else {
			exit_shell = TRUE;
			printf("God bye!\n");
		}

		strcpy(command_base, "/bin/");
	}


	return EXIT_SUCCESS;
}
