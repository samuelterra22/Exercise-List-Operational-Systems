#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

/******************************************************************************
 * Existe uma chamada de sistema que é responsável por retornar informações
 * sobre um arquivo. Fazer um programa que, dado o caminho de um arquivo,
 * retorne as seguintes informações:
 * - Identificação do dono do arquivo
 * - Identificação do grupo do arquivo
 * - Tamanho do arquivo
 * - Número de links do arquivo
 *****************************************************************************/

int main(int argc, char **argv) {

    char *path = argv[1];
    struct stat file_stat;
    int file;

    if (argc < 2) {
        printf("O caminho do arquivo deve ser informado!\n");
        return EXIT_FAILURE;
    }

    if ((file = open(path, O_RDONLY)) < -1) {
        return EXIT_FAILURE;
    }

    if (fstat(file, &file_stat) < 0) {
        return EXIT_FAILURE;
    }

    printf("\n------------------------------------\n");
    printf("Informações para: \n -> %s\n", path);
    printf("------------------------------------\n");
    printf("ID do usuário: \t\t%d\n", file_stat.st_uid);
    printf("ID do grupo: \t\t%d\n", file_stat.st_gid);
    printf("Tamanho do arquivo: \t%ld bytes\n", file_stat.st_size);
    printf("Número de Links: \t%ld\n", file_stat.st_nlink);
    printf("------------------------------------\n");

    printf("\n\n");

    return EXIT_SUCCESS;
}