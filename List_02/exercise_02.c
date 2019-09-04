#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

/*************************************************************************
 No presente exercício, o processo inicial deve criar 3 processos filhos.
 Cada filho criará mais 3 processos. Assim, teremos ao todo 13 processos.
 Cada processo neto sorteará um número aleatório entre 0 e 100 e o mandará
 para o processo filho através de um pipe. O processo filho ordenará os
 números recebidos em ordem crescente e os mandará para o pai através do
 pipe. O processo pai agora fará a ordenação dos dados recebidos e os
 imprimirá na tela. Note que a ordernação deve ser feita utilizando-se a
 função merge (como no mergesort).
 ************************************************************************/

enum TYPE {
    DAD, SON, GRANDSON
};

#define READ 0
#define WRITE 1

int main(int argc, const char *argv[]) {

    // inicia o tipo como sendo PAI
    enum TYPE type = DAD;

    // variável para guarda o pid do processo
    pid_t pid;

    // file descriptor
    int file_descriptor[3][2];

    pipe(file_descriptor[0]);
    pipe(file_descriptor[1]);
    pipe(file_descriptor[2]);

    // cria os FILHOS
    for (int i = 0; i < 3; ++i) {
        if (type == DAD) {
            pid = fork();
            if (pid == 0) {
                type = SON;
            }
        }
    }

    // cria os NETOS
    for (int i = 0; i < 3; ++i) {
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

        /* Parent process closes up output side of pipe */
        close(file_descriptor[0][WRITE]);

        int readbuffer;

        /* Read in a string from the pipe */
        read(file_descriptor[0][READ], &readbuffer, sizeof(int));
        printf("Received: %d\n", readbuffer);
    } else {
        close(file_descriptor[0][READ]);

        srand(time(NULL) ^ getpid() << 16);
        int number = rand() % 101;

        printf("Sent: %d\n", number);

        /* Send "string" through the output side of pipe */
        write(file_descriptor[0][WRITE], &number, sizeof(int));
        _exit(0);
    }

    exit(EXIT_SUCCESS);
}