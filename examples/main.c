#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t filho;
    int status;

    // Apartir desta linha, tudo o que tiver abaixo dela,
    // será compartilhado pelo processo filho criado pelo fork()
    filho = fork(); // processo terá o mesmo código do pai apartir daqui

    // a variável "filho", ela pertence tanto ao pai quanto processo filho
    // só que no processo pai, ela recebe um valor que é igual ao pid do processo filho
    // e na execução do processo filho, esta variável esta com o valor 0.

    if (filho == -1) {
        printf("Erro ao criar filho!");
    } else if (filho == 0) {
        // Se filho igual a zero, nesse momento, código do filho.
        printf("Sou o processo filho com pid=%d.\n", getpid());
        for (int i = 0; i < 5; ++i) {
            printf("%d \n ", i);
            sleep(2);
        }
        _exit(EXIT_SUCCESS);
    } else {
        // código do pai
        waitpid(filho, &status, 0);
        // variável "status" armazena o status do término da execução do filho

        printf("Sou o pai com pid=%d. Agora posso executar meu código.\n", getpid());
    }

    exit(EXIT_SUCCESS);
}