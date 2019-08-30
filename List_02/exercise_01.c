#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*************************************************************************
 A chamada de sistema fork é utilizada para a criação de um
 processo filho que é um clone do processo pai. Fazer um programa que:
 - O processo inicial cria 2 processos filhos
 - Cada filho cria mais 2 processos
 Assim, no final teremos 7 processos.
 Os processos, exceto os netos, ficarão esperando o encerramento dos
 respetivos filhos (chamada de sistema wait). Os processos netos
 executarão o comando ls (utilizando a chamada execve). Cada processo
 que estava esperando, após o retorno, imprimem seu respectivo PID e
 encerram.
 ***********************************************************************/

enum TYPE {
    DAD, SON, GRANDSON
};

int main(void) {

    // inicia o tipo como sendo PAI
    enum TYPE type = DAD;

    // variável para guarda o pid do processo
    pid_t pid;

    // cria os FILHOS
    for (int i = 0; i < 2; ++i) {
        if (type == DAD) {
            pid = fork();
            if (pid == 0) {
                type = SON;
            }
        }
    }

    // cria os NETOS
    for (int i = 0; i < 2; ++i) {
        if (type == SON) {
            pid = fork();
            if (pid == 0) {
                type = GRANDSON;
            }
        }
    }

    // se o tipo for diferente de NETOS, espera os netos terminarem a 'tarefa'
    if (type != GRANDSON) {
        while (wait(NULL) > 0);
        printf("Sou o tipo %d e meu pid é %d.\n", type, getpid());
    } else {
        // senão são netos e executa a listagem de diretório
        execlp("/bin/ls", "ls", NULL);
    }

    exit(EXIT_SUCCESS);
}