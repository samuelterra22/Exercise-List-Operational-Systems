#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>

/******************************************************************************
 * Bloqueio de arquivo. A chamada ​ flock() ​ é utilizada para colocar um lock
 * em um determinado arquivo. Esse lock pode ser do tipo compartilhado ou
 * exclusivo. Voce deve tentar resolver o problema dos leitores e escritores
 * utilizando ​ flock ​ da seguinte forma: processos leitores devem utilizar um
 * lock compartilhado que permite vários leitores leiam o arquivo ao mesmo
 * tempo. Já um processo escritor deve utilizar um lock exclusivo, visto que
 * ele irá modificar o arquivo. Deve ser possível, no terminal, iniciar um
 * processo (leitor ou escritor) e indicar quanto tempo ele deve ficar com o
 * lock no arquivo indicado pelo usuário.
 *****************************************************************************/

void writer(char *file_name, long int time) {
    /* abre o arquivo */
    int fd = open(file_name, O_CREAT | O_WRONLY);

    printf("Escritor ira bloquear o arquivo '%s'.\n", file_name);

    /* make exclusive lock */
    flock(fd, LOCK_EX);

    printf("Escritor bloqueou o arquivo '%s' para escriva exclusiva por %d segundos.\n", file_name, time);

    /* sleep for some time */
    sleep(time);

    /* remove lock */
    flock(fd, LOCK_UN);

    printf("Escritor desbloqueou o arquivo '%s'.\n", file_name);

    /* close file descriptor */
    close(fd);
}

void reader(char *file_name, long int time) {
    /* open file */
    int fd = open(file_name, O_CREAT | O_RDWR);

    printf("Leitor ira bloquear o arquivo '%s' de forma compartilhada.\n", file_name);

    /* make shared lock */
    flock(fd, LOCK_SH);

    printf("Leitor bloqueou o arquivo '%s' para escriva compartilhada por %d segundos.\n", file_name, time);

    /* sleep for some time */
    sleep(time);

    /* remove lock  */
    flock(fd, LOCK_UN);

    printf("Leitor desbloqueou o arquivo.\n");

    /* close file descriptor */
    close(fd);
}

int main(int argc, char **argv) {
    /*************************************
     * Input example:
     * $ ./exercise01 writer file.txt 3
     * $ ./exercise01 reader file.txt 3
     ************************************/
    char *type = argv[1];
    char *file_name = argv[2];
    long int time = strtol(argv[3], NULL, 10);

    if (strcmp(type, "writer") == 0) {
        writer(file_name, time);
    } else if (strcmp(type, "reader") == 0) {
        reader(file_name, time);
    } else {
        printf("Entered type is unknown\n");
    }

    return EXIT_SUCCESS;
}
