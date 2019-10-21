#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>

/******************************************************************************
 * O problema 1 desta lista será implementado com threads e mutexes (POSIX).
 *****************************************************************************/

#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define FALSE 0
#define TRUE 1

pthread_mutex_t mutex;
pthread_mutex_t empty;
pthread_mutex_t full;

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
	// sem_getvalue(full, &n);
	printf("Consome item %d \n", buffer[n]);
}

void insert_item(int item) {
	int n;
	// sem_getvalue(full, &n);
	buffer[n] = item;
}

int remove_item() {
	int n;
	// sem_getvalue(full, &n);
	return buffer[n];
}

void down(pthread_mutex_t *sem) {
	pthread_mutex_lock(sem);
}

void up(pthread_mutex_t *sem) {
	pthread_mutex_unlock(sem);
}

void *producer(void *args) {
	int item;

	while (TRUE) {
		item = produce_item();  /* gera algo para por no buffer */
		down(&empty);            /* decresce o contador empty */
		down(&mutex);            /* entra na regiao critica */
		insert_item(item);      /* poe novo item no buffer */
		up(&mutex);              /* sai da regiao critica */
		up(&full);               /* incrementa o contador de lugares preenchidos */

		usleep(100 * 1000);
	}
}

void *consumer(void *args) {
	int item;

	while (TRUE) {
		down(&full);             /* decresce o contador full */
		down(&mutex);            /* entra na regiao critica */
		item = remove_item();   /* pega item do buffer */
		up(&mutex);              /* sai da regiao critica */
		up(&empty);              /* incrementa o contador de lugares vazios */
		consume_item(item);     /* faz algo com o item */

		usleep(300 * 1000);
	}
}

int main(int argc, char **argv) {

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&empty, NULL);
	pthread_mutex_init(&full, NULL);

	int n, m;

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

	int *ids_producer = malloc(n * sizeof(int));
	pthread_t *thread_producer_id = malloc(n * sizeof(pthread_t));

	int *ids_costumer = malloc(m * sizeof(int));
	pthread_t *thread_consumer_id = malloc(m * sizeof(pthread_t));

	for (int i = 0; i < n; i++) {
		ids_producer[i] = i;
		if (pthread_create(&thread_producer_id[i], NULL, producer, (void *) &ids_producer[i]) != 0) {
			printf("Producer thread [%d] not created.\n", i);
			exit(0);
		}
	}

	for (int i = 0; i < m; i++) {
		ids_costumer[i] = i;
		if (pthread_create(&thread_consumer_id[i], NULL, consumer, (void *) &ids_costumer[i]) != 0) {
			printf("Consumer thread [%d] not created.\n", i);
			exit(0);
		}
	}

	munmap(buffer, sizeof(int) * N);
	munmap(global_item, sizeof(int));

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&empty);
	pthread_mutex_destroy(&full);

	return EXIT_SUCCESS;
}