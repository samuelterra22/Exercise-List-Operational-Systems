#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/******************************************************************************
 * No presente exercício, será criado n processos produtores e m consumidores.
 * n e m são parâmetros de entrada do programa. Cada produtor e cada consumidor
 * será implementado em um processo criado pela chamada fork. Para
 * sincronização, semáforos Posix nomeados serão utilizados. O buffer será
 * compartilhado na memória utilizando-se para isso a chamada mmap, com
 * mapeamento do tipo anônimo e compartilhado. Nesse tipo de mapeamento, no
 * momento da chamada fork, a mesma área de memória (da RAM) é entregue aosdois
 * processos, permitindo compartilhamento de dados.
 *****************************************************************************/

#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define FALSE 0
#define TRUE 1

sem_t *mutex;
sem_t *empty;
sem_t *full;

int *buffer;
int *global_item;

#define N 10

int produce_item() {
	*global_item = *global_item + 1;
	printf("Produz item: %d\n", *global_item);
	return *global_item;
}

void consume_item(int item) {
	int n;
	sem_getvalue(full, &n);
	printf("Consome item %d \n", buffer[n]);
}

void insert_item(int item) {
	int n;
	sem_getvalue(full, &n);
	buffer[n] = item;
}

int remove_item() {
	int n;
	sem_getvalue(full, &n);
	return buffer[n];
}

void down(sem_t *sem) {
	sem_wait(sem);
}

void up(sem_t *sem) {
	sem_post(sem);
}

void producer(void) {
	int item;

	while (TRUE) {
		item = produce_item();  /* gera algo para por no buffer */
		down(empty);            /* decresce o contador empty */
		down(mutex);            /* entra na regiao critica */
		insert_item(item);      /* poe novo item no buffer */
		up(mutex);              /* sai da regiao critica */
		up(full);               /* incrementa o contador de lugares preenchidos */

		usleep(100 * 1000);
	}
}

void consumer(void) {
	int item;

	while (TRUE) {
		down(full);             /* decresce o contador full */
		down(mutex);            /* entra na regiao critica */
		item = remove_item();   /* pega item do buffer */
		up(mutex);              /* sai da regiao critica */
		up(empty);              /* incrementa o contador de lugares vazios */
		consume_item(item);     /* faz algo com o item */

		usleep(300 * 1000);
	}
}

int main(int argc, char **argv) {

	mutex = sem_open("/mutex_sem", O_CREAT, 0644, 1);
	empty = sem_open("/empty_sem", O_CREAT, 0644, N);
	full = sem_open("/full_sem", O_CREAT, 0644, 0);

	int n, m;
	pid_t pid;

	printf("Informe o valor de m consumidores: ");
	scanf("%d", &m);

	printf("Informe o valor de n produtores: ");
	scanf("%d", &n);

	buffer = (int *) mmap(NULL, sizeof(int) * N, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
	global_item = (int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

	if (buffer == MAP_FAILED) {
		printf("NMAP Error\n");
		return EXIT_FAILURE;
	}

	if ((pid = fork()) == -1) {
		printf("Error when make fork\n");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < m; ++i) {
		pid = fork();
		if (pid == 0) {
			producer();
		}
	}

	for (int i = 0; i < n; ++i) {
		pid = fork();
		if (pid == 0) {
			consumer();
		}
	}

	while (wait(NULL) > 0);

	munmap(buffer, sizeof(int) * N);
	munmap(global_item, sizeof(int));

	sem_destroy(mutex);
	sem_destroy(empty);
	sem_destroy(full);

	sem_unlink("/mutex_sem");
	sem_unlink("/empty_sem");
	sem_unlink("/full_sem");

	return EXIT_SUCCESS;
}