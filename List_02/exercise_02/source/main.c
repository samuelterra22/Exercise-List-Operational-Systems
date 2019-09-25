#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

/******************************************************************************
 * No presente exercício, o processo inicial deve criar 3 processos filhos.
 * Cada filho criará mais 3 processos. Assim, teremos ao todo 13 processos.
 * Cada processo neto sorteará um número aleatório entre 0 e 100 e o mandará
 * para o processo filho através de um pipe. O processo filho ordenará os
 * números recebidos em ordem crescente e os mandará para o pai através do
 * pipe. O processo pai agora fará a ordenação dos dados recebidos e os
 * imprimirá na tela. Note que a ordernação deve ser feita utilizando-se a
 * função merge (como no mergesort).
 *****************************************************************************/

enum TYPE {
    DAD, SON, GRANDSON
};

#define READ 0
#define WRITE 1

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int main(int argc, const char *argv[]) {

    // initialize as DAD
    enum TYPE type = DAD;
    int DAD_ID = 0;
    int SON_ID = 0;

    // variable to store process pid
    pid_t pid;

    // file descriptor to dad <-> son
    int FD_DAD_SON[3][2];

    // file descriptor to son <-> grandson
    int FD_SON_GRANDSON[3][2];

    for (int j = 0; j < 3; ++j) {
        if (pipe(FD_DAD_SON[j]) == -1) {
            printf("FD_DAD_SON[%d] pipe failed\n", j);
            return EXIT_FAILURE;
        }
    }

    for (int j = 0; j < 3; ++j) {
        if (pipe(FD_SON_GRANDSON[j]) == -1) {
            printf("FD_SON_GRANDSON[%d] pipe failed\n", j);
            return EXIT_FAILURE;
        }
    }

    // create sons
    for (int i = 0; i < 3; ++i) {
        if (type == DAD) {
            pid = fork();
            if (pid == 0) {
                DAD_ID = i;
                type = SON;
            }
        }
    }

    // create grandsons
    for (int i = 0; i < 3; ++i) {
        if (type == SON) {
            pid = fork();
            if (pid == 0) {
                SON_ID = i;
                type = GRANDSON;
            }
        }
    }

    // se o tipo for FILHOS, espera todos seus filhos (netos)
    if (type == SON) {
        while (wait(NULL) > 0);

        // variable to read number sent
        int read_buffer;
        int son_array_received[3];

        for (int i = 0; i < 3; ++i) {
            // Parent process closes up output side of pipe
            close(FD_SON_GRANDSON[i][WRITE]);

            // Read in a string from the pipe
            read(FD_SON_GRANDSON[i][READ], &read_buffer, sizeof(int));
            son_array_received[i] = read_buffer;
        }

        // sort array
        if (son_array_received[0] > son_array_received[2]) {
            swap(&son_array_received[0], &son_array_received[2]);
        }
        if (son_array_received[0] > son_array_received[1]) {
            swap(&son_array_received[0], &son_array_received[1]);
        }
        if (son_array_received[1] > son_array_received[2]) {
            swap(&son_array_received[1], &son_array_received[2]);
        }

        close(FD_DAD_SON[DAD_ID][READ]);

        // Send the number through the output side of pipe
        write(FD_DAD_SON[DAD_ID][WRITE], son_array_received, sizeof(int) * 3);

        printf("Sent: %d %d %d\n", son_array_received[0], son_array_received[1], son_array_received[2]);

        _exit(0);

    }
        // se for netors, gera numero aleátorio e envia para os pais
    else if (type == GRANDSON) {
        close(FD_SON_GRANDSON[SON_ID][READ]);

        srand(time(NULL) ^ getpid() << 16);
        int number = rand() % 101;

        // Send the number through the output side of pipe
        write(FD_SON_GRANDSON[SON_ID][WRITE], &number, sizeof(int));
        _exit(0);
    }
        // senão é pai, pega o vetor de inteiros e printa ordenado
    else {
        while (wait(NULL) > 0);
        printf("Sou o pai.\n");

        int read_buffer_array[3];
        int matrix_buffer[3][3];
        int result_array[3 * 3];

        for (int i = 0; i < 3; ++i) {
            // Parent process closes up output side of pipe
            close(FD_DAD_SON[i][WRITE]);

            // Read in a string from the pipe
            read(FD_DAD_SON[i][READ], read_buffer_array, sizeof(int) * 3);
            for (int j = 0; j < 3; ++j) {
                matrix_buffer[i][j] = read_buffer_array[j];
            }
        }

        int LIM = 3;
        int lim_a = 0, lim_b = 0, lim_c = 0, count = 0;

        while (lim_a < LIM || lim_b < LIM || lim_c < LIM) {
            if (
                    lim_a < LIM && matrix_buffer[0][lim_a] < matrix_buffer[1][lim_b] &&
                    matrix_buffer[0][lim_a] < matrix_buffer[2][lim_c]
                    ) {
                result_array[count] = matrix_buffer[0][lim_a];
                lim_a++;
            } else if (lim_b < LIM && matrix_buffer[1][lim_b] < matrix_buffer[2][lim_c]) {
                result_array[count] = matrix_buffer[1][lim_b];
                lim_b++;
            } else if (lim_c < LIM) {
                result_array[count] = matrix_buffer[2][lim_c];
                lim_c++;
            }

            count++;
        }

        for (int k = 0; k < 9; ++k) {
            printf("%d ", result_array[k]);
        }

    }

    return EXIT_SUCCESS;
}