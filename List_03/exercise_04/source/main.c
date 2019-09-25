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

#define MAX_COMMAND_LENGHT 100

char **explode(char *str) {
    int i = 0, cont = 0;
    const char s[2] = " ";
    char *token;

    /* get the first token */
    token = strtok(str, s);

    char **ret = malloc(sizeof(*ret));

    while (token != NULL) {
        // printf(" %s\n", token);

        ret = realloc(ret, (i + 1) * sizeof(ret));
        ret[i] = malloc(254 * sizeof(char *));
        strcpy(ret[i], token);

        printf(" %s\n", ret[i]);

        token = strtok(NULL, s);
        i++;
    }

    return ret;
}

int main(int argc, const char *argv[]) {

    char command[MAX_COMMAND_LENGHT];
    int exit_shell = TRUE;

    pid_t pid;
    char command_base[] = "/bin/";

    char asd[] = "ls -a -l";

    char **c = explode(asd);
    for (int j = 0; j < sizeof(**c) - 1; j++) printf("%s\t", c[j]);

    printf("> ");
    while (!exit_shell) {
        scanf("%s", command);

        pid = fork();

        strcat(command_base, command);

        if (pid == 0) {
            char *argvs[3] = {command, NULL};
            if (execvp(command_base, argvs) == -1) {
                printf("Erro - Comando nao existe\n");
                _exit(EXIT_SUCCESS);
            }


            printf("\n");
        } else {
            while (wait(NULL) > 0);
            printf("> ");
        }

        if (strcmp(command, "exit") == 0) {
            exit_shell = TRUE;
            printf("God bye!\n");
        }

        strcpy(command_base, "/bin/");
    }


    return EXIT_SUCCESS;
}
