#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

/******************************************************************************
 * No presente exercício, será criado n processos produtores e m consumidores.
 * n e m são parâmetros de entrada do programa. Cada produtor e cada consumidor
 * será implementado em um processo criado pela chamada fork​. Para 
 * sincronização, semáforos Posix nomeados serão utilizados. O buffer será
 * compartilhado na memória utilizando-se para isso a chamada mmap, com 
 * mapeamento do tipo ​anônimo e compartilhado​. Nesse tipo de mapeamento, no
 * momento da chamada fork, a mesma área de memória (da RAM) é entregue aosdois
 * processos, permitindo compartilhamento de dados.
 *****************************************************************************/

int main(int argc, char **argv) {

	printf("Exercicio 01");

	return EXIT_SUCCESS;
}
