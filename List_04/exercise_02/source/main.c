#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/******************************************************************************
 * Solução de Peterson. Nesse exercício, um contador compartilhado será 
 * acessado por 2 threads simultaneamente. Elas devem pegar o valor do 
 * contador, imprimir na tela, executar thread_yield, somar um no contador.
 * Duas versões devem ser feitas: uma sem nenhum controle de condição de
 * corrida e outra utilizando a solução de Peterson para isso.
 *****************************************************************************/

#define FALSE    0
#define TRUE    1
#define N        2    /* Número de processos */

int turn;            /* De quem é a vez? */
int interested[N];    /* Todos os valores como 0 (FALSE)*/

int counter = 0;    /* Contador global */

typedef void *(FUNC)(void *);

void enter_region(int process) {
	int other;                    /* Número do outro processo */
	other = 1 - process;        /* O oposto do processo */
	interested[process] = TRUE;    /* Mostra que você está interessado */
	turn = process;                /* Altera o valor de turn */
	while (turn == process && interested[other] == TRUE);    /* Comando nulo */
}

void leave_region(int process) {
	/* Indica saida da região crítica */
	interested[process] = FALSE;
}

void *thread_peterson(void *vargp) {
	int *myid = (int *) vargp;

	for (int i = 0; i < N * 5; i++) {
		enter_region(*myid);

		int local_counter = counter;
		printf("Thread %d, Value of counter: %d\n", *myid, local_counter);
		pthread_yield();
		counter = local_counter + 1;

		leave_region(*myid);
	}

	return NULL;
}

void *thread(void *vargp) {
	int *myid = (int *) vargp;

	for (int i = 0; i < N * 5; i++) {
		int local_counter = counter;
		printf("Thread %d, Value of counter: %d\n", *myid, local_counter);
		pthread_yield();
		counter = local_counter + 1;
	}

	return NULL;
}

int main(int argc, const char *argv[]) {
	pthread_t thread_id[N];
	int ids[N];

	FUNC *functions[3];
	functions[0] = &thread;
	functions[1] = &thread_peterson;

	for (int i = 0; i < N; i++) {
		ids[i] = i;
		if (pthread_create(&thread_id[i], NULL, thread_peterson, (void *) &ids[i]) != 0) {
			printf("Thread not created.\n");
			exit(0);
		}
	}

	for (int i = 0; i < N; i++) {
		pthread_join(thread_id[i], NULL);
	}

	return EXIT_SUCCESS;
}
