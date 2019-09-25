#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * No presente exercício, um interpretador de comandos deve ser criado. Ele
 * deve apresentar um sinal que está aguardando comandos do usuário (por
 * exemplo ">"), receber um comando, separá-lo utilizando strtok e executar
 * comandos com uma função da família exec (para isso o fork também deve ser
 * utilizado). O shell deve aguardar o processo teminar com wait. Além de
 * executar comandos externos, o shell deve ter também os seguintes comandos
 * internos: cd (utilizando a chamada de sistema chdir), mkdir, rmdir, mv, exit
 * (para terminar o shell). Também deve ser possível a execução de 2 comandos
 * ligados por pipe (por exemplo: "ls | grep teste").
 *****************************************************************************/

#define FALSE 0
#define TRUE 1

int main(int argc, const char *argv[]) {

    char command[100];
    int exit_shell = FALSE;

    while (!exit_shell) {
        printf("> ");
        scanf("%s", command);


        if (strcmp(command, "exit") == 0) {
            exit_shell = TRUE;
            printf("God bye!\n");
        }
    }


    return EXIT_SUCCESS;
}