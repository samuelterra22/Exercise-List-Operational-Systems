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

int main(int argc, const char *argv[]) {

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
        // senão são netos e executa sorteia um numero
        srand(time(NULL) ^ getpid() << 16);
        int r = rand() % 101;
        printf("%d \n", r);

        // envia por pipe
    }

    exit(EXIT_SUCCESS);
}