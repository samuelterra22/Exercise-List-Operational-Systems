#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * Bloqueio de arquivo. A chamada ​ flock() ​ é utilizada para colocar um lock
 * em um determinado arquivo. Esse lock pode ser do tipo compartilhado ou
 * exclusivo. Vc deve tentar resolver o problema dos leitores e escritores
 * utilizando ​ flock ​ da seguinte forma: processos leitores devem utilizar um
 * lock compartilhado que permite vários leitores leiam o arquivo ao mesmo
 * tempo. Já um processo escritor deve utilizar um lock exclusivo, visto que
 * ele irá modificar o arquivo. Deve ser possível, no terminal, iniciar um
 * processo (leitor ou escritor) e indicar quanto tempo ele deve ficar com o
 * lock no arquivo indicado pelo usuário.
 *****************************************************************************/

int main(int argc, char **argv) {

	printf("Exercicio 03");

	return EXIT_SUCCESS;
}
